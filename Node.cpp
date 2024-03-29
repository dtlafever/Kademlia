//Kadelima Node Class

#include "Node.h"
#include <algorithm>
#include "JoinNetworkQueue.h"
#include "MsgTimer.h"
#include "string.h"
#include <thread>
#include <iostream>
#include <cmath>
#include <cstring>

//Node Construction, Node Listener,
//Refresher and Update, User Interface, Sending

//------------------------------Node Construction---------------------------

//Pre: id is a valid node id that is not yet taken
//Post: RT is initalized, ID = id, inNetwork = true
//      RT is empty since this node is starting a new network
Node::Node(uint32_t nodeID) : RT(nodeID){
  if(nodeID < pow(2, NUMBITS))
    {
      ID = nodeID;
      inNetwork = true;
      memset(refreshCounters, 0, sizeof(refreshCounters));
    }
  else {
    printf("Invalid ID : %u \n", nodeID);
    inNetwork = false;
  }
}

// This function returns true if the node has joined or created a network.
bool Node::joined()
{
  return inNetwork;
}

//Pre: msg, queue, and time were declared in the constructor below
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
		
      if (curTimer.getNodeIP() == senderIP) // If it is the same ID
	{
	  found = true;
	  timeOut.erase(timeOut.begin() + index);
	}
      index++;
		
    } //the sender node is now removed from the time out vector
	
  Triple closestK[K];
  int size = msg.getKClos(closestK);
	
  for (int index = 0; (index < size); ++index){
    if(closestK[index].node != ID)
      queue.add(closestK[index]);
  }

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
  if(contactID >= pow(2, NUMBITS)) // If ID provided is valid
    {
      printf("Invalid ID : %u \n", contactID);
      inNetwork = false;
    }
  else
    {
      ID = nodeID; // update this node's nodeID
      RT.updateTable(contactID, contactIP); // Insert contact node in routing table
      inNetwork = false; //at this point, no other node knows about us
      bool recvContact = false;
      memset(refreshCounters, 0, sizeof(refreshCounters));
		
      // Create a vector for timeouts
      vector<MsgTimer> timeOut;
		
      // Create socket
      UDPSocket socket(UIPORT, "Add.log");
		
      // Create Contact triple and add it to the queue
      Triple contactTriple (contactIP, contactID, UIPORT);
      JoinNetworkQueue nodesToAsk(contactTriple);
		
      while ((timeOut.size() > 0 )
	     || (nodesToAsk.hasNext() ))
	{
	  // Try to receive a message
	  string response;
	  int msgSize = socket.recvMessage(response);
				
	  // If there is a message
	  if (msgSize != -1)
	    {
	      uint32_t IP = socket.getRemoteIP();
	      printf("%s\n from %s\n", response.c_str(), IP_toString(IP).c_str());
				
	      Message msg(response);
				
	      if (msg.getMsgType() == KCLOSEST)
		{
		  if (msg.getNodeID() == contactID)
		    {
		      recvContact = true;
						
		      //ASSERT: contact node responds, but no one else in the network
		      Triple closestK[K];
		      int size = msg.getKClos(closestK);
						
		      if(size == 0){
			//ASSERT: nothing in the KClos, but recieved response so
			//        we are in the network. Just only 1 node in the
			//        network.
			inNetwork = true;
		      }
		    }
						
		  handleKClosMsg(msg, timeOut, nodesToAsk, IP);
						
		} //Done Dealing with a received message
	    }
				
	  if(nodesToAsk.hasNext()){
	    // Get next Triple to ask
	    Triple nextToAsk;
	    nodesToAsk.getNext(nextToAsk);
				
	    // Send a FINDNODE message to the next node.
	    Message toSend(FINDNODE, ID, ID);
	    socket.sendMessage(toSend.toString(), nextToAsk.address, UIPORT);
	    printf("%s to %s\n", toSend.toString().c_str(), IP_toString(nextToAsk.address).c_str());
				
	    // Add a timeout
	    MsgTimer timer(RESPONDTIME_UI, nextToAsk.node, nextToAsk.address);
	    timeOut.push_back(timer);
	  }
				
	  clearTimeOut(timeOut);
	  if(recvContact){
	    inNetwork = true;
	  }
			
	  if((timeOut.size() == 0) && !recvContact){
	    //ASSERT: contact never responded
	    //	recvContact = true;
	    printf("Contact Node Timed out.\n");
	  }
	}
      // End of Id check else
    }
	
}

