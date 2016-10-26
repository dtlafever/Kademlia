#include "Node.h"
#include "UDPSocket.h"
#include "SocketException.h"
#include <algorithm>
#include <chrono>
#include <stdlib.h>
#include <iostream>

//---------------------------------------------------------------------------------
//            PRIVATE FUNCTIONS
//---------------------------------------------------------------------------------

uint32_t Node::getMyID() {
  return ID;
}


//---------------------------------------------------------------------------------
//            PUBLIC FUNCTIONS
//---------------------------------------------------------------------------------

//PRE:
//POST: lets create a new network and init this node with 32 empty k buckets and a given id

Node::Node(uint32_t nodeID) : routingTable(nodeID), snap(nodeID)
{
  ID = nodeID;
  exit = false;
	
  threadCount = 0;
	
}

//PRE: the contact node we know about that will allow us to join the network
//POST: create a given id that is unique in this network, create our 32 k buckets that
//      correspond to the network
Node::Node(uint32_t nodeID, uint32_t contactID, uint32_t contactIP,
	   uint32_t contactPort) : routingTable(nodeID), snap(nodeID)
{
  ID = nodeID;
  exit = false;
  routingTable.addNode(contactID, contactIP);
	
	//find_node to the contact node
	UDPSocket socket(UDPPORT);
	Message msg(FINDNODE, nodeID);
	socket.sendMessage(msg.toString(), contactIP, contactPort);
	
	//TODO: Add this message to timeouts. + check if fails 
	
	threadCount = 0;
	
}

void Node::joinNetwork(Triple &contact)
{
	
}

//---------------------------------------------------------------------------------
//            THREAD FUNCTIONS
//---------------------------------------------------------------------------------


//PRE: duration and now
//POST: given the current time and our duration time, add them
//      together to get the new timepoint we wait for.
void Node::resetTimePoint(){
	TIME::time_point currentTime = TIME::now();
	chrono::duration<int, milli>durationTime(20);
	
  waitFor = currentTime + durationTime;
	
}


//PRE: Object defined. threadCount is member data.
//POST: RV is true iff threadCount with the new thread added is less
//      than MAXTHREADS. Otherwise, RV is false.
bool Node::canSpawn(){
  bool allowed = false;
  if( (threadCount + 1) < MAXTHREADS){
    allowed = true;
  }
  return(allowed);
}


//PRE:
//POST: the thread that handles pinging every node in our k buckets
//      every TIME_TO_PING amount of time
void Node::refresher_T()
{
  // TODO: We need to keep timers for all messages sent
  // i is the Kbucket and j is the element in the k bucket
  int j=0,i=0;
	
  KBucket curKB;
	
  // This boolean is set to true once we've traversed the whole routing table.
  bool done = false;
	
  while (true)
    {
		
      std::this_thread::sleep_for(std::chrono::milliseconds(PINGTIME));
		
      sendPing (done, ALPHA, curKB, i, j);
		
      while (!done)
	{
			
	  // check refresh IP to see if some have responded
	  while (refreshIP.size()==ALPHA){};
			
	  sendPing(done, ALPHA-refreshIP.size(), curKB, i ,j);
	}
		
      j=i=0;
		
    }
	
}

void Node :: sendPing (bool & done, uint32_t numReq, KBucket & curKB, int & i, int &j)
{
  UDPSocket socket(UDPPORT);
  Message msg (PING);
  string toSend;
  for (int ind=0; ind<numReq && done; ++ind)
    {
      toSend ="";
      if(i<NUMBITS)
	{
	  if(j<curKB.getNumTriples()) // still some triples
	    {
	      Triple temp = curKB[j];
	      socket.sendMessage(msg.toString(), temp.address, UDPPORT);
	      refreshIP.push_back(temp);
	      j++;
	    }
	  else
	    {
	      i++; // we are done with this bucket.
	      j=0;
	      ///TODO: Figure out how to copy the KBucket to curKB
	      curKB = routingTable[i];
	      ind --;
	    }
	}
      else // no more KBuckets
	{
		toSend ="";
		if(i<NUMBITS)
		{
			if(j<curKB.getNumTriples()) // still some triples
			{
				Triple temp = curKB[j];
				socket.sendMessage(msg.toString(), temp.address, UDPPORT);
				pair<Triple, TIME::time_point> tempTime(temp, TIME::now());
				messageTimeouts.push_back(tempTime);
				message
				refreshIP.push_back(temp);
				j++;
			}
			else
			{
				i++; // we are done with this bucket.
				j=0;
				///TODO: Figure out how to copy the KBucket to curKB
				curKB = routingTable[i];
				ind --;
			}
		}
		else // no more KBuckets
		{
			done = true;
		}
	}
    }
}

