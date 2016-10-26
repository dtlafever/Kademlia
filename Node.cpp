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

  socket.sendMessage(uint32_t ID, contactIP, PORT);
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


