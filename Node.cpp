//Kadelima Node Class
#include "UDPSocket.h"
#include "Node.h"
#include "constants.h"
#include "Message.hpp"
#include "UDPSocket.h"
#include "SnapShot.h"
#include <vector>
#include <algorithm>
#include "MsgTimer.h"


//Pre: id is a valid node id that is not yet taken
//Post: RT is initalized, ID = id, inNetwork = true
//      RT is empty since this node is starting a new network
Node::Node(uint32_t id) : RT(id){
  ID = id;
  inNetwork = true;
}

//Pre: id is a valid node id that is not yet taken, contactID and contactIP
//     belongs to a node already existing in the network
//Post: ID = id, contact exists within our routing table, as well as
//      other nodes our contact has told about us
//      inNetwork = true if FindNode on ourselves succeds, false otherwise
Node::Node(uint32_t id, uint32_t contactID, uint32_t contactIP) : RT(id) {
	ID = id;
	RT.updateTable(contactID, contactIP);
	inNetwork = true;
	//ASSERT: contact is in our routing table

	vector<Timeout> TV();

	/// TODO: send on the UIPORT for FINDNODE procedure.
  UDPSocket socket(PORT);

  socket.sendMessage(uint32_t ID, contactIP, PORT);
  TV.push_back(contactID);


	QueryQueue nodesToAsk(contactID, contactIP);

	//Note, we have joined the network as long as some other node added us
	//in its KBucket, until then, we are not a part of the network

	while ((!nodesToAsk.isAllQuereied() and RT.isEmpty())) {
		if (socket.recieved()) {
			Message msg = socket.getMessage();
			if (msg == KCLOS) {
				RT.updateTable(msg.getID(), msg.getIP(), PORT);
				TV.erase(/*iterator where msg.getID() lives*/);
				if (!msg.includes(ID)) {
					queryQueue.add(msg.getKClos());
				}
				socket.sendMessage(FIND_NODE ID
			}
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
     
  while(listening){
    recNum = socket.recvMessage(receiveString);
       
    if(recNum > 0){
      Message receivedMessageOBJ(receiveString);
      senderIP = socket.getRemoteIP();
      senderID = receivedMessageOBJ.getNodeID();

      Triple sendTriple(senderIP, MAINPORT, senderID);
      	
      if(receivedMessageOBJ.getMSGType() == STORE){
	uint32_t keyToStore = receivedMessageOBJ.getID();
	keys.push_back(keyToStore);
	   
	//add sender to refresh queue
      }
      else if(receivedMessageOBJ.getMSGType() == FINDNODE){
	   
	//access k closest to send over
	   

	sendString = sendMessageOBJ.toString();
	socket.sendMessage(sendString, UIPORT, senderIP);
      }
      else if(receivedMessageOBJ.getMSGType() == FINDVALUE){
	//ASSERT: A node is trying to find a key
	uint32_t theKey = receivedMessageOBJ.getID();

	vector<int>::iterator it;
	it = find(keys.begin(), keys.end(), theKey);
	
      	if(it != keys.end()){
	  //ASSERT: we found the key
	  sendMessageOBJ.setMsgType = FVRESP;
	  sendString = sendMessageOBJ.toString();
	  socket.sendMessage(sendString, MAINPORT, senderIP);
	}
	else{
	  //ASSERT: we could not find in the key
	  sendMessageOBJ.setMsgType = KCLOSEST;

	  
	  sendString = sendMessageOBJ.toString();
	  socket.sendMessage(sendString, UIPORT, senderIP);
	}

	//ASSERT: ADD SENDER TO REFRESHER
      }
    }
  }
 
  PingThread.join();
  UIThread.join();
  //ASSERT: join the threads after we have finished listening
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

	bool listening = true;

	while(listening) {
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
				}else{
					//ASSERT: we did not find the value, lets check
					//        the rest of the network
					Triple kClos[K];
					int size = getKClosetNodes(curMsg.getID(), kClos);
					//ASSERT: kClos contains the K closest nodes that we
					//        know about it.
					snapSnot.addClosest(kClos, size);
					Message sendMsg(FINDVALUE);
					sendMsg.getKClos(kClos, size);
					for (int i = 0; (i < ALPHA) && (snapSnot.nextExist()); i++) {
						Triple nextNode;
						snapSnot.getNext(nextNode);
						socketUI.sendMessage(sendMsg.toString(), nextNode.address, UDPPORT);
						
					}
				}
			}
				
		}
	}
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


