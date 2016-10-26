//Kadelima Node Class

#include "Node.h"

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
Node::Node(uint32_t id, uint32_t contactID, uint32_t contactIP) : RT(id){
  ID = id;
  RT.updateTable(contactID, contactIP);
  inNetwork = true;
  //ASSERT: contact is in our routing table

  vector<Timeout> TV();

  UDPSocket socket(PORT);

  socket.sendMessage(FIND_NODE ID, contactIP, PORT);
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

  std::string newMSG;
  uint32_t recNum;

  try{
     UDPSocket socket(MAINPORT);
     //ASSERT: connect socket to our main port

     thread PingThread = thread(startRefresher);
     thread UIThread = thread(startUIListener);
     //ASSERT: Create the two threads for handling Pings and
     //        for handling UIs

     while(listening){
       recNum = socket.recvMessage(newMSG);

       if(recNum > 0){
	 Message newMessage(newMSG);

	 if(newMSG.getMSGType() == PING){
	   //give back ping response
	   //add sender to refresh queue
	 }
	 else if(newMSG.getMSGType() == STORE){
	   //push key to our key list
	   //add sender to refresh queue
	 }
	 else if(newMSG.getMSGType() == KCLOS){
	   //give back kclos
	   //add sender to refresh queue
	 }
	 else if(new.getMSGType() == FIND_VALUE){
	   //check for value
	   // if(we have value){
	   //   send back FIND_VALUE_RESP_TRUE;
	   // }
	   // else{
	   //   send KCLOS;
	   // }

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

}
