#include "Node.h"

Node::Node(int id) : RT(id){
  ID = id;
  inNetwork = true;
}

Node::Node(int id, int contactID, int contactIP) : RT(id){
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
  //Handles messages from other Nodes.
  //Everything is constant time
  //MAIN: port 6666
  //      READS: PING, STORE, FIND_NODE, FIND_VALUE
  //      SENDS: PING_RESP, K_CLOS, FIND_VALUE_RESP_TRUE

  //Handles all UI
  //Variable Time
  //L1  : port 6667
  //      READS: FIND_VALUE_UI, STORE_UI, KCLOS
  //      SENDS: FIND_VALUE, FIND_NODE, STORE

  //Refresher/ Update Table
  //Possibly Variable Time
  //L2  : port 6668
  //      READS: PING_RESP
  //      SENDS: PING
}
