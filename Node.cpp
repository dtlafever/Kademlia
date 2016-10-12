#include "Node.h"
#include "UDPSocket.h"
#include "Message.hpp"
#include "SocketException.h"
#include <algorithm>

//---------------------------------------------------------------------------------
//            PRIVATE FUNCTIONS
//---------------------------------------------------------------------------------

uint32_t Node::getMyID() {
	return ID;
}

//PRE: the node we want to ping
//POST: checks to make sure that node is still alive. Removes from the k-bucket
//      if dead node
void Node::ping(uint32_t nodeID) {
	
}

//PRE: the key that represents our file
//POST: Finds the distance (ID XOR key) and sends the store command to the k closest nodes
void Node::findCandidatesForStore(uint32_t key) {
	
}

//PRE: the key that represents our file
//POST: adds the key to our list of keys
void Node::store(uint32_t key) {
	keys.push_back(key);
}

//PRE: the node ID we are looking for in our network
//POST: Starts by picking ALPHA nodes from the closest non-empty bucket. Then call findNode
//      on each of those nodes asyncronously. Nodes that fail to respond quickly (TIME_TO_RESPOND)
//      are removed from consideration until and unless they do respond. If these nodes fail to return
//      nodes that are closer than the closest we have already seen, resend findNode to another ALPHA number
//      of nodes from our list up until k times.
void Node::nodeLookup(uint32_t nodeID) {
	
}

//PRE: the key of the file we want to lookup
//POST: acts the same as the nodeLookup, but now we return true if we can find the key, false otherwise.
bool Node::valueLookup(uint32_t keyID) {
	
}

//PRE: a node ID we want to find in the network
//POST: returns up to k nodes that are closest to the requested nodeID.
//NOTE: if a node fails to respond, they will get removed for the coorsponding bucket
//      until and unless they respond. additionally, the node we are looking for is
//      updated in the appropriate bucket for this node so we can find it later
//      (including adding to our bucket if it does not exist).
//NOTE: by return we mean sending a message
void Node::findNode(uint32_t nodeID) {
	
}

//PRE: a key we want to find in the network
//POST: acts the same as findNode, but now we return true if we have found a value
//NOTE: by return we mean sending a message
void Node::findValue(uint32_t key) {
	
}

//---------------------------------------------------------------------------------
//            PUBLIC FUNCTIONS
//---------------------------------------------------------------------------------

//PRE:
//POST: lets create a new network and init this node with 32 empty k buckets and a given id

Node::Node(uint32_t nodeID) : routingTable(nodeID)
{
	ID = nodeID;
	exit = false;
	
}

//PRE: the contact node we know about that will allow us to join the network
//POST: create a given id that is unique in this network, create our 32 k buckets that
//      correspond to the network
Node::Node(uint32_t nodeID, uint32_t contactID, uint32_t contactIP,
					 uint32_t contactPort) : routingTable(nodeID)
{
	ID = nodeID;
	exit = false;
	routingTable.addNode(contactID, contactIP);
	
	//TODO: FIX WHAT WE WANT TO DO WITH THIS
	//find_node to the contact node
	UDPSocket socket(UDPPORT);
	MsgType t = FINDNODE;
	Message msg(t, nodeID);
	socket.sendMessage(msg.toString(), contactIP, contactPort);
	
}

//---------------------------------------------------------------------------------
//            THREAD FUNCTIONS
//---------------------------------------------------------------------------------

//PRE:
//POST: the thread that handles pinging every node in our k buckets
//      every TIME_TO_PING amount of time
void Node::refresher_T() {
	UDPSocket socket(UDPPORT);
	
	mut.lock();
	
	mut.unlock();
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
			}
			
			//Listening on the UDP socket
			recvlenUDP = socketUDP.recvMessage(msgUDP);
			if (recvlenUDP > 0)
			{
				//TODO: the handing of messages and spawning of threads
				//ASSERT: we definitely got a message from someone
				int sendTo = socketUDP.getRemoteIP(); // getting the ip of who
																							// sent the message to us
																							// so we can respond to the
																							// message
																							//send to the heavy lifting thread sendTo, msg
			}
		}
	}
	catch (SocketException & e) {
		printf("ERROR: %s\n", ((char *)(e.description().c_str())));
	}
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

//POST:

void Node::UITagResponse(Message & m, uint32_t ip) {
	MsgType type = m.getMsgType();
	uint32_t key = stoi(m.toString());

	if (type == STORE) {
		UDPSocket socket(UDPPORT);
		
		Triple clos[K];

		uint32_t size = routingTable.getKClosest(key, clos);

		//TODO: FIND_NODE and get k closest Nodes
		snap = SnapShot(clos, size, );

		Triple snapTriples[K];
		snap.getTriples(snapTriples);
		
		//Send store to the k closest nodes
		for (int i = 0; i < K; i++) {
			Message sendMsg(STORE, ID);
			socket.sendMessage(sendMsg, snapTriples[i].address, UDPPORT);
		}

		//Send to UI that store suceeded
		UDPSocket socketUI(UIPORT);
		Message sendMsgUI(STORERESP, ID);
		socketUI.sendMessage(sendMsgUI.toString(), ip, UIPORT);
	}
	else if (type == FINDVALUE) {

		uint32_t key = stoi(m.toString());
		
		//Send a message to the UI client saying we found the value
		if (std::find(keys.begin(), keys.end(), key) != keys.end()) {
			Message sendMsg(FVRESPP, ID);
			UDPSocket socket(UIPORT);
			socket.sendMessage(sendMsg.toString(), ip, UDPPORT);
		}
		else { //Send a message to the node asking us for find value
			Message sendMsg(KCLOSEST, ID);
			//TODO: send snapshot/K closest of K-closest nodes
			//NOTE: snapshot because it will be sorted by distance,
			//      if we iterate through the list and reach a Triple
			//      that is further than the closest node, stop going
			//      through the list
			Triple clos[K];
			//TODO: do alpha at a time for this thingy
			/*sendMsg.setKClos(clos);
			 UDPSocket socket(UDPPORT);
			 socket.sendMessage(sendMsg.toString(), ip, UDPPORT);*/
			
			//We get to the end, no one has value
			Message sendMsgUI(FVRESPN, ID);
			UDPSocket socket(UIPORT);
			socket.sendMessage(sendMsg.toString(), ip, UDPPORT);
		}
	}
}

//PRE: the message we want to read and the UDP IP address
//POST:
void Node::nonUIResponse(Message & m, uint32_t ip) {
	MsgType type = m.getMsgType();
	if (type == STORE) {
		keys.push_back(stoi(m.toString()));
	}
	else if (type == PINGRESP) {
		//TODO: update K-bucket with this node being most recently used
	}
	else if (type == FINDVALUE) {
		uint32_t key = stoi(m.toString());
		
		//Send a message to the UI client saying we found the value
		if (std::find(keys.begin(), keys.end(), key) != keys.end()) {
			Message sendMsg(FVRESP, ID);
			UDPSocket socket(UDPPORT);
			socket.sendMessage(sendMsg.toString(), ip, UDPPORT);
		}
		else { //Send a message to the node asking us for find value
			Message sendMsg(KCLOSEST, ID);
			//TODO: send snapshot/K closest of K-closest nodes
			//NOTE: snapshot because it will be sorted by distance,
			//      if we iterate through the list and reach a Triple
			//      that is further than the closest node, stop going
			//      through the list
			Triple clos[K];
			sendMsg.setKClos(clos);
			UDPSocket socket(UDPPORT);
			socket.sendMessage(sendMsg.toString(), ip, UDPPORT);
		}
	}
	else if (type == PING) {
		Message sendMsg(PINGRESP, ID);
		UDPSocket socket(UDPPORT);
		socket.sendMessage(sendMsg.toString(), ip, UDPPORT);
	}
	else if (type == FINDNODE) {
		Message sendMsg(KCLOSEST, ID);
		//TODO: send snapshot/K closest of K-closest nodes
		//NOTE: snapshot because it will be sorted by distance,
		//      if we iterate through the list and reach a Triple
		//      that is further than the closest node, stop going
		//      through the list
		Triple clos[K];
		sendMsg.setKClos(clos);
		UDPSocket socket(UDPPORT);
		socket.sendMessage(sendMsg.toString(), ip, UDPPORT);
	}
}

//PRE:
//POST: recieves messages thread
void Node::handler_T( string * msg, uint32_t * ip){
	Message m(*msg);
	if (m.getUI())
	{
		UITagResponse();
	}
	else {
		if(m.getMsgType() == KCLOSEST || m.getMsgType() == FVRESP)
		{
			nonUITagResponse(m);
		}
		else
		{
			nonUIResponse(m, *ip);
		}
	}
}

// PRE: Takes the message that was received
// POST: This function is to be used when the current node receives a response to UI request sent earlier which can be checked through the type of the message, it must be KCLOSEST or FVRESP.

// KCLOSEST:
//     - Check if there are more node to query in the snapshot. **Snapshot should update automatically.
//     - If there are send messages to the alpha next nodes.
//     - Else the process of the current is finished, either it's a Findvalue and it failed or the store is finished.
/// Case when you put STORE in the currequest but actually get answer to a findnode. ????
void Node::nonUITagResponse (Message m)
{
	UDPSocket sock(UIPORT);
	Message msg;

	if( m.getMsgType()==FVRESP) // The message returned indicates that FindValue response.
	{
		m.setType(FVRESPP);
		sock.sendMessage(m.toString(), "localhost", UIPORT);
		
		//////// Clear the snapshot
		
	}
	else if( m.getMsgType() == KCLOSEST) // The message is an answer to a store or findvalue and contains the k closest nodes.
	{
		snap.addClosest(m);
		if(snap.nextExists()) // Check if there are unqueried nodes
		{
			///////// TODO: send to alpha.
			Triple next = snap.getNext();
			sock.sendMessage(curRequest.toString(), next.address, UDPPORT);
			
		}
		else // The process is finished no more nodes to query.
		{
			// Send back a response to the UI
			///////// Clear the Snapshot
			if(curRequest.getMsgType()== STORE)
				msg.setType(STORERESP);
			
			else if(curRequest.getMsgType() == FINDVALUE)
				msg.setType(FVRESPN);
			
			// Respond directly to UI
			sock.sendMessage(msg, "localhost", UIPORT);
			
		}
	}
	else printf("Error in response format or parsing \n");
}