//--------------------------------Node Listener----------------------------

//Functions used to breka up the listener

//Store a given key into our keys
void Node::mainStore(uint32_t aKey, Triple & sendTriple){
	
  if (find(keys.begin(), keys.end(), aKey) == keys.end()){
    mLock.lock();
    keys.push_back(aKey);
    cout << "Keys" << std::endl;
    for (int i=0; i < keys.size();i++){
      cout << keys[i] << std::endl;
    }
    refresherVector.push_back(make_pair(sendTriple, Triple()));
    mLock.unlock();
  }
}

//get our KClosest and send it off to the sender
void Node::mainFindNode(uint32_t aKey, Triple & sendTriple, 
			uint32_t senderIP, UDPSocket & socket){
  Triple KClosest[K];
  int closestSize = RT.getKClosestNodes(aKey, KClosest);
  Message sendMessageOBJ(NONE, ID);
	
  sendMessageOBJ.setKClos(KClosest, closestSize);
  sendMessageOBJ.setType(KCLOSEST);
  sendMessageOBJ.setNodeID(ID);
	
  socket.sendMessage(sendMessageOBJ.toString(), senderIP, UIPORT);
  mLock.lock();
  refresherVector.push_back(make_pair(sendTriple, Triple()));
  mLock.unlock();
}

