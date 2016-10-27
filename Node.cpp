//Kadelima Node Class

#include "Node.h"
#include <algorithm>

//Pre: id is a valid node id that is not yet taken
//Post: RT is initalized, ID = id, inNetwork = true
//      RT is empty since this node is starting a new network
Node::Node(uint32_t id) : RT(id){
  ID = id;
  inNetwork = true;
}

//Pre: msg, queue, and timeOut were declared in the constructor below
//Post: the id of the node sending msg is removed from timeOut
//      if our id is in closest times, return true, false other wise
void Node::handleKClosMsg(Message msg, vector<MsgTimer>& timeOut,
													JoinNetworkQueue& queue) {
	bool isMyIDInMsg = false;
	uint32_t senderID = msg.getID();
	uint32_t senderIP = msg.get();
	RT.updateTable(senderID, senderIP, PORT);
	bool found = false;
	int index = 0;
	MsgTimer currTimer;
	while (!found) {
		currTimer = timeOut[timeOut.begin() + index];
		if (currTimer.getID() == senderID) {
			found = true;
			timeOut.erase(timeOut.begin() + index);
		}
		index++;
	} //the sender node is now removed from the time out vector
	if (msg.includes(ID)) {  //This node knows us
		isMyIDInMsg = true;
	}
	//Continue adding in the nodes we have not asked yet
	Triple closestK[K];
	int size = msg.getKClos(closestK);
	for (int index = 0; (index < size); index) {
		queue.add(closestK[index]);
	}
	if (!inNetwork) { //no need to check if we're already in network
		inNetwork = myIdInMsg;
	}
}

//Pre: timer is from the constructor
//Post: Removes elements from timer that have timed out
Node::clearTimeOut(Vector<MsgTimer>& timer) {
  int size = timer.size(); //number of elements in timer
  MsgTimer currTimer;
  for (int index = 0; (index < size); index++){
    currTimer = timer[timeOut.begin() + index];
    if (currTimer.timedOut()) {
      timeOut.erase(timeOut.begin() + index);
    }
  }
}
  
//Pre: id is a valid node id that is not yet taken, contactID and contactIP
//     belongs to a node already existing in the network
//Post: ID = id, contact exists within our routing table, as well as
//      other nodes our contact has told about us
//      inNetwork = true if FindNode on ourselves succeds, false otherwise
Node::Node(uint32_t id, uint32_t contactID, uint32_t contactIP) : RT(id) {
  ID = id;
  RT.updateTable(contactID, contactIP);
  inNetwork = false; //at this point, no other node knows about us
  vector<MsgTimer> timeOut();
  UDPSocket socket(UIPORT);
  Message firstMsg(FINDNODE, ID, "FINDNODE", ID);
  socket.sendMessage(firstMsg, contactIP, MAINPORT);
  timeOut.push_back(MsgTimer(RESPONSETIME, contactID, contactIP));
  Triple contactTriple(contactID, contactIP, MAINPORT);
  JoinNetworkQueue nodesToAsk(contactID, contactIP);
  while (nodesToAsk.hasNext() and !RT.isFull()) {
    string resposne;
    int msgSize = socket.recvMessage(response);
    if (msgSize != -1) {
      Message msg(response);
      if (msg.getMsgType() == KCLOSEST) {
	bool myIdInMsg = handleKClosMsg(msg, timeOut, nodesToAsk);
      } //Done Dealing with a received message
    }
    Triple nextToAsk = nodesToAsk.getNext();
    Message toSend(FINDNODE, ID, "FINDNODE", ID);
    socket.sendMessage(toSend.toString(), nextToAsk.address, nextToAsk.port);
    clearTimeOut(timeOut);
  }
}

//--------------------------------Node Listener----------------------------

