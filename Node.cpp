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
      if (msg.getMsgType() == KCLOSEST){
	RT.updateTable(msg.getID(), contactIP, PORT);
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

//Handles messages from other Nodes.
//Everything is constant time
//MAIN: port 6666
//      READS:  STORE, FIND_NODE, FIND_VALUE
//      SENDS:  K_CLOS, FIND_VALUE_RESP
void Node::startListener(){



}

//Handles all UI
//Variable Time
//L1  : port 6667
//      READS: FIND_VALUE_UI, STORE_UI, KCLOS, FIND_VALUE_RESP
//      SENDS: FIND_VALUE, FIND_NODE, STORE
//			TO UI: FIND_VALUE_RESP_POSITIVE, FIND_VALUE_RESP_NEGATIVE, STORE_RESP


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
	time_t last_refresh;
	
	// Current bucket we are checking.
	KBucket curKBucket;
	
	// indices used to access elements of the routing table.
	int i =j =0;
	
	while (!exit)
	{
		// Check for new message
		 if (PING is received)
			 Respond to request
			if PINGRESP was received
				check timeouts & clear timeouts using IP.
			if(refresh)
			{
				check if we can send more PINGs////TODO
				if(can send)
				{
					send more messages such that a max of alpha are sent.
					get next element in curKBucket
					
					if(no more elements)
					{
						i++;
						j =0;
						curKBucket= RT[i];
					}
					
					if(no more KBuckets)
					{
						i=j=0;
						reset last_refresh
						refresh = false;
					}
				}
				
			}
			if(!refresh && time to refresh)
			{
				refresh = true;
				i=j=0;
				retrieve KBucket=RT[0];
				sendAlpha PINGs
				refresh = true;
			}
	}
}