//return FVRESP if we have the key, oterhwise send Kclosest
void Node::mainFindValue(uint32_t aKey, Triple & sendTriple, 
			 uint32_t senderIP, UDPSocket & socket){
  Triple KClosest[K];
  int closestSize = RT.getKClosestNodes(aKey, KClosest);
  Message sendMessageOBJ(NONE, ID);
	
  if (find(keys.begin(), keys.end(), aKey) != keys.end())
    {
      //ASSERT: we found the key
      sendMessageOBJ.setType(FVRESP);
      sendMessageOBJ.setNodeID(ID);
      socket.sendMessage(sendMessageOBJ.toString(), senderIP, UIPORT);
    }
  else {
    //ASSERT: we could not find in the key
    closestSize = RT.getKClosestNodes(aKey, KClosest);
    sendMessageOBJ.setKClos(KClosest, closestSize);
		
    sendMessageOBJ.setType(KCLOSEST);
    sendMessageOBJ.setNodeID(ID);
    socket.sendMessage(sendMessageOBJ.toString(), senderIP, UIPORT);
  }
  mLock.lock();
  refresherVector.push_back(make_pair(sendTriple, Triple()));
  mLock.unlock();
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
	
  thread PingThread = thread(&Node::startRefresher, this);
  thread UIThread = thread(&Node::startUIListener, this);
  //ASSERT: Create the two threads for handling Pings and
  //        for handling UIs
	
  std::string sendString; //the message we will fill up and send
  std::string receiveString; //the message we will receive
	
  uint32_t senderIP;//the IP of the node we're receiving a msg from
  uint32_t senderID; //the ID of the node we're receiving a msg from
	
  int32_t recNum = -1;
	
  UDPSocket socket(MAINPORT, "main.log");
  //ASSERT: connect socket to our main port
	
	
  while(!exit)
    {
      recNum = socket.recvMessage(receiveString);
		
      if(recNum > 0) // if we received a message through the socket.
	{
	  Message receivedMessageOBJ(receiveString);
	  senderIP = socket.getRemoteIP();
	  senderID = receivedMessageOBJ.getNodeID();
	  Triple sendTriple (senderIP, senderID, MAINPORT);
				
	  printf("%s from %s\n", receiveString.c_str(), IP_toString(senderIP).c_str());
	  uint32_t aKey = receivedMessageOBJ.getID();
	  //ASSERT: to be extracted from each message.
				
	  switch (receivedMessageOBJ.getMsgType())
	    {
	    case STORE:
	      {
		//ASSERT: A node wants to store
		mainStore(aKey, sendTriple);
	      }
	      break;
					
	    case FINDNODE:
	      {
		//ASSERT: A node wants k closest nodes
		mainFindNode(aKey, sendTriple, senderIP, socket);
	      }
	      break;
					
	    case FINDVALUE:
	      {
		//ASSERT: A node is trying to find a key
		mainFindValue(aKey, sendTriple, senderIP, socket);
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

      IP=0;
		
      /// Check and Handle new message
      if (socket.recvMessage(incoming) != -1) // if it returns 0 then no message was received
	{
	  IP=socket.getRemoteIP(); // get the IP of new message
			
	  // Print incoming message
	  printf("%s from %s\n", incoming.c_str(), IP_toString(IP).c_str());
			
	  // Parse incoming message
	  msg.parse(incoming);
				
	  switch(msg.getMsgType())
	    {
	    case PING: // We are receiving a ping request
	      {
		Message pingr(PINGRESP, this->ID); // respond immediately
		socket.sendMessage (pingr.toString(), IP, REFRESHERPORT);
	      }
	      break;
					
	    case PINGRESP: // we are receiving a PINGRESP to a PING we have sent.
	      {
		handlePINGRESP(IP, msg);
	      }
	      break;
					
	    default:
	      printf("Unrecognized message received: %s\n", incoming.c_str());
	      break;
	    }
	}
		
      /// Check routingTable refreshing
      routingTableRefresh(socket, curKBucket, lastRefresh, refresh, i, j);
		
      /// Update elements in the refresher vector
      updateRefresherVectorElements(socket);
		
      /// Check timeouts related to Refresher and Pinger
      checkPingTimeOut();
      // End of While Loop
    }
	
}

// Check if it is time to refresh the routingTable or if we are already in the process of refreshing it check if we can send more messages.
void Node::routingTableRefresh(UDPSocket & socket, KBucket & curKBucket, MsgTimer & lastRefresh, bool & refresh, uint32_t & i, uint32_t &j)
{
  /// Currently refreshing the routingTable
  if(refresh)
    {
      // check if we can send more PINGs
      if(timeouts[REFRESH_TIMEOUT].size()<ALPHA)
	// send more messages such that a max of alpha are sent.
	sendUpToAlphaPing(curKBucket, socket, i, j, lastRefresh, refresh);
		
    }
	
  /// Check if we are currently refreshing and if it is time to refresh
  if(!refresh && lastRefresh.timedOut())
    {
      refresh = true; // start refreshing
      i=j=0; // reset indices
		
      // Retrieve the first KBucket
      curKBucket = RT[i];
		
      // Send the first alpha messages
      sendUpToAlphaPing(curKBucket, socket, i, j, lastRefresh, refresh);
		
    }
}

// check timeouts & clear timeouts using IP.
// Assumes that there is no case where we could have pinged the same IP more than once and have more than one timeout corresponding
void Node::handlePINGRESP(uint32_t & IP, Message & msg)
{
  bool found = false;
	
  // Check in timeouts for other threads & refresher
  int m, n;
  for (m =0, n=0; (m<timeouts[PINGER_TIMEOUT].size() ||
		   n<timeouts[REFRESH_TIMEOUT].size()) && !found; ++m, ++n)
    {
      // Checking in other threads timeouts
      if(m<timeouts[PINGER_TIMEOUT].size() && timeouts[PINGER_TIMEOUT][m].getNodeIP() == IP) // If we found a timeout with the same IP
	{
	  // Which one it corresponded to
	  int32_t index = findInRefresherVector(timeouts[PINGER_TIMEOUT][m].getNodeID());
			
	  if (index != -1)
	    {
	      // erase it from the refresherVector
	      mLock.lock();
	      refresherVector.erase(refresherVector.begin()+index);
	      mLock.unlock();
	      // get the Kbucket index
	      int32_t kbindex = RT.findKBucket(timeouts[PINGER_TIMEOUT][m].getNodeID());
				
	      if(refreshCounters[kbindex]>0) refreshCounters[kbindex]--;
	    }
			
	  // erase element in vector
	  timeouts[PINGER_TIMEOUT].erase(timeouts[PINGER_TIMEOUT].begin()+m);
	  found = true; // Update flag
	  m--;
	}
		
      // Checking in timeouts for refresher
      if(!found && n<timeouts[REFRESH_TIMEOUT].size() &&
	 timeouts[REFRESH_TIMEOUT][n].getNodeIP() == IP)
	// If we found a timeout with the same IP
	{
	  // erase element in vector
	  timeouts[REFRESH_TIMEOUT].erase(timeouts[REFRESH_TIMEOUT].begin()+n);
	  found = true; // Update flag
	  n--;
	}
		
      //Refreshing nodes in updateTable if possible
      if(msg.getNodeID() != ID)
	RT.updateTable(msg.getNodeID(), IP);
    }
	
  if(!found)
    {
      if(msg.getNodeID() != ID)
	RT.updateTable(msg.getNodeID(),IP);
    }
}

// Takes care of trying to PING, update or delete elements that have been inserted in the refreshervector by the main thread and the UI thread.
void Node::updateRefresherVectorElements (UDPSocket & socket)
{
	
  /// Update elements in the refresher vector
  for (int i=0; refresherVector.size()>0 && i<ALPHA && 
	 i<refresherVector.size(); ++i)
    {
      // try to update in table then ping if necessary
      if ((refresherVector[i].first.node != ID)&&
	  !RT.updateTable(refresherVector[i].first.node, refresherVector[i].first.address))
	{
	  int32_t index = RT.findKBucket(refresherVector[i].first.node);
			
	  if(refreshCounters[index]<K)
	    {
	      refreshCounters[index]++;
				
	      // Get LRU node from the appropriate kbucket
	      Triple tripleToRefresh = RT.getOldestNode(refresherVector[i].first.node);
	      mLock.lock();
	      refresherVector[i].second = tripleToRefresh;
	      mLock.unlock();
				
	      // PING
	      Message msg(PING, ID);
	      socket.sendMessage(msg.toString(), tripleToRefresh.address, REFRESHERPORT);
				
	      // Add to the timeouts
	      MsgTimer timer(RESPONDTIME_PING, tripleToRefresh.node, tripleToRefresh.address);
	      timeouts[PINGER_TIMEOUT].push_back(timer);
	    }
	  else refresherVector.erase(refresherVector.begin()+i);
			
	}
      else refresherVector.erase(refresherVector.begin()+i);
      // Remove from the vector, the node was refreshed
    }
}

// Check Timeouts resulting from PINGing an element from the refreshervector or from refreshing the nodes in the routing table, and delete them from the routing Table if the node timed out.
void Node::checkPingTimeOut()
{
  int i=0, j=0;
  // Check in timeouts for other threads & refresher
  for ( i=0, j=0; i<timeouts[PINGER_TIMEOUT].size() ||
	  j<timeouts[REFRESH_TIMEOUT].size(); ++i, ++j)
    {
      // Checking if anything timed out and remove it.
      if(i<timeouts[PINGER_TIMEOUT].size() && timeouts[PINGER_TIMEOUT][i].timedOut())
	{
	  // Which one it corresponded to in the refreshervector
	  int32_t index = findInRefresherVector(timeouts[PINGER_TIMEOUT][i].getNodeID());
			
	  if (index != -1)
	    {
	      // erase it from the refresherVector
	      mLock.lock();
	      refresherVector.erase(refresherVector.begin()+index);
	      mLock.unlock();
				
	      // get the Kbucket index
	      int32_t kbindex = RT.findKBucket(timeouts[PINGER_TIMEOUT][i].getNodeID());
				
	      if(refreshCounters[kbindex]>0) refreshCounters[kbindex]--;
	    }
			
	  RT.deleteNode(timeouts[PINGER_TIMEOUT][i].getNodeID());
	  timeouts[PINGER_TIMEOUT].erase(timeouts[PINGER_TIMEOUT].begin()+i);
	  i--;
	}
		
      if (j<timeouts[REFRESH_TIMEOUT].size() && timeouts[REFRESH_TIMEOUT][j].timedOut())
	{
	  RT.deleteNode(timeouts[REFRESH_TIMEOUT][i].getNodeID());
	  timeouts[REFRESH_TIMEOUT].erase(timeouts[REFRESH_TIMEOUT].begin()+j);
	  j--;
	}
    }
  // End of ping timeouts
}

//---------------------------User Interface Related-----------------------

//Look at our current keys and respond true to UI if we have it, otherwise send
//findvalue to up to alpha at a time to Kclosest
void Node::UIFindValue(bool & respondedToUI, Message & curMsg, Message & recvMsg, uint32_t ipUI, SnapShot & snapShot, UDPSocket & socketUI){
  respondedToUI = false;
  MsgType type = recvMsg.getMsgType();
  curMsg.setType(type);
  curMsg.setID(recvMsg.getID());
  if (std::find(keys.begin(), keys.end(), recvMsg.getID())!= keys.end())
    {
      //ASSERT: we have the value, send confirm message
      Message sendMsg(FVRESPP, ID);
      socketUI.sendMessage(sendMsg.toString(), ipUI, TPORT);
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
      if (!snapShot.nextExist() && !respondedToUI)
	{
	  //ASSERT: there is no k clos to check,
	  //        send fail message to UI
	  Message sendMsg(FVRESPN, ID);
	  socketUI.sendMessage(sendMsg.toString(), ipUI, TPORT);
	  respondedToUI= true;
			
	}
      else
	{
	  sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDVALUE);
	}
    }
}

//Find Kclosest and then ask up to alpha at a time what their kclosest are 
void Node::UIStore(Message & curMsg, Message & recvMsg, 
		   bool & respondedToUI, UDPSocket & socketUI, SnapShot & snapShot, uint32_t ipUI){
  // Set the current message to the received request from the UI
  curMsg.setType(recvMsg.getMsgType());
  curMsg.setID(recvMsg.getID());
  // Get KClosest nodes to the key
  Triple kClos[K];
  int size = RT.getKClosestNodes(curMsg.getID(), kClos);
  respondedToUI = false;
	
  // If there are no close nodes, there are no nodes.
  if (size == 0)
    {
      //ASSERT: special case where we are the only node in network,
      //        so we store the key
      mLock.lock();
      keys.push_back(recvMsg.getID());
      mLock.unlock();
		
      // Respond to UI
      Message sendMsg(STORERESP, ID);
      socketUI.sendMessage(sendMsg.toString(), ipUI, TPORT);
    }
  else // Try to find the KClosest to the Key
    {
      snapShot.addClosest(kClos, size);
      sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDNODE);
    }
}

//read the kClosest and add the elements to our snapShot if they are a closer distance.
//Then, if we have run out of Kclosest, then send store to the Kclosest if store,
//otherwise send FVRESPN to the UI
void Node::UIKClosest(Message & recvMsg, SnapShot & snapShot, 
		      Message & curMsg, UDPSocket & socketUI, bool & respondedToUI, uint32_t ipUI){
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
	mLock.lock();
	refresherVector.push_back(make_pair(refresh, Triple()));
	mLock.unlock();
			
	if (!snapShot.nextExist()&& !respondedToUI) {
	  //ASSERT: we have found the K closest, send store messages
	  Message sendMsg(STORE, ID, curMsg.getID());
	  
	  for (int i = 0; i < snapShot.getSize(); i++)
	    {
	      socketUI.sendMessage(sendMsg.toString(),
				   snapShot.getElementIP(i), MAINPORT);
	    }
	  // Send Store Resp to UI
				
	  Message UIansw (STORERESP, ID);
	  socketUI.sendMessage(UIansw.toString(), ipUI, TPORT);
	  respondedToUI = true;
				
	}else{
	  //ASSERT: we are not done searching for kClos
	  sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDNODE);
	}
			
      }
      break;
    case FINDVALUE:
      {
	if (!snapShot.nextExist() && !respondedToUI) {
	  //ASSERT: we have found the K closest and no value,
	  //        send UI that wouldn't couldnt find it.
	  Message sendMsg(FVRESPN, ID);
	  socketUI.sendMessage(sendMsg.toString(), ipUI, TPORT);
	  respondedToUI = true;
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

//someone has find the value, send true to UI
void Node::UIFVResp(Message & recvMsg, UDPSocket & socketUI, 
		    bool & respondedToUI, uint32_t ipUI){
  //printf("START OF UI FV RESP ");
  //printf("%d\n", respondedToUI);
  removeFromUITimeout(recvMsg.getNodeID());
	
  //Add to refresh vector
  Triple refresh;
  refresh.address = socketUI.getRemoteIP();
  refresh.node = recvMsg.getNodeID();

  mLock.lock();
  refresherVector.push_back(make_pair(refresh, Triple()));
  mLock.unlock();
	
  if(!respondedToUI)
    {
      //printf("NOT RESPONDED TO UI\n");
      //ASSERT: Send message only once to UI
      Message sendMsg(FVRESPP, ID);
      socketUI.sendMessage(sendMsg.toString(), ipUI, TPORT);
      respondedToUI = true;
    }
}

//specifically for the adding a node, repsond with our Kclosest
void Node::UIFindNode(Message & recvMsg, UDPSocket & socketUI, uint32_t senderIP){
  uint32_t key = recvMsg.getID();
  Triple kClos [K];
  int32_t closestSize = RT.getKClosestNodes(key, kClos);
	
  Message sendMsg(KCLOSEST, ID);
  sendMsg.setKClos(kClos, closestSize);
	
  // REspond with KCLOSEST
  socketUI.sendMessage(sendMsg.toString(), senderIP, UIPORT);
	
  //create triple for sending off
  Triple sendTriple;
  sendTriple.address = senderIP;
  sendTriple.port = UIPORT;
  sendTriple.node = recvMsg.getNodeID();

  mLock.lock();
  refresherVector.push_back(make_pair(sendTriple, Triple()));
  mLock.unlock();
}

//removes items from timeout if we recieve responses from nodes
void Node::handleUITimeouts(Message & curMsg, SnapShot & snapShot, 
			    bool & respondedToUI, UDPSocket & socketUI, uint32_t ipUI){
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
	    if (!snapShot.nextExist() && !respondedToUI) {
	      //ASSERT: we have found the K closest, send store messages
	      Message sendMsg(STORE, ID);
	      for (int i = 0; i < snapShot.getSize(); i++)
		{
		  socketUI.sendMessage(sendMsg.toString(),
				       snapShot.getElementIP(i), MAINPORT);
		}
					
	      Message UIMsg(STORERESP, curMsg.getID());
	      socketUI.sendMessage(UIMsg.toString(), ipUI, TPORT);
	      respondedToUI = true;
	    }
	    else {
	      //ASSERT: we are not done searching for kClos
	      sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDNODE);
	    }
	  }else if(curMsg.getMsgType() == FINDVALUE)
	  {
	    if (!snapShot.nextExist() && !respondedToUI)
	      {
		//ASSERT: we have found the K closest and no value,
		//        send UI that wouldn't couldnt find it.
		Message sendMsg(FVRESPN, curMsg.getID());
		socketUI.sendMessage(sendMsg.toString(), ipUI, TPORT);
		respondedToUI = true;
	      }
	    else
	      {
		//ASSERT: we are not done searching for kClos
		sendUpToAlphaKClos(snapShot, socketUI, curMsg.getID(), FINDVALUE);
	      }
	  }
      }
      // End of timeouts loop
    }
}