//PRE:
//POST: responds to other nodes asking for things like findNode and
//      findValue. This thread will spawn sendMessage and recieveMessage
//      threads
void Node::listenerLoop()
{
  std::string msgUDP;
  std::string msgUI;
	
  uint32_t recvlenUDP;
  uint32_t recvlenUI;
	
  uint32_t ipUI = 0;
	
  try
    {
      UDPSocket socketUDP(UDPPORT);
      UDPSocket socketUI(UIPORT);
		
      for (;;)
	{
	  //Listening on UI socket
	  recvlenUI = socketUI.recvMessage(msgUI);
	  if (recvlenUI > 0) {
	    //Update the ip for the UI
	    ipUI = socketUI.getRemoteIP();
				
	    //Handler
	    if(canSpawn()){
	      future<void> Handler;
	      Handler = async(&Node::handler_T, this, msgUI, ipUI);
	      currentThreads.push_back(move(Handler));
	      threadCount = threadCount + 1;
	    }
				
	  }
			
	  //Listening on the UDP socket
	  recvlenUDP = socketUDP.recvMessage(msgUDP);
	  if (recvlenUDP > 0){
				
	    //TODO: the handing of messages and spawning of threads
	    //ASSERT: we definitely got a message from someone
	    int sendTo = socketUDP.getRemoteIP(); // getting the ip of who
	    // sent the message to us
	    // so we can respond to the
	    // message
	    //send to the heavy lifting thread sendTo, msg
				
	    if(canSpawn()){
	      future<void> Handler;
	      Handler = async(&Node::handler_T, this, msgUDP, sendTo);
	      currentThreads.push_back(move(Handler));
	      threadCount = threadCount + 1;
			  
	    }
				
	  }
			
			
	}
    }
  catch (SocketException & e) {
    printf("ERROR: %s\n", ((char *)(e.description().c_str())));
  }
	
  //TODO: Iterate through current open threads and check if done
  //Maybe while loop instead?
  //
  // for(int i = 0; i < threadCount; i++){
  //   resetTimePoint();
  //   if(future_status::ready == currentThreads[i].wait_until(waitFor)){
  //     currentThreads[i].get();
  //   }
  // }
	
}

