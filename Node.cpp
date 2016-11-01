//Kadelima Node Class

#include "Node.h"
#include <algorithm>
#include "JoinNetworkQueue.h"
#include "MsgTimer.h"
#include "string.h"
#include <thread>
#include <iostream>

//Node Construction, Node Listener,
//Refresher and Update, User Interface, Sending

//------------------------------Node Construction---------------------------

//Pre: id is a valid node id that is not yet taken
//Post: RT is initalized, ID = id, inNetwork = true
//      RT is empty since this node is starting a new network
Node::Node(uint32_t nodeID) : RT(nodeID){
  ID = nodeID;
  inNetwork = true;
}

bool Node::joined()
{
  return inNetwork;
}

//Pre: msg, queue, and timeOut were declared in the constructor below
//Post: the id of the node sending msg is removed from timeOut
//      if our id is in closest times, return true, false other wise

void Node::handleKClosMsg(Message & msg, vector<MsgTimer>& timeOut,
			  JoinNetworkQueue& queue, uint32_t IP) {
  bool found = false;
  int index = 0;

  // Get the IP and ID
  uint32_t senderID = msg.getNodeID();
  uint32_t senderIP = IP;

  if(senderID != ID){
    //ASSERT: where we don't want to add ourselves to the kBucket
    RT.updateTable(senderID, senderIP);
  }
  
  MsgTimer curTimer (RESPONDTIME_UI, senderID, senderIP);

  while (!found && (index < timeOut.size()))
    {
      curTimer = timeOut[index];

      if (curTimer.getNodeID() == senderID) // If it is the same ID
	{
	  found = true;
	  timeOut.erase(timeOut.begin() + index);
	}
      index++;

    } //the sender node is now removed from the time out vector

  // if (msg.includes(ID))// If this node knows us
  //   {
  //     inNetwork = true;
  //   }
  //else{
    // Continue adding in the nodes we have not asked yet
    Triple closestK[K];
    int size = msg.getKClos(closestK);
    
    for (int index = 0; (index < size); ++index){
			if(closestK[index].node != ID)
      	queue.add(closestK[index]);
    }
    //}
}

//Pre: timer is from the constructor
//Post: Removes elements from timer that have timed out
void Node::clearTimeOut(vector<MsgTimer>& timer)
{

  for (int index = 0; (index < timer.size()); index++)
    {
      if (timer[index].timedOut()) {
	timer.erase(timer.begin() + index);
      }
    }
}