//Handles messages from other Nodes.
//Everything is constant time
//MAIN: port 6666
//      READS:  STORE, FINDNODE, FINDVALUE
//      SENDS:  KCLOSEST, FVRESP
void Node::startListener(){
 
  //       When we send a message, make sure we've got
  //       6666 included so people know to respond to the
  //       right one.
  //       WHENEVER WE SEND K CLOSEST, SEND TO 6667
	
  thread PingThread = thread(startRefresher);
  thread UIThread = thread(startUIListener);
  //ASSERT: Create the two threads for handling Pings and
  //        for handling UIs
	
  Message sendMessageOBJ();
  //ASSERT: empty message object to send later
	
  std::string sendString; //the message we will fill up and send
  std::string receiveString; //the message we will receive
	
  int senderIP;//the IP of the node we're receiving a msg from
  uint32_t senderID; //the ID of the node we're receiving a msg from
	
  uint32_t recNum;
	
  UDPSocket socket(MAINPORT);
  //ASSERT: connect socket to our main port
	
  Message sendMessageOBJ();
  //ASSERT: empty message object to send later
  
  Triple KClosest[K];
  int closestSize; //size of nodes after we fill with K closest
  //ASSERT: Declare a Triple array of size K to send to FINDNODE
  //        and FINDVALUE requests

  while(listening){
    recNum = socket.recvMessage(receiveString);
	  
    if(recNum > 0){
      Message receivedMessageOBJ(receiveString);
      senderIP = socket.getRemoteIP();
      senderID = receivedMessageOBJ.getNodeID();
      Triple sendTriple(senderIP, MAINPORT, senderID);

      uint32_t aKey;
      //ASSERT: to be extracted from each message.
			
      if(receivedMessageOBJ.getMSGType() == STORE){
	//ASSERT: A node wants to store
	aKey = receivedMessageOBJ.getID();
	keys.push_back(keyToStore);
	refresherVector.push_back(sendTriple);
      }
      else if(receivedMessageOBJ.getMSGType() == FINDNODE){
	//ASSERT: A node wants k closest nodes
	aKey = receivedMessageOBJ.getID();

	closestSize = RT.getKClosetNodes(aKey, KClosest);
	sendMessageOBJ.setKClos(KClosest, closestSize);
	
	sendMessageOBJ.setMsgType(KCLOSEST);
	sendMessageOBJ.setNodeId(ID);
	
	sendString = sendMessageOBJ.toString();
	socket.sendMessage(sendString, UIPORT, senderIP);

	refresherVector.push_back(sendTriple);
      }
      else if(receivedMessageOBJ.getMSGType() == FINDVALUE){
	//ASSERT: A node is trying to find a key
	aKey = receivedMessageOBJ.getID();
	      
	vector<int>::iterator it;
	it = find(keys.begin(), keys.end(), theKey);
	      
	if(it != keys.end()){
	  //ASSERT: we found the key
	  sendMessageOBJ.setMsgType(FVRESP);
	  sendMessageOBJ.setNodeId(ID);
	  sendString = sendMessageOBJ.toString();
	  socket.sendMessage(sendString, MAINPORT, senderIP);
	}
	else{
	  //ASSERT: we could not find in the key
	  closestSize = RT.getKClosetNodes(aKey, KClosest);
	  sendMessageOBJ.setKClos(KClosest, closestSize);
	  
	  sendMessageOBJ.setMsgType(KCLOSEST);
	  sendMessageOBJ.setNodeId(ID);
	  
	  sendString = sendMessageOBJ.toString();
	  socket.sendMessage(sendString, UIPORT, senderIP);
	}
	      
	refresherVector.push_back(sendTriple);
      }
    }
  }

  PingThread.join();
  UIThread.join();
  //ASSERT: join the threads after we have finished listening
}

//------------------------------Refresher and Update----------------------

