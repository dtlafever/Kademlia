#ifndef INCLUDE_NODE
#define INCLUDE_NODE

#include <Vector>
#include "RoutingTable.h"
#include "constants.h"
#include "Message.hpp"
#include <mutex>
#include "SnapShot.h"

using namespace std;

class Node{
	
private:
	uint32_t ID;				//The ID of this computer
	vector<uint32_t> keys;		//The keys of the files stored on this pc
	RoutingTable routingTable;	//The K-Buckets
 	bool exit;
	mutex mut;					//The mutex lock for threads
	
	Message curRequest; // Keeps track of the last request sent by the UI.
	
	SnapShot snap;

	uint32_t getMyID();
	
	//PRE: the node we want to ping
	//POST: checks to make sure that node is still alive. Removes from the k-bucket
	//      if dead node
	void ping(uint32_t nodeID);
	
	//PRE: the key that represents our file
	//POST: Finds the distance (ID XOR key) and sends the store command to the k closest nodes
	void findCandidatesForStore(uint32_t key);
	
	//PRE: the key that represents our file
	//POST: adds the key to our list of keys
	void store(uint32_t key);
	
	//PRE: the node ID we are looking for in our network
	//POST: Starts by picking ALPHA nodes from the closest non-empty bucket. Then call findNode
	//      on each of those nodes asyncronously. Nodes that fail to respond quickly (TIME_TO_RESPOND)
	//      are removed from consideration until and unless they do respond. If these nodes fail to return
	//      nodes that are closer than the closest we have already seen, resend findNode to another ALPHA number
	//      of nodes from our list up until k times.
	void nodeLookup(uint32_t nodeID);
	
	//PRE: the key of the file we want to lookup
	//POST: acts the same as the nodeLookup, but now we return true if we can find the key, false otherwise.
	bool valueLookup(uint32_t keyID);
	
	//PRE: a node ID we want to find in the network
	//POST: returns up to k nodes that are closest to the requested nodeID.
	//NOTE: if a node fails to respond, they will get removed for the coorsponding bucket
	//      until and unless they respond. additionally, the node we are looking for is
	//      updated in the appropriate bucket for this node so we can find it later
	//      (including adding to our bucket if it does not exist).
	//NOTE: by return we mean sending a message
	void findNode(uint32_t nodeID);
	
	//PRE: a key we want to find in the network
	//POST: acts the same as findNode, but now we return true if we have found a value
	//NOTE: by return we mean sending a message
	void findValue(uint32_t key);
	
public:
	//PRE:
	//POST: lets create a new network and init this node with 32 empty k buckets and a given id
	Node(uint32_t nodeID);
	
	//PRE: the contact node we know about that will allow us to join the network
	//POST: create a given id that is unique in this network, create our 32 k buckets that
	//      correspond to the network
	Node(uint32_t nodeID, uint32_t contactID, uint32_t contactIP,
		uint32_t contactPort);
	
	//---------------------------------------------------------------------------------
	//            THREAD FUNCTIONS
	//---------------------------------------------------------------------------------
	
	//PRE:
	//POST: the thread that handles pinging every node in our k buckets every TIME_TO_PING amount of time
	 void refresher_T();

	//PRE:
	//POST: responds to other nodes asking for things like findNode and findValue. This thread
	//      will spawn sendMessage and recieveMessage threads
	 void listenerLoop();
		
	//PRE:
	//POST: recieves messages thread
	void handler_T( std::string * msg, uint32_t * ip);

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
	void UITagResponse(Message & m, uint32_t ip);

	void nonUIResponse(Message & m, uint32_t ip);
	
	void nonUITagResponse (Message m);

};
#endif // !INCLUDE_NODE