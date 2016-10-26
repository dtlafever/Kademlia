#include "Node.h"
#include "constants.h"

#include <thread>

Node::Node(int id) : RT(id){
  ID = id;
  inNetwork = true;
}

Node::Node(int id, int contactID, int contactIP) : RT(id){

  //TODO: RefresherQueue as member data
  
  ID = id;
  Triple contact(contactID, contactIP, PORT);
  RT.updateTable(contact);
  inNetwork = true;
  //ASSERT: contact is in our routing table

  vector<Timeout> TV();

  UDPSocket socket(PORT);

  socket.sendMessage(FIND_NODE ID, contactIP, PORT);
  TV.push_back(contactID);

  StupidQueueObject queue();
  //TODO: create a queue where:
  //       - inserting will not add duplicates
  //       - each element has a bool queried
  //       - each element is a Triple

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

     thread PingThread = thread(startPingListener);
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


  //thread PingThread = thread(startPingListener);
  //Refresher/ Update Table
  //Possibly Variable Time
  //L2  : port 6668
  //      READS: PING_RESP
  //      SENDS: PING

  //thread UIThread = thread(startUIListener);  
  //Handles all UI
  //Variable Time
  //L1  : port 6667
  //      READS: FIND_VALUE_UI, STORE_UI, KCLOS
  //      SENDS: FIND_VALUE, FIND_NODE, STORE