//Pre: id is a valid node id that is not yet taken, contactID and contactIP
//     belongs to a node already existing in the network
//Post: ID = id, contact exists within our routing table, as well as
//      other nodes our contact has told about us
//      inNetwork = true if FindNode on ourselves succeds, false otherwise
Node::Node(uint32_t nodeID, uint32_t contactID, uint32_t contactIP) : RT(nodeID)
{
  ID = nodeID; // update this node's nodeID
  RT.updateTable(contactID, contactIP); // Insert contact node in routing table
  inNetwork = false; //at this point, no other node knows about us
  bool recvContact = false;
  
  // Create a vector for timeouts
  vector<MsgTimer> timeOut;

  // Create socket
  UDPSocket socket(UIPORT, "Add.log");

  // Create Contact triple and add it to the queue
  Triple contactTriple (contactIP, contactID, UIPORT);
  JoinNetworkQueue nodesToAsk(contactTriple);

  while (!recvContact || (timeOut.size() > 0 && !inNetwork)
	 || (nodesToAsk.hasNext() && !inNetwork))
    {
      // Try to receive a message
      string response;
      int msgSize = socket.recvMessage(response);

      // If there is a message
      if (msgSize != -1)
	{
	  uint32_t IP = socket.getRemoteIP();
	  printf("%s\n from %u\n", response.c_str(), IP);
	  
	  Message msg(response);

	  if (msg.getMsgType() == KCLOSEST)
	    {
	      if (msg.getNodeID() == contactID){
		recvContact = true;
		
		//ASSERT: contact node responds, but no one else in the network
		Triple closestK[K];
		int size = msg.getKClos(closestK);

		if(size == 0){
		  //ASSERT: nothing in the KClos, but recieved response so
		  //        we are in the network. Just only 1 node in the
		  //        network.
		  inNetwork = true;
		}else{
		  handleKClosMsg(msg, timeOut, nodesToAsk, IP);
		}
	      }else{
		handleKClosMsg(msg, timeOut, nodesToAsk, IP);
	      }
	    } //Done Dealing with a received message
	}
      
      if(nodesToAsk.hasNext()){
	// Get next Triple to ask
	Triple nextToAsk;
	nodesToAsk.getNext(nextToAsk);

	// Send a FINDNODE message to the next node.
	Message toSend(FINDNODE, ID, ID);
	socket.sendMessage(toSend.toString(), nextToAsk.address, UIPORT);
	printf("%s to %u\n", toSend.toString().c_str(), nextToAsk.address);

	// Add a timeout
	MsgTimer timer(RESPONDTIME_UI, nextToAsk.node, nextToAsk.address);
	timeOut.push_back(timer);
      }

      clearTimeOut(timeOut);

      if((timeOut.size() == 0) && !recvContact){
	//ASSERT: contact never responded
	recvContact = true;
	printf("Contact Node Timed out.\n");
      }
    }
  if(recvContact){
    inNetwork = true;
  }
	cout << "hi ";
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

  thread PingThread = thread(&Node::startRefresher, this);
  thread UIThread = thread(&Node::startUIListener, this);
  //ASSERT: Create the two threads for handling Pings and
  //        for handling UIs

  Message sendMessageOBJ(NONE, ID);
  //ASSERT: empty message object to send later

  std::string sendString; //the message we will fill up and send
  std::string receiveString; //the message we will receive

  uint32_t senderIP;//the IP of the node we're receiving a msg from
  uint32_t senderID; //the ID of the node we're receiving a msg from

  int32_t recNum = -1;

  UDPSocket socket(MAINPORT, "main.log");
  //ASSERT: connect socket to our main port

  Triple KClosest[K];
  int closestSize; //size of nodes after we fill with K closest
  //ASSERT: Declare a Triple array of size K to send to FINDNODE
  //        and FINDVALUE requests

  while(!exit)
    {
      //printf("Main Listener\n");
      recNum = socket.recvMessage(receiveString);

      if(recNum > 0)
	{
	  Message receivedMessageOBJ(receiveString);
	  senderIP = socket.getRemoteIP();
	  senderID = receivedMessageOBJ.getNodeID();
	  Triple sendTriple (senderIP, senderID, MAINPORT);

	  uint32_t aKey;
	  //ASSERT: to be extracted from each message.

	  switch (receivedMessageOBJ.getMsgType())
	    {
	    case STORE:
	      {
		//ASSERT: A node wants to store
		aKey = receivedMessageOBJ.getID();
		if (find(keys.begin(), keys.end(), aKey) == keys.end()){
		  keys.push_back(aKey);
		  refresherVector.push_back(sendTriple);
		}
	      }
	      break;

	    case FINDNODE:
	      {
		//ASSERT: A node wants k closest nodes
		aKey = receivedMessageOBJ.getID();

		closestSize = RT.getKClosestNodes(aKey, KClosest);
		sendMessageOBJ.setKClos(KClosest, closestSize);

		sendMessageOBJ.setType(KCLOSEST);
		sendMessageOBJ.setNodeID(ID);

		sendString = sendMessageOBJ.toString();
		socket.sendMessage(sendString, senderIP, UIPORT);

		refresherVector.push_back(sendTriple);
	      }
	      break;

	    case FINDVALUE:
	      {
		//ASSERT: A node is trying to find a key
		aKey = receivedMessageOBJ.getID();

		if (find(keys.begin(), keys.end(), aKey) != keys.end())
		  {
		    //ASSERT: we found the key
		    sendMessageOBJ.setType(FVRESP);
		    sendMessageOBJ.setNodeID(ID);
		    sendString = sendMessageOBJ.toString();
		    socket.sendMessage(sendString, senderIP, MAINPORT);
		  }
		else {
		  //ASSERT: we could not find in the key
		  closestSize = RT.getKClosestNodes(aKey, KClosest);
		  sendMessageOBJ.setKClos(KClosest, closestSize);

		  sendMessageOBJ.setType(KCLOSEST);
		  sendMessageOBJ.setNodeID(ID);

		  sendString = sendMessageOBJ.toString();
		  socket.sendMessage(sendString, senderIP, UIPORT);
		}

		refresherVector.push_back(sendTriple);
	      }
	      break;

	    default:
	      break;
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
  uint32_t i =0, j =0;

  // Creating the socket for the refresher
  UDPSocket socket(REFRESHERPORT, "refresh.log");

  // Incoming Message as a string
  std::string incoming;

  // Incoming message as a Message
  Message msg(NONE, ID);

  // Incoming IP
  uint32_t IP =0;

  while (!exit)
    {
      //printf("Refresher Listener\n");
      IP=0;
      // Check for new message
      if (socket.recvMessage(incoming) != -1) // if it returns 0 then no message was received
	{
	  IP=socket.getRemoteIP();
	  printf("%s from %u\n", incoming.c_str(), IP);
	  msg.parse(incoming);

	  switch(msg.getMsgType())
	    {
	    case PING: // We are receiving a ping request
	      {
		Message pingr(PINGRESP, this->ID);
		socket.sendMessage (pingr.toString(), IP, REFRESHERPORT);
	      }
	      break;

	    case PINGRESP:
	      {
		//check timeouts & clear timeouts using IP.
		// Is there a case where we could have pinged the same IP more than once and have more than one timeout corresponding
		// We update the older one (the one at the beginning of the vector

		bool found = false;

		// Check in timeouts for other threads & refresher
		int m, n;
		for ( m =0, n=0; ((m<timeouts[PINGER_TIMEOUT].size() || n<timeouts[REFRESH_TIMEOUT].size()) && !found); ++m, ++n)
		  {
		    // Checking in other threads timeouts
		    if(m<timeouts[PINGER_TIMEOUT].size() && timeouts[PINGER_TIMEOUT][m].getNodeIP() == IP) // If we found a timeout with the same IP
		      {
			// erase element in vector
			timeouts[PINGER_TIMEOUT].erase(timeouts[PINGER_TIMEOUT].begin()+m);
			found = true; // Update flag
			m--;
		      }

		    // Checking in timeouts for refresher
		    if(!found && n<timeouts[REFRESH_TIMEOUT].size() && timeouts[REFRESH_TIMEOUT][n].getNodeIP() == IP) // If we found a timeout with the same IP
		      {
			// erase element in vector
			timeouts[REFRESH_TIMEOUT].erase(timeouts[REFRESH_TIMEOUT].begin()+n);
			found = true; // Update flag
			n--;
		      }

		    /// Refreshing nodes in updateTable if possible
		    if( msg.getNodeID() != ID)
		      RT.updateTable(msg.getNodeID(), IP);
		  }

		if(!found)
		  {
		    if( msg.getNodeID() != ID)
		      RT.updateTable(msg.getNodeID(),IP);
		  }
	      }
	      break;

	    default:
	      printf("Unrecognized message received: %s\n", incoming.c_str());
	      break;
	    }
	}

      if(refresh) // We are currently refreshing the routingTable
	{
	  // check if we can send more PINGs
	  if(timeouts[REFRESH_TIMEOUT].size()<ALPHA)
	    {
	      // send more messages such that a max of alpha are sent.
	      sendUpToAlphaPing(curKBucket, socket, i, j, lastRefresh, refresh);
	    }

	}

      // Check if we are currently refreshing and if it is time to refresh
      if(!refresh && lastRefresh.timedOut())
	{
	  refresh = true; // start refreshing
	  i=j=0; // reset indices

	  // Retrieve the first KBucket
	  RT[i].copyKBucket(curKBucket);

	  // Send the first alpha messages
	  ///TODO: check this again
	  sendUpToAlphaPing(curKBucket, socket, i, j, lastRefresh, refresh);

	}

      for (int i=0; refresherVector.size()>0 && i<ALPHA; ++i)
	{
	  // try to update in table then ping if necessary
	  if ((refresherVector[0].node != ID)&&!RT.updateTable(refresherVector[0].node, refresherVector[0].address))
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
	int i=0, j=0;
	// Check in timeouts for other threads & refresher
	for ( i =0,  j=0; (i<timeouts[PINGER_TIMEOUT].size() || j<timeouts[REFRESH_TIMEOUT].size()); ++i, ++j)
	  {
	    // Checking if anything timed out and remove it.
	    if(timeouts[PINGER_TIMEOUT][i].timedOut())
	      {
		RT.deleteNode(timeouts[PINGER_TIMEOUT][i].getNodeID());
		timeouts[PINGER_TIMEOUT].erase(timeouts[PINGER_TIMEOUT].begin()+i);
		i--;
	      }

	    if (timeouts[REFRESH_TIMEOUT][j].timedOut())
	      {
		RT.deleteNode(timeouts[PINGER_TIMEOUT][i].getNodeID());
		timeouts[REFRESH_TIMEOUT].erase(timeouts[REFRESH_TIMEOUT].begin()+j);
		j--;
	      }

	  }

      }

    }

}

//---------------------------User Interface Related-----------------------

//Handles all UI
//Variable Time
//L1  : port 6667
//      READS: FIND_VALUE_UI, STORE_UI, KCLOS, FIND_VALUE_RESP
//      SENDS: FIND_VALUE, FIND_NODE, STORE
//			TO UI: FIND_VALUE_RESP_POSITIVE, FIND_VALUE_RESP_NEGATIVE, STORE_RESP
void Node::startUIListener() {
  SnapShot snapShot;
  Message curMsg(NONE, ID);

  std::string strUI;
  Message recvMsg(NONE, ID);
  int recvlenUI;


  UDPSocket socketUI(UIPORT, "UI.log");

  while (!exit)
    {
      //printf("UI Listener\n");
      //Listening on UI socket
      recvlenUI = socketUI.recvMessage(strUI);
      if (recvlenUI > 0)
	{
	  
	  //Update the ip for the UI
	  int ipUI = socketUI.getRemoteIP();

	  printf("%s from %u\n", strUI.c_str(), ipUI);
	  
	  // Parsing incoming string
	  recvMsg.parse(strUI);

	  switch (recvMsg.getMsgType())
	    {
	    case FINDVALUE:
	      {
		MsgType type = recvMsg.getMsgType();
		curMsg.setType(type);
		curMsg.setID(recvMsg.getID());
		if (std::find(keys.begin(), keys.end(), recvMsg.getID())!= keys.end())
		  {
		    //ASSERT: we have the value, send confirm message
		    Message sendMsg(FVRESPP, ID);
		    socketUI.sendMessage(sendMsg.toString(), ipUI, UIPORT);
		  }
		else
		  {
		    //ASSERT: we did not find the value, let's check
		    //        the rest of the network
		    Triple kClos[K];
		    Message sendMsg(KCLOSEST, ID);
		    int size = RT.getKClosestNodes(sendMsg.getID(), kClos);
		    //ASSERT: kClos contains the K closest nodes that we
		    //        know about.
		    snapShot.addClosest(kClos, size);
		    if (!snapShot.nextExist())
		      {
			//ASSERT: there is no k clos to check,
			//        send fail message to UI
			Message sendMsg(FVRESPN, ID);
			socketUI.sendMessage(sendMsg.toString(), ipUI, UIPORT);
		      }
		    else
		      {
			sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDVALUE);
		      }
		  }
	      }
	      break;
	    case STORE:
	      {
		// Set the current message to the received request from the UI
		curMsg.setType(recvMsg.getMsgType());
		curMsg.setID(recvMsg.getID());
		// Get KClosest nodes to the key
		Triple kClos[K];
		int size = RT.getKClosestNodes(curMsg.getID(), kClos);

		// If there are no close nodes, there are no nodes.
		if (size == 0)
		  {
		    //ASSERT: special case where we are the only node in network,
		    //        so we store the key
		    keys.push_back(recvMsg.getID());

		    // Respond to UI
		    Message sendMsg(STORERESP, ID);
		    socketUI.sendMessage(sendMsg.toString(), ipUI, UIPORT);
		  }
		else // Try to find the KClosest to the Key
		  {
		    snapShot.addClosest(kClos, size);
		    sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDNODE);
		  }
	      }
	      break;

	    case KCLOSEST:
	      {

		//ASSERT: this is a response from a node.
		Triple kClos[K];
		removeFromUITimeout(recvMsg.getNodeID());
		int size = recvMsg.getKClos(kClos);
		snapShot.addClosest(kClos, size);

		switch (curMsg.getMsgType())
		  {
		  case STORE:
		    {
			//Add to refresh vector
			Triple refresh;
		      refresh.address = socketUI.getRemoteIP();
		      refresh.node = recvMsg.getNodeID();
		      refresherVector.push_back(refresh);


		      if (!snapShot.nextExist()) {
			//ASSERT: we have found the K closest, send store messages
			Message sendMsg(STORE, ID);
			for (int i = 0; i < snapShot.getSize(); i++) {
			  socketUI.sendMessage(sendMsg.toString(),
					       snapShot.getElementIP(i), MAINPORT);
			}
		      }else{
			//ASSERT: we are not done searching for kClos
			sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDNODE);
		      }
		    }
		    break;
		  case FINDVALUE:
		  {
		    if (!snapShot.nextExist()) {
		      //ASSERT: we have found the K closest and no value,
		      //        send UI that wouldn't couldnt find it.
		      Message sendMsg(FVRESPN, ID);
		      socketUI.sendMessage(sendMsg.toString(), ipUI, UIPORT);
		    }
		    else {
		      //ASSERT: we are not done searching for kClos
		      sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDVALUE);
		    }

		  }
		break;

	      default:
		break;
	      }
	    }
	  break;
	case FVRESP:
	  {
	    //ASSERT: we have the value, send confirm message
	    removeFromUITimeout(recvMsg.getNodeID());

	    //Add to refresh vector
	    Triple refresh;
	    refresh.address = socketUI.getRemoteIP();
	    refresh.node = recvMsg.getNodeID();
	    refresherVector.push_back(refresh);

	    Message sendMsg(FVRESPP, ID);
	    socketUI.sendMessage(sendMsg.toString(), ipUI, UIPORT);
	  }
	  break;
	case FINDNODE:
	  {
	    //ASSERT: A node wants k closest nodes
	    uint32_t key = recvMsg.getID();
	    Triple kClos [K];
	    int32_t closestSize = RT.getKClosestNodes(key, kClos);

	    Message sendMsg(KCLOSEST, ID);
	    sendMsg.setKClos(kClos, closestSize);

	    socketUI.sendMessage(sendMsg.toString(), ipUI, UIPORT);

	    Triple sendTriple;
	    sendTriple.address = ipUI;
	    sendTriple.port = UIPORT;
	    sendTriple.node = recvMsg.getNodeID();
	    refresherVector.push_back(sendTriple);
	  }
	  break;
	default:
	  break;
	}

      // TIMEOUTS
      for (int i = 0; i < timeouts[UI_TIMEOUT].size(); i++)
	{
	  if (timeouts[UI_TIMEOUT][i].timedOut()) {
	    //ASSERT: this node timed out, remove from queue
	    RT.deleteNode(timeouts[UI_TIMEOUT][i].getNodeID());
	    timeouts[UI_TIMEOUT].erase(timeouts[UI_TIMEOUT].begin()+i);
	    i--;

	    //Now we need to continue depending on what we are on
	    if (curMsg.getMsgType() == STORE)
	      {
		if (!snapShot.nextExist()) {
		  //ASSERT: we have found the K closest, send store messages
		  Message sendMsg(STORE, ID);
		  for (int i = 0; i < snapShot.getSize(); i++)
		    {
		      socketUI.sendMessage(sendMsg.toString(), snapShot.getElementIP(i), MAINPORT);
		    }
		}
		else {
		  //ASSERT: we are not done searching for kClos
		  sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDNODE);
		}
	      }else if(curMsg.getMsgType() == FINDVALUE)
	      {
		if (!snapShot.nextExist())
		  {
		    //ASSERT: we have found the K closest and no value,
		    //        send UI that wouldn't couldnt find it.
		    Message sendMsg(FVRESPN);
		    socketUI.sendMessage(sendMsg.toString(), ipUI, UIPORT);
		  }
		else
		  {
		    //ASSERT: we are not done searching for kClos
		    sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDVALUE);
		  }
	      }
	  }
	}
    }
}
}
//PRE: a node ID we want to remove from the list
//POST: finds the node ID in the list and removes from timeout,
//      if it exist in the list.
void Node::removeFromUITimeout(uint32_t ID)
{
  bool foundNode = false;
  for (int i = 0; i < timeouts[UI_TIMEOUT].size() && !foundNode; i++)
    {
      if (timeouts[UI_TIMEOUT][i].getNodeID() == ID)
	{
	  timeouts[UI_TIMEOUT].erase(timeouts[UI_TIMEOUT].begin()+i);
	  foundNode = true;
	  i--;
	}
    }
}