//Refresher/ Update Table
//Possibly Variable Time
//L2  : port 6668
//      READS: PING_RESP, PING
//      SENDS: PING, PING_RESP
void Node::startRefresher()
{
  // Refreshing the whole table boolean
  bool refresh = false;
	
  // last time refresh was finished
  MsgTimer lastRefresh (PINGTIME, 0,0);
	
  // Current bucket we are checking.
  KBucket curKBucket;
	
  // indices used to access elements of the routing table.
  int i =j =0;
	
  // Creating the socket for the refresher
  UDPSocket socket(REFRESHERPORT);
	
  // Incoming Message as a string
  std::string incoming;
	
  // Incoming message as a Message
  Message msg;
	
  // Incoming IP
  uint32_t IP =0;
	
  while (!exit)
    {
      IP=0;
      // Check for new message
      if (socket.recvMessage(incoming) != -1) // if it returns 0 then no message was received
	{
	  IP=socket.getRemoteIP();
			
	  msg.parse(incoming);
			
	  switch(msg.getMsgType())
	    {
	    case PING: // We are receiving a ping request
	      Message pingr(PINGRESP, this->ID);
	      socket.sendMessage (pingr.toString(), IP, REFRESHERPORT);
	      break;
					
	    case PINGRESP:
	      //check timeouts & clear timeouts using IP.
	      // Is there a case where we could have pinged the same IP more than once and have more than one timeout corresponding
	      // We update the older one (the one at the beginning of the vector
					
	      bool found = false;
					
					// Check in timeouts for other threads & refresher
					for (int i =0, int j=0; (i<timeouts[PINGER_TIMEOUT] || j<timeouts[REFRESH_TIMEOUT]) && !found; ++i, ++j)
					{
						// Checking in other threads timeouts
						if(i<timeouts[PINGER_TIMEOUT] && timeouts[PINGER_TIMEOUT][i].getIP() == IP) // If we found a timeout with the same IP
						{
							// erase element in vector
							timeouts[PINGER_TIMEOUT][i].erase(timeouts[PINGER_TIMEOUT].begin()+i);
							found = true; // Update flag
							i--;
						}
						
						// Checking in timeouts for refresher
						if(!found && j<timeouts[REFRESH_TIMEOUT] && timeouts[REFRESH_TIMEOUT][j].getIP() == IP) // If we found a timeout with the same IP
						{
							// erase element in vector
							timeouts[REFRESH_TIMEOUT][j].erase(timeouts[REFRESH_TIMEOUT].begin()+j);
							found = true; // Update flag
							j--;
						}
						
		  /// Refreshing nodes in updateTable if possible
		  RT.updateTable(msg.getNodeID(), IP);
		}
					
					if(!found)
					{
						RT.updateTable(msg.getNodeID(),IP);
					}
					break;
					
	    default:
	      cout << "Unrecognized message received: "<< incoming<<endl;
	      break;
	    }
	}
		
		if(refresh) // We are currently refreshing the routingTable
		{
			// check if we can send more PINGs
			if(timeouts[REFRESH_TIMEOUT].size()<ALPHA)
			{
				// send more messages such that a max of alpha are sent.
				sendUpToAlphaPing(curKBucket, socket, i, j);
			}
			
	}
		
		// Check if we are currently refreshing and if it is time to refresh
		if(!refresh && lastRefresh.timedOut())
		{
			refresh = true; // start refreshing
			i=j=0; // reset indices
			
			// Retrieve the first KBucket
			curKBucket =RT[i];
			
			// Send the first alpha messages
			///TODO: check this again
			sendUpToAlphaPing(curKBucket, socket, i, j);
			
	}
		
		for (int i=0; refresherVector.size()>0 && i<ALPHA; ++i)
		{
			// try to update in table then ping if necessary
			if (!RT.updateTable(refresherVector[0].node, refresherVector[0].address))
			{
				// PING
				Message msg(PING, ID);
				socket.sendMessage(msg.toString(), refresherVector[0].address, REFRESHERPORT);
				
				// Add to the timeouts
				MsgTimer timer(RESPONDTIME_PING, refresherVector[0].node, refresherVector[0].address);
				timeouts[PINGER_TIMEOUT].push_back(timer);
				
			}
			refresherVector.erase(refresherVector.begin()); // Remove from the vector, the node was refreshed
		}
		
		{			
			// Check in timeouts for other threads & refresher
			for (int i =0, int j=0; (i<timeouts[PINGER_TIMEOUT] || j<timeouts[REFRESH_TIMEOUT]); ++i, ++j)
			{
				// Checking if anything timed out and remove it.
				if(timeouts[PINGER_TIMEOUT][i].timedOut())
				{
					timeouts[PINGER_TIMEOUT].erase(timeouts[PINGER_TIMEOUT].begin()+i);
					i--;
				}
				
				if (timeouts[REFRESH_TIMEOUT][j].timedOut())
				{
					timeouts[REFRESH_TIMEOUT].erase(timeouts[REFRESH_TIMEOUT].begin()+j);
					j--;
				}

			}
			
		}
		
	}
    }
	
  socket.close();
}

//---------------------------User Interface Related-----------------------

