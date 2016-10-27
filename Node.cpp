//Kadelima Node Class

#include "Node.h"
#include "constants.h"
#include "Message.hpp"
#include "UDPSocket.h"
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

void startRefresher() {


}


void startUIListener() {
	SnapShot snapSnot;
	MsgType curMsg;
  vector<Timeout> timeoutVector;

  std::string msgUI;
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
				snapSnot.addClosest(kClos, size);
				sendUpToAlphaKClos(SnapShot, socketUI);
			}
		}
	}
}

void Node::sendUpToAlphaKClos(SnapShot & ss, UDPSocket & sock) {
	Message sendMsg(FINDVALUE);
	sendMsg.getKClos(kClos, size);
	for (int i = 0; (i < ALPHA) && (snapSnot.nextExist()); i++) {
		Triple nextNode;
		snapSnot.getNext(nextNode);
		socket.sendMessage(sendMsg.toString(), nextNode.address, UDPPORT);
		MsgTimer timer(RESPONDTIME, nextNode.node, nextNode.address);
		timeous[UI_TIMEOUT].push_back(timer);
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


