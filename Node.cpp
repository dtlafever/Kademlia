//Kadelima Node Class
#include "UDPSocket.h"
#include "Node.h"
#include "constants.h"
#include "Message.hpp"
#include "UDPSocket.h"
#include "SnapShot.h"
#include <vector>
#include <algorithm>


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
					case PING:
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
						MsgTimer timer (RESPONDTIME, curTriple.node, curTriple.address);
						timeouts[REFRESH_TIMEOUT].push_back(timer);
					}
					

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
		MsgTimer timer (RESPONDTIME, curTriple.node, curTriple.address);
		timeouts[REFRESH_TIMEOUT].push_back(timer);
	}
	
}



