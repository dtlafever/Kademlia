//Kadelima Node Class

#include "Node.h"
#include "constants.h"
#include "Message.hpp"
#include "SnapShot.h"
#include <vector>
#include <algorithm>
#include "JoinNetworkQueue.h"
#include "MsgTimer.h"

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
bool Node::handleKClosMsg(Message msg, vector<MsgTimer>& timeOut,
			  JoinNewtorkQueue& queue) {
  bool isMyIDInMsg = false;
  uint32_t senderID = msg.ID();
  uint32_t senderIP = msg.IP();
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
    if (msg.includes(ID)) {
      isMyIDInMsg = true;
    }
    else {
      Triple closestK[K];
      int size = msg.getKClos(closestK);
      for (int index = 0; (index < size); index) {
	queue.add(closestK[index]);
      }
    }
  }
  return (isMyIDInMsg);
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
  socket.sendMessage(FIND_NODE ID, contactIP, UDPPORT);
  timeOut.push_back(MsgTimer(RESPONSETIME, contactID, contactIP));
  Triple contactTriple(contactID, contactIP, UDPPORT);
  JoinNetworkQueue nodesToAsk(contactID, contactIP);
  while (nodesToAsk.hasNext() and !RT.isFull()) {
    if (socket.recieved()) {
      Message msg = socket.getMessage();
      if (msg == KCLOSEST) {
	bool myIdInMsg = handleKClosMsg(msg, timeOut, nodesToAsk);
	if (!inNetwork) { //no need to check if we're already in network
	  inNetwork = myIdInMsg;
	}
      }
      Triple nextToAsk = nodesToAsk.getNext();
      socket.sendMessage(FIND_NODE ID, nextToAsk.address, nextToAsk.port);
				 
    }
    /*
    //LOOP
    //TODO: stop when our KBuckets are full or when our
    //      query has all elements are queried
    while(inNetwork){
    if (socket.recieved()){
    Message msg = socket.getMessage();
    if (msg == KCLOS){
    RT.updateTable(msg.getID(), msg.getIP(), PORT);
    if(RT.full()){ //TODO: routing table is full function
    //ASSERT: the routing table is full,
    //        stop trying to add to the network
    break;
    }
    TV.eraseElement(msg.getID()); //TODO:remove element
    //     of the msg sender ID
    if (!msg.includes(ID)){
    //ASSERT: these kClos should be added to the queue
    queue.add(msg.getKClos()); //TODO:
    }
    if(queue.isNext()){
    socket.sendMessage(FIND_NODE ID, queue.getNext()); //TODO
    }else{
    //ASSERT: nothing left to check
    break;
    }
    }
    }

    for (int i=0; i < TV.size(); i++){
    if (TV[i].timedOut()){
    TV.erase(i);
    i--;
    socket.sendMessage(FIND_NODE ID, queue.getNext()); //TODO
    }
    }
    if (TV.size() == 0){
    //ASSERT: We have not joined the network
    inNetwork = false;
    }
    }
    */

  }
}

