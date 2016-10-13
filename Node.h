#ifndef INCLUDE_NODE
#define INCLUDE_NODE

#include <vector>
#include "RoutingTable.h"
#include "constants.h"
#include "Message.hpp"
#include <mutex>
#include "SnapShot.h"
#include <future>
#include <chrono>


using namespace std;

class Node{
	
private:
	//---------------------------------------------------------
	//        MEMBER DATA
	//---------------------------------------------------------

        uint32_t ID;			//The ID of this computer
	vector<uint32_t> keys;		//The keys of the files stored on this pc
	RoutingTable routingTable;	//The K-Buckets
 	bool exit;			//the bool to keep listener ging
	mutex mut;			//The mutex lock for threads
	vector<Triple> refreshIP;   // This keeps track of the IPs the refresher is pinging
	
	Message curRequest;         // Keeps track of the last request sent by the UI.
	
	SnapShot snap;	            // Keeps track of k closest for UI requests.

	int threadCount;            //number of threads owned

	//---------------------------------------------------------
	//        PRIVATE FUNCTIONS
	//---------------------------------------------------------

	vector<future<void> > currentThreads; //a vector to hold all current open threads
	chrono::system_clock::time_point waitFor;
	//a time_point for waiting until we try and join threads
	
	uint32_t getMyID();
	
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

	//PRE: duration and now 
	//POST: given the current time and our duration time, add them
	//      together to get the new timepoint we wait for.
	void resetTimePoint();

	//PRE: Object defined. threadCount is member data.
	//POST: RV is true iff threadCount with the new thread added is less
	//      than MAXTHREADS. Otherwise, RV is false.
	bool canSpawn();
	
	//PRE:
	//POST: the thread that handles pinging every node in our k buckets every
	//      TIME_TO_PING amount of time
	 void refresher_T();

	//PRE:
	//POST: responds to other nodes asking for things like findNode and findValue. This thread
	//      will spawn sendMessage and recieveMessage threads
	 void listenerLoop();
		
	//PRE:
	//POST: recieves messages thread
	void handler_T(string msg, uint32_t ip);

	//PRE: the message we want to read and the UI IP address
	//POST: Handle the messages sent directly from the UI client
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
	void UITagResponse(Message  m, uint32_t ip);

	//PRE:
	//POST:
	void nonUIResponse(Message & m, uint32_t ip);
	
	//PRE:
	//POST:
	void nonUITagResponse (Message m);
	
	void  sendPing (bool & done, uint32_t numReq, KBucket & curKB, int & i, int &j);

};
#endif // !INCLUDE_NODE
