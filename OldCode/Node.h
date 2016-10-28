//#ifndef INCLUDE_NODE
//#define INCLUDE_NODE
//
//#include <vector>
//#include "RoutingTable.h"
//#include "constants.h"
//#include "Message.hpp"
//#include <mutex>
//#include "SnapShot.h"
//#include <future>
//#include <chrono>
//#include <utility>
//
//using namespace std;
//
//class Node{
//	
//private:
//	//---------------------------------------------------------
//	//        MEMBER DATA
//	//---------------------------------------------------------
//
//	uint32_t ID;							//The ID of this computer
//	vector<uint32_t> keys;		//The keys of the files stored on this pc
//	RoutingTable routingTable;	//The K-Buckets
// 	bool exit;			//the bool to keep listener ging
//	mutex mut;			//The mutex lock for threads
//	vector<Triple> refreshIP;   // This keeps track of the IPs the refresher is pinging
//	
//	Message curRequest;         // Keeps track of the last request sent by the UI.
//	
//	SnapShot snap;	            // Keeps track of k closest for UI requests.
//
//	int threadCount;            //number of threads owned
//
//	vector<pair<Triple, TIME::time_point>> messageTimeouts;
//	//TODO: figure out if this vector is going to HUGE
//
//	vector<future<void> > currentThreads; //a vector to hold all current open threads
//	
//	//---------------------------------------------------------
//	//        PRIVATE FUNCTIONS
//	//---------------------------------------------------------
//	
//	uint32_t getMyID();
//	
//	void joinNetwork(Triple & contact);
//	
// public:
//	//PRE:
//	//POST: lets create a new network and init this node with 32 empty k buckets and a given id
//	Node(uint32_t nodeID);
//	
//	//PRE: the contact node we know about that will allow us to join the network
//	//POST: create a given id that is unique in this network, create our 32 k buckets that
//	//      correspond to the network
//	Node(uint32_t nodeID, uint32_t contactID, uint32_t contactIP,
//		uint32_t contactPort);
//	
//	//---------------------------------------------------------------------------------
//	//            PUBLIC FUNCTIONS
//	//---------------------------------------------------------------------------------
//
//	//PRE: duration and now 
//	//POST: given the current time and our duration time, add them
//	//      together to get the new timepoint we wait for.
//	void resetTimePoint();
//
//	//PRE: Object defined. threadCount is member data.
//	//POST: RV is true iff threadCount with the new thread added is less
//	//      than MAXTHREADS. Otherwise, RV is false.
//	bool canSpawn();
//	
//	//PRE:
//	//POST: the thread that handles pinging every node in our k buckets every
//	//      TIME_TO_PING amount of time
//	 void refresher_T();
//
//	//PRE:
//	//POST: responds to other nodes asking for things like findNode and findValue. This thread
//	//      will spawn sendMessage and recieveMessage threads
//	 void listenerLoop();
//		
//	//PRE:
//	//POST: recieves messages thread
//	void handler_T(string msg, uint32_t ip);
//
//	//PRE: the message we want to read and the UI IP address
//	//POST: Handle the messages send directly from the UI client
//	//      STORE:
//	//        - Call find node to find the k closest nodes
//	//          to the key value we want to store
//	//        - Send the STORE message to the k closest nodes
//	//      FINDVALUE:
//	//        - check if the key is stored with us. If so,
//	//          send to UI that we found it.
//	//        - Otherwise, get the k closest nodes to the key
//	//        - send findValue to those nodes
//	//        - if they respond yes, we will send a message to the UI,
//	//        - otherwise we will update our k closest until there is
//	//          no more closest. If no more closest, send fail message to UI
//	void UITagResponse(Message  m, uint32_t ip);
//
//	//PRE: the message we want to read and the UDP IP address
//	//POST: Handle the message sent from other nodes
//	//      STORE:
//	//        - take the key and store it in our keys vector
//	//      PINGRESP:
//	//        - we have recieved a ping response from IP
//	//        - find where in the routing table this ping request
//	//          came from.
//	//        - remove that ip from our refreshIP vector
//	//        - update the node in the routing table
//	//      FINDVALUE:
//	//        - check if we have the key and if so, send UI success
//	//        - if not, find the kclosest to the node ID
//	//        - send the kClosest to the asker
//	//      PING:
//	//        - send pingResp to the asker
//	//      FINDNODE:
//	//        - find kclosest to the node ID
//	//        - send the kClosest to the asker
//	void nonUIResponse(Message & m, uint32_t ip);
//	
//	// PRE: Takes the message that was received
//	// POST: This function is to be used when the current node receives a response to UI request sent earlier which can be checked through the type of the message, it must be KCLOSEST or FVRESP.
//
//	// KCLOSEST:
//	//     - Check if there are more node to query in the snapshot. **Snapshot should update automatically.
//	//     - If there are send messages to the alpha next nodes.
//	//     - Else the process of the current is finished, either it's a Findvalue and it failed or the store is finished.
//	void nonUITagResponse (Message m);
//	
//	void  sendPing (bool & done, uint32_t numReq, KBucket & curKB, int & i, int &j);
//
//};
//#endif // !INCLUDE_NODE