void Node::startListener(){
  
  //NOTES: Timeout?
  //       When we send a message, make sure we've got
  //       6666 included so people know to respond to the
  //       right one.

 
  //Handles messages from other Nodes.
  //Everything is constant time
  //MAIN: port 6666
  //      READS: PING, STORE, FIND_NODE, FIND_VALUE
  //      SENDS: PING_RESP, K_CLOS, FIND_VALUE_RESP_TRUE

  std::string sendString; //the message we will fill up and send
  std::string receiveString; //the message we will receive
  
  int senderIP; //the IP of the node we're receiving a msg from

  uint32_t recNum; 

  try{
    UDPSocket socket(MAINPORT);
    //ASSERT: connect socket to our main port

    thread PingThread = thread(startRefresher);
    thread UIThread = thread(startUIListener);
    //ASSERT: Create the two threads for handling Pings and
    //        for handling UIs

    Message sendMessageOBJ();
    //ASSERT: empty message object to send later
     
    while(listening){
      //listening on the main socket
       
      recNum = socket.recvMessage(receiveString);

      if(recNum > 0){
	Message receivedMessageOBJ(receiveString);
	senderIP = socket.getRemoteIP();
	 
	if(receivedMessageOBJ.getMSGType() == PING){
	  sendMessageOBJ.setMsgType = PINGRESP;
	  sendString = sendMessageOBJ.toString();
	  socket.sendMessage(sendString, MAINPORT, senderIP); 
	   
	  //add sender to refresh queue
	}
	else if(receivedMessageOBJ.getMSGType() == STORE){
	  uint32_t keyToStore = receivedMessageOBJ.getID();
	  //push key to our key list
	  //add sender to refresh queue
	}
	else if(receivedMessageOBJ.getMSGType() == KCLOS){
	   
	  //access k closest to send over
	   
	  //give back kclos
	  //add sender to refresh queue
	}
	else if(receivedMessageOBJ.getMSGType() == FIND_VALUE){
	  uint32_t theKey = receivedMessageOBJ.getID();
	  //FIRST EXTRACT THE VALUE
	  if(KeyFound){
	    sendMessageOBJ.setMsgType = FVRESP;
	    sendString = sendMessageOBJ.toString();
	    socket.sendMessage(sendString, MAINPORT, senderIP);
	  }
	  else{
	    sendMessageOBJ.setMsgType = KCLOSEST;
	    //add k closest nodes to message? 
	    sendString = sendMessageOBJ.toString();
	    socket.sendMessage(sendString, MAINPORT, senderIP);
	  }

	  //add sender to refresh queue
	}

      }
       

    }
     
     
  }
  catch (SocketException & e) {
    printf("ERROR: %s\n", ((char *)(e.description().c_str())));
  }
}

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
					Message pingr(PINGRESP);
					socket.sendMessage (pingr.toString(), IP, REFRESHERPORT);
					break;
					
				case PINGRESP:
					//check timeouts & clear timeouts using IP.
					// Is there a case where we could have pinged the same IP more than once and have more than one timeout corresponding
					// We update the older one (the one at the beginning of the vector
					
					bool found = false;
					
					/// TODO: check to see order of checking
					
					// Check in timeouts for other threads & refresher
					for (int i =0; i<timeouts[PINGER_TIMEOUT] && !found; ++i)
					{
						// Checking in other threads timeouts
						if(timeouts[PINGER_TIMEOUT][i].getIP() == IP) // If we found a timeout with the same IP
						{
							// erase element in vector
							timeouts[PINGER_TIMEOUT][i].erase(timeouts[PINGER_TIMEOUT].begin()+i);
							found = true; // Update flag
							
							///TODO: Do something about refreshing the table?
						}
						
						// Checking in timeouts for refresher
						if(timeouts[REFRESH_TIMEOUT][i].getIP() == IP) // If we found a timeout with the same IP
						{
							// erase element in vector
							timeouts[REFRESH_TIMEOUT][i].erase(timeouts[REFRESH_TIMEOUT].begin()+i);
							found = true; // Update flag
						}
					}
					
					if(!found)
						cout << "Error PINGRESP does not correspond to any PING request"<<endl;
					break;
					
				default:
					cout << "Unrecognized message received: "<< incoming<<endl;
					break;
			}
		}
		
		if(refresh) // We are currently refreshing the routingTable
		{
			//check if we can send more PINGs
			if(timeouts[REFRESH_TIMEOUT].size()<ALPHA)
			{
				//					send more messages such that a max of alpha are sent.
				sendUpToAlphaPing(curKBucket, socket);
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
			sendUpToAlphaPing(curKBucket, socket);
			
		}
		
		///TODO: check the refresh queue.
	}
	
	socket.close();
}

///TODO: check again

void Node::sendUpToAlphaPing(KBucket &curKBucket, UDPSocket &socket)
{
	while (timeouts[REFRESH_TIMEOUT].size()<ALPHA)
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
		///TODO: Update with respond time for PING
		MsgTimer timer (RESPONDTIME_PING, curTriple.node, curTriple.address);
		timeouts[REFRESH_TIMEOUT].push_back(timer);
	}
	
}

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

  std::string msgUI;
  uint32_t recvlenUI;

  UDPSocket socketUI(UIPORT);

  bool listening = true;

  while(listening) {
    //Listening on UI socket
    recvlenUI = socketUI.recvMessage(msgUI);
    if (recvlenUI > 0) {
      //Update the ip for the UI
      ipUI = socketUI.getRemoteIP();

      Message msg(msgUI);
			
      if (msg.getMsgType() == FINDVALUE) {
	curMsg = msg.getMsgType();
	if (std::find(keys.begin(), keys.end(), curMsg.getID())
	    != keys.end()) {
	  //ASSERT: we have the value
	  Message returnMsg(FVRESPP);
	  socketUI.sendMessage(returnMesg);
	}
      }
    }
  }
}