//Handles all UI
//Variable Time
//L1  : port 6667
//      READS: FIND_VALUE_UI, STORE_UI, KCLOS, FIND_VALUE_RESP
//      SENDS: FIND_VALUE, FIND_NODE, STORE
//			TO UI: FIND_VALUE_RESP_POSITIVE, FIND_VALUE_RESP_NEGATIVE, STORE_RESP
void Node::startUIListener() {
  SnapShot snapShot;
  snapShot.setCreatorID(ID);
  Message curMsg(NONE, ID);
	
  std::string strUI;
  Message recvMsg(NONE, ID);
  int recvlenUI;
  int ipUI =0;
  bool respondedToUI = false;
	
  UDPSocket socketUI(UIPORT, "UI.log");
	
  while (!exit)
    {
      //printf("UI Listener\n");
      //Listening on UI socket
      recvlenUI = socketUI.recvMessage(strUI);
      if (recvlenUI > 0)
	{
	  
	  //Update the ip for the UI
			
	  int senderIP = socketUI.getRemoteIP();
			
	  printf("%s from %s\n", strUI.c_str(), IP_toString(senderIP).c_str());
	  
	  // Parsing incoming string
	  recvMsg.parse(strUI);
	  
	  // Get UI IP
	  if(recvMsg.getUI()){
	    respondedToUI = false;
	    snapShot.clear();
	    snapShot.setCompareID(recvMsg.getID());
	    ipUI = senderIP;
	  }
			
	  switch (recvMsg.getMsgType())
	    {
	    case FINDVALUE:
	      {
		UIFindValue(respondedToUI, curMsg, recvMsg, ipUI, snapShot, socketUI);
	      }
	      break;
	    case STORE:
	      {
		UIStore(curMsg, recvMsg, respondedToUI, socketUI, snapShot, ipUI);
	      }
	      break;
					
	    case KCLOSEST:
	      {
		UIKClosest(recvMsg, snapShot, curMsg, socketUI, respondedToUI, ipUI);
	      }
	      break;
	    case FVRESP:
	      {
		//ASSERT: we have the value, send confirm message
		UIFVResp(recvMsg, socketUI, respondedToUI, ipUI);
	      }
	      break;
	    case FINDNODE:
	      {
		//ASSERT: A node wants k closest nodes
		UIFindNode(recvMsg, socketUI, senderIP);
	      }
	      break;
	    default:
	      break;
	    }
			
	  // TIMEOUTS
	  handleUITimeouts(curMsg, snapShot, respondedToUI, socketUI, ipUI);
			
	}
		
      // End of while loop
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
void Node::sendUpToAlphaKClos(SnapShot & ss, UDPSocket & sock, 
			      uint32_t msgID, MsgType mType)
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


// The function tries to find the next element to PING but makes sure that no more than ALPHA messages were already sent. If there is an element to ping it will send a PING message to the node and update the timeouts array
void Node::sendUpToAlphaPing(KBucket &curKBucket, UDPSocket &socket,
			     uint32_t & i, uint32_t & j, MsgTimer & lastRefresh,
			     bool & refresh)
{
  // while there is at most ALPHA messages sent and we are still refreshing.
  while (timeouts[REFRESH_TIMEOUT].size()<ALPHA && refresh)
    {
      while(j>=curKBucket.getNumTriples() && refresh)
	// Check if we have reached the end of the Kbucket
	{
	  if(i>= NUMBITS-1) // If we did all the KBuckets, reset
	    {
	      // seet last refresh timepoint to Now
	      lastRefresh.resetTimer();
	      refresh = false; // stop refreshing after sending to all elements in the KBuckets
	    }
	  else
	    {
	      i++; // Go to next KBucket
	      curKBucket = RT[i];
	      // Start at first element of the KBucket.
	      j =0;
	    }
	}
		
      if(refresh) // If we are still getting more elements
	{
	  // get next element in curKBucket and increment j
	  Triple curTriple = curKBucket[j++];
				
	  if(curTriple.node != ID)
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

// PRE: Takes the nodeID of the Node that has responed to a PING
// POST: Searches the refresherVector for an Triple that we have contacted to check if they were alive.
uint32_t Node::findInRefresherVector (uint32_t nodeid)
{
  bool found = false; // flag to stop the loop when we find what we want.
  uint32_t index = -1; // index of element in the vector that we found to correspond
	
  for(int i =0; i<refresherVector.size() && !found; ++i) // Loop over at most the size of the refresherVector.
    {
      if(refresherVector[i].second.node == nodeid) // check if element that was pinged is the same.
	{
	  index= i;
	  found = true;
	}
    }
  return index;
}