//PRE: the snapshot we are currently using, as well as the socket to
//     send messages on.
//POST: sends up to ALPHA nodes FINDVALUE and then add them to
//      the timer queue.
void Node::sendUpToAlphaKClos(SnapShot & ss, UDPSocket & sock, uint32_t msgID, MsgType mType)
{

  Message sendMsg(mType, ID, msgID);
  printf("sendMsg: %s\n", sendMsg.toString().c_str());
  
  for (int i = 0; (i < ALPHA) && (ss.nextExist()); i++)
    {
      Triple nextNode;
      ss.getNext(nextNode);

      if (nextNode.node != ID){
	// Sending Message
	sock.sendMessage(sendMsg.toString(), nextNode.address, MAINPORT);

	// Adding a timeout
	MsgTimer timer(RESPONDTIME_UI, nextNode.node, nextNode.address);
	timeouts[UI_TIMEOUT].push_back(timer);
      }
    }

}

void Node::sendUpToAlphaPing(KBucket &curKBucket, UDPSocket &socket, uint32_t & i, uint32_t & j, MsgTimer & lastRefresh, bool & refresh)
{
  while (timeouts[REFRESH_TIMEOUT].size()<ALPHA)
    {
      if(j>=curKBucket.getNumTriples()) // Check if we have reached the end of the Kbucket
	{
	  if(j>=curKBucket.getNumTriples()) // Check if we have reached the end of the Kbucket
	    {
	      i++; // Go to next KBucket
	      RT[i].copyKBucket(curKBucket);
	      // Start at first element of the KBucket.
	      j =0;
	    }

	  if(i>= NUMBITS) // If we did all the KBuckets, reset
	    {
	      i=j=0; // Reset indices

	      // seet last refresh timepoint to Now
	      lastRefresh.resetTimer();
	      refresh = false;
	    }

	  // get next element in curKBucket and increment j
	  Triple curTriple = curKBucket[j++];

	  if( curTriple.node != ID)
	    {
	      // Send PING
	      Message pingr(PING, ID);
	      socket.sendMessage (pingr.toString(), curTriple.address, REFRESHERPORT);

	      // Updating timeouts
	      MsgTimer timer (RESPONDTIME_PING, curTriple.node, curTriple.address);
	      timeouts[REFRESH_TIMEOUT].push_back(timer);
	    }

	}
    }

}