//Handles all UI
//Variable Time
//L1  : port 6667
//      READS: FIND_VALUE_UI, STORE_UI, KCLOS, FIND_VALUE_RESP
//      SENDS: FIND_VALUE, FIND_NODE, STORE
//			TO UI: FIND_VALUE_RESP_POSITIVE, FIND_VALUE_RESP_NEGATIVE, STORE_RESP
void startUIListener() {
	SnapShot snapSnot;
	MsgType curMsg;
	vector<Timeout> timeoutVector;
	
	std::string strUI;
	Message msgUI;
	uint32_t recvlenUI;
	
	UDPSocket socketUI(UIPORT);
	
	while (listening) {
		//Listening on UI socket
		recvlenUI = socketUI.recvMessage(strUI);
		if (recvlenUI > 0) {
			//Update the ip for the UI
			int ipUI = socketUI.getRemoteIP();
			msgUI.parse(strUI);
			
			if (msgUI.getMsgType() == FINDVALUE) {
				curMsg = msgUI;
				if (std::find(keys.begin(), keys.end(), curMsg.getID())
						!= keys.end()) {
					//ASSERT: we have the value, send confirm message
					Message sendMsg(FVRESPP);
					socketUI.sendMessage(sendMsg.toString(), ipUI, UIPORT);
				}
				else {
					//ASSERT: we did not find the value, lets check
					//        the rest of the network
					Triple kClos[K];
					int size = getKClosetNodes(curMsg.getID(), kClos);
					//ASSERT: kClos contains the K closest nodes that we
					//        know about it.
					snapSnot.addClosest(kClos, size);
					if (!snapShot.nextExist()) {
						//ASSERT: there is no k clos to check,
						//        send fail message to UI
						Message sendMsg(FVRESPN);
						socketUI.sendMessage(sendMsg.toString(), ipUI, UIPORT);
					}
					else {
						sendUpToAlphaKClos(SnapShot, socketUI);
					}
				}
			}else if (msgUI.getMsgType() == STORE_UI) {
				curMsg = msgUI;
				Triple kClos[K];
				int size = getKClosetNodes(curMsg.getID(), kClos);
				if (size == 0) {
					//ASSERT: We are the only node in the network, store
					//        inside ourselves
					keys.push_back(msgUI.);
				}
				snapSnot.addClosest(kClos, size);
				sendUpToAlphaKClos(SnapShot, socketUI);
			}
		}
	}
}else if (msgUI.getMsgType() == STORE_UI) {
	curMsg = msgUI;
	Triple kClos[K];
	int size = getKClosetNodes(curMsg.getID(), kClos);
	snapSnot.addClosest(kClos, size);
	sendUpToAlphaKClos(SnapShot, socketUI);
}
}
}
}

//PRE: the snapshot we are currently using, as well as the socket to 
//     send messages on.
//POST: sends up to ALPHA nodes FINDVALUE and then add them to
//      the timer queue.
void Node::sendUpToAlphaKClos(SnapShot & ss, UDPSocket & sock) {
  Message sendMsg(FINDVALUE);
  sendMsg.getKClos(kClos, size);
  for (int i = 0; (i < ALPHA) && (snapSnot.nextExist()); i++) {
    Triple nextNode;
    snapSnot.getNext(nextNode);
    socket.sendMessage(sendMsg.toString(), nextNode.address, MAINPORT);
    MsgTimer timer(RESPONDTIME, nextNode.node, nextNode.address);
    timeouts[UI_TIMEOUT].push_back(timer);
  }

}

///TODO: check again

void Node::sendUpToAlphaPing(KBucket &curKBucket, UDPSocket &socket, uint32_t & i, uint32_t & j)
{
  while (timeouts[REFRESH_TIMEOUT].size()<ALPHA)
    {
      if(j>=curKBucket.getNumTriples()) // Check if we have reached the end of the Kbucket
	{
		if(j>=curKBucket.getNumTriples()) // Check if we have reached the end of the Kbucket
		{
			i++; // Go to next KBucket
			curKBucket= RT[i];
			// Start at first element of the KBucket.
			j =0;
		}
		
		if(i>= NUMBITS) // If we did all the KBuckets, reset
		{
			i=j=0; // Reset indices
			
			// seet last refresh timepoint to Now
			lastRefresh.reset();
			refresh = false;
		}
		
      // get next element in curKBucket and increment j
      Triple curTriple = curKBucket[j++];
		
      // Send PING
      Message pingr(PING);
      socket.sendMessage (pingr.toString(), curTriple.address, REFRESHERPORT);
		
		// Updating timeouts
		MsgTimer timer (RESPONDTIME_PING, curTriple.node, curTriple.address);
		timeouts[REFRESH_TIMEOUT].push_back(timer);
		
	}
}