//PRE: the message we want to read and the UI IP address
//POST: Handle the messages send directly from the UI client
//      STORE:
//        - Call find node to find the k closest nodes
//          to the key value we want to store
//        - Send the STORE message to the k closest nodes
//      FINDVALUE:
//        - check if the key is stored with us. If so,
//          send to UI that we found it.
//        - Otherwise, get the k closest nodes to the key
//        - send findValue to those nodes
//        - if they respond yes, we will send a message to the UI,
//        - otherwise we will update our k closest until there is
//          no more closest. If no more closest, send fail message to UI
void Node::UITagResponse(Message m, uint32_t ip) {
  MsgType type = m.getMsgType();
  uint32_t key = m.getID();
  uint32_t size=0;
  UDPSocket socket(UDPPORT);
  Triple clos[K];
  Triple temp;
  curRequest.parse(m.toString()); // TODO: maybe add an assignment operator for Message
	
  if (type == STORE)
    {
      size = routingTable.getKClosetNodes(key, clos); // retrieves k closest and actual size
		
      // What I did : I got the Kclosest from the Routing Table, I get the ID we are checking for from the message because it's a STORE. I set the key of the snapshot.
		
      snap.clear();
      snap.addClosest(clos, size);
      snap.setCompareID(key);
		
		//Send store to the k closest nodes
		for (int i = 0; (i < ALPHA) && snap.nextExist(); i++)
		{
			Message sendMsg(STORE, key);
			snap.getNext(temp);
			socket.sendMessage(sendMsg.toString(), temp.address, UDPPORT);
			pair<Triple, TIME::time_point> tempTime(temp, TIME::now());
			messageTimeouts.push_back(tempTime);
		}
		
		
		//Send a message to the UI client saying we found the value
		if (std::find(keys.begin(), keys.end(), key) != keys.end())
		{
			Message sendMsg(FVRESPP, ID);
			UDPSocket socket(UIPORT);
			socket.sendMessage(sendMsg.toString(), ip, UIPORT);
		}
		else //Send a message to the node asking us for find value
		{
			
	  /// Initializing snapshot
	  size = routingTable.getKClosetNodes(key, clos); // retrieves k closest and actual size
	  snap.clear();
	  snap.addClosest(clos, size);
	  snap.setCompareID(key);
			
			///Send the FindValue request to Alpha nodes to the k closest nodes
			for (int i = 0; (i < ALPHA) && snap.nextExist(); i++)
			{
				snap.getNext(temp);
				socket.sendMessage(m.toString(), temp.address, UDPPORT);
				pair<Triple, TIME::time_point> tempTime(temp, TIME::now());
				messageTimeouts.push_back(tempTime);
			}
			
	}
    }
}

//PRE: the message we want to read and the UDP IP address
//POST: Handle the message sent from other nodes
//      STORE:
//        - take the key and store it in our keys vector
//      PINGRESP:
//        - we have recieved a ping response from IP
//        - find where in the routing table this ping request
//          came from.
//        - remove that ip from our refreshIP vector
//        - update the node in the routing table
//      FINDVALUE:
//        - check if we have the key and if so, send UI success
//        - if not, find the kclosest to the node ID
//        - send the kClosest to the asker
//      PING:
//        - send pingResp to the asker
//      FINDNODE:
//        - find kclosest to the node ID
//        - send the kClosest to the asker
void Node::nonUIResponse(Message & m, uint32_t ip)
{
  UDPSocket socket(UDPPORT);
  MsgType type = m.getMsgType();
  Message sendMsg;
  uint32_t key = m.getID();
  uint32_t size = 0;
  Triple clos[K];
	
  if (type == STORE) // Store request from another node so we added to out keys.
    {
      keys.push_back(atoi(m.toString().c_str()));
    }
  else if (type == PINGRESP) // Here someone is responding to the Refresher
    {
      bool found = false;
		
      // Find from which node the response came
      for (int i = 0; i < refreshIP.size() && !found; i++)
	{
	  if(refreshIP[i].address == ip)
	    {
	      found = true;
	      routingTable.updateTable(refreshIP[i].node);
	      cout << "Error Finding Node to refresh "<<endl;
				
	      refreshIP.erase(refreshIP.begin()+i);
	    }
	}
		
      if(!found) cout << "Invalid PING response" << endl;
		
    }
  else if (type == FINDVALUE)
    {
		
      //Send a message to the UI client saying we found the value
      if (std::find(keys.begin(), keys.end(), key) != keys.end())
	{
	  sendMsg.setType(FVRESP);
	  socket.sendMessage(sendMsg.toString(), ip, UDPPORT);
	}
      else // We don't have the value so we need to return the k closest
	{
		
		//Send a message to the UI client saying we found the value
		if (std::find(keys.begin(), keys.end(), key) != keys.end())
		{
			temp.//TODO
			sendMsg.setType(FVRESP);
			socket.sendMessage(sendMsg.toString(), ip, UDPPORT);
			pair<Triple, TIME::time_point> tempTime(temp, TIME::now());
			tempTime = tempTime + DELAY_DURATION;
			messageTimeouts.push_back(tempTime);
		}
		else // We don't have the value so we need to return the k closest
		{
	  // Prepare Message by setting the type to KCLOSEST and set the elements
	  sendMsg.setType(KCLOSEST);
			
	  // retrieves k closest and actual size
	  size = routingTable.getKClosetNodes(key, clos);
	  sendMsg.setKClos(clos, size);
			
	}
    }
  else if (type == PING) // Someone is checking we are alive
    {
      sendMsg.setType(PINGRESP);
      socket.sendMessage(sendMsg.toString(), ip, UDPPORT);
    }
  else if (type == FINDNODE)
    {
      // Prepare Message by setting the type to KCLOSEST and set the elements
      sendMsg.setType(KCLOSEST);
		
      // retrieves k closest and actual size
      size = routingTable.getKClosetNodes(key, clos);
      sendMsg.setKClos(clos, size);
		
      socket.sendMessage(sendMsg.toString(), ip, UDPPORT);
    }
}

//PRE: Takes the message received and the incoming IP from the Listener.
//POST: This thread does most of the work and ensures that the protocol steps are fulfilled. If the message is faulty and parsed to NONE, the function should end and discard the message.
void Node::handler_T(string msg, uint32_t ip)
{
  Message m(msg);
  if (m.getUI())
    {
      this->UITagResponse(m, ip);
    }
  else
    {
      if(m.getMsgType() == KCLOSEST || m.getMsgType() == FVRESP)
	{
	  this->nonUITagResponse(m);
	}
      else
	{
	  this->nonUIResponse(m, ip);
	}
    }
}

// PRE: Takes the message that was received
// POST: This function is to be used when the current node receives a response to UI request sent earlier which can be checked through the type of the message, it must be KCLOSEST or FVRESP.

// KCLOSEST:
//     - Check if there are more node to query in the snapshot. **Snapshot should update automatically.
//     - If there are send messages to the alpha next nodes.
//     - Else the process of the current is finished, either it's a Findvalue and it failed or the store is finished.
void Node::nonUITagResponse (Message m)
{
  UDPSocket sock(UIPORT);
  Message msg;
	
  if( m.getMsgType()==FVRESP) // The message returned indicates that FindValue response.
    {
      m.setType(FVRESPP);
      sock.sendMessage(m.toString(), "localhost", UIPORT);
		
      // Clear the snapshot
      snap.clear();
		
    }
  // The message is an answer to a store or findvalue and contains the k closest nodes.
  else if( m.getMsgType() == KCLOSEST) 
    {
      Triple clos [K]; 
      int size= 0;
      size = m.getKClos(clos);
		
      snap.addClosest(clos, size);
		
      if(!snap.nextExist())// Check if there are unqueried nodes & send a max of alpha
			
	{
	  for(int i=0; i<ALPHA && snap.nextExist(); ++i)
	    {
	      Triple next;
	      snap.getNext(next); // check if there is a next triple, if yes if will update the Triple.
		
	      //Special case: we are dealing with STORE and we need
	      //              to keep calling FIND_NODE
	      if (curRequest.getMsgType() == STORE) {
		msg.toString(FINDNODE, curRequest.getID());
		sock.sendMessage(msg.toString(), next.address, UDPPORT);
	      }
	      else {
		sock.sendMessage(curRequest.toString(), next.address, UDPPORT);
	      }
	    }
	}
      else // The process is finished no more nodes to query.
	{
	  // Send back a response to the UI
	  snap.clear();
	  if(curRequest.getMsgType()== STORE)
	    msg.setType(STORERESP);
			
	  else if(curRequest.getMsgType() == FINDVALUE)
	    msg.setType(FVRESPN);
			
	  // Respond directly to UI
	  sock.sendMessage(msg.toString(), "localhost", UIPORT);
			
	}
		
    }
  else printf("Error in response format or parsing \n");
}
