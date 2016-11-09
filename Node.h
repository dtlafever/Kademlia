#define PORT 6666
#include "Message.hpp"
#include <vector>
#include "RoutingTable.h"
#include "MsgTimer.h"
#include "UDPSocket.h"
#include "SnapShot.h"
#include "JoinNetworkQueue.h"
#include "MsgTimer.h"
#include "constants.h"
#include <mutex>
#include <utility>

#define UI_TIMEOUT 0
#define PINGER_TIMEOUT 1
#define REFRESH_TIMEOUT 2

using namespace std;

class Node
{

 private:
  
  RoutingTable RT;
  vector<uint32_t> keys;
  uint32_t ID;
  bool inNetwork;
	// refresherVector[i].first = what we want to add to the routing Table
	// refresherVector[i].second = what we need to ping
  vector<pair<Triple, Triple>> refresherVector;
	
	// Counters for nodes that we need to refresh
	uint32_t refreshCounters [NUMBITS];
  bool exit = false;
	mutex mLock; // Mutex to lock 

	// Index 0 is a vector of MsgTimer to keep track of the timeouts for the UI thread.
	// Index 1 is reserved for messages that the PINGer sends for other threads.
	// Index 2 is reserved for timeouts for the refresher -> there should never be more than ALPHA
	vector<MsgTimer> timeouts[3];

  //Pre: msg, queue, and timeOut were declared in the constructor below
  //Post: the id of the node sending msg is removed from timeOut
  //      if our id is in closest times, return true, false other wise
	void handleKClosMsg(Message & msg, vector<MsgTimer>& timeOut,
		JoinNetworkQueue& queue, uint32_t IP);

	//PRE: a node ID we want to remove from the list
	//POST: finds the node ID in the list and removes from timeout,
	//      if it exist in the list.
	void removeFromUITimeout(uint32_t ID);
	
	// PRE: Assumes that the indices i & j (i = KBucket index, j = Index of a triple within the KBucket) are reset each time refresh flag is reset to true. The function takes the current KBucket we are refreshing, the socket through which we send the PING messages, lastRefresh which times when the Node starts refreshing the routingTable which is reset here after going through all triples in the RoutingTable, and the refresh flag which indicates if we are currently refreshing.
	// POST: The function tries to find the next element to PING but makes sure that no more than ALPHA messages were already sent. If there is an element to ping it will send a PING message to the node and update the timeouts array
	void sendUpToAlphaPing(KBucket &curKBucket, UDPSocket &socket, uint32_t & i, uint32_t & j, MsgTimer & lastRefresh, bool & refresh);

//PRE: the snapshot we are currently using, as well as the socket to 
//     send messages on.
	//POST: sends up to ALPHA nodes FINDVALUE and then adds them to
	//      the timer queue.
	void sendUpToAlphaKClos(SnapShot & ss, UDPSocket & sock, uint32_t  msgID, MsgType mType);
	
	//Pre: timer is from the constructor
	//Post: Removes elements from timer that have timed out
	void clearTimeOut(vector<MsgTimer>& timer);
	
	// PRE: Takes the nodeID of the Node that has responed to a PING
	// POST: Searches the refresherVector for an Triple that we have contacted to check if they were alive.
	uint32_t findInRefresherVector (uint32_t nodeid);

 public:

  //Pre: id is a valid node id that is not yet taken
  //Post: RT is initalized, ID = id, inNetwork = true
  //      RT is empty since this node is starting a new network
  Node(uint32_t nodeID);

  //Pre: id is a valid node id that is not yet taken, contactID and contactIP
  //     belongs to a node already existing in the network
  //Post: ID = id, contact exists within our routing table, as well as
  //      other nodes our contact has told about us
  //      inNetwork = true if FindNode on ourselves succeds, false otherwise
  Node(uint32_t nodeID, uint32_t contactID, uint32_t contactIP);
  
	// This function returns true if the node has joined or created a network.
  bool joined();

	//Store a given key into our keys
	void mainStore(uint32_t aKey, Triple & sendTriple);

	//get our KClosest and send it off to the sender
	void mainFindNode(uint32_t aKey, Triple & sendTriple, uint32_t senderIP, UDPSocket & socket);

	//return FVRESP if we have the key, oterhwise send Kclosest
	void mainFindValue(uint32_t aKey, Triple & sendTriple, uint32_t senderIP, UDPSocket & socket);

  //Handles messages from other Nodes.
  //Everything is constant time
  //MAIN: port 6666
  //      READS:  STORE, FIND_NODE, FIND_VALUE
  //      SENDS:  K_CLOS, FIND_VALUE_RESP
  void startListener();

	//Refresher/ Update Table
	//Possibly Variable Time
	//      port 6668
	//      READS: PING_RESP, PING
	//      SENDS: PING, PING_RESP
	
	/* Responsibilities:
			- Refreshing the table using elements in the RefreshQueue
			- Answer PING request
			- PING every nodes in the routing table every hour.
			- Keep track of PING Timeouts
	 */
	void startRefresher();
	
	//Look at our current keys and respond true to UI if we have it, otherwise send
	//findvalue to up to alpha at a time to Kclosest
	void UIFindValue(bool & respondedToUI, Message & curMsg, Message & recvMsg, uint32_t ipUI, SnapShot & snapShot, UDPSocket & socketUI);

	//Find Kclosest and then ask up to alpha at a time what their kclosest are 
	void UIStore(Message & curMsg, Message & recvMsg, bool & respondedToUI, UDPSocket & socketUI, SnapShot & snapSnot, uint32_t ipUI);

	//read the kClosest and add the elements to our snapShot if they are a closer distance.
	//Then, if we have run out of Kclosest, then send store to the Kclosest if store,
	//otherwise send FVRESPN to the UI
	void UIKClosest(Message & recvMsg, SnapShot & snapShot, Message & curMsg, UDPSocket & socketUI, bool & respondedToUI, uint32_t ipUI);

	//someone has find the value, send true to UI
	void UIFVResp(Message & recvMsg, UDPSocket & socketUI, bool & respondedToUI, uint32_t ipUI);

	//specifically for the adding a node, repsond with our Kclosest
	void UIFindNode(Message & recvMsg, UDPSocket & socketUI, uint32_t senderIP);

	//removes items from timeout if we recieve responses from nodes
	void handleUITimeouts(Message & curMsg, SnapShot & snapShot, bool & respondedToUI, UDPSocket & socketUI, uint32_t ipUI);

	//Handles all UI
	//Variable Time
	//L1  : port 6667
	//      READS: FIND_VALUE_UI, STORE_UI, KCLOS, FIND_VALUE_RESP
	//      SENDS: FIND_VALUE, FIND_NODE, STORE
	//			TO UI: FIND_VALUE_RESP_POSITIVE, FIND_VALUE_RESP_NEGATIVE, STORE_RESP
	
	/* Responsibilities:
   		- Handles one request at a time
	 	 	- Handles its own requests.
	 */
	void startUIListener();
	
	// PRE: assumes that all MsgTimer elements  and refreshervector triples were initialized properly. 
	// POST: Check Timeouts resulting from PINGing an element from the refreshervector or from refreshing the nodes in the routing table, and delete them from the routing Table if the node timed out.
	void checkPingTimeOut();
	
	// PRE: Takes the socket through which to PING the elements and assumes that the proper elements were inserted in the vector.
	// POST: Takes care of trying to PING, update or delete elements that have been inserted in the refreshervector by the main thread and the UI thread.
	void updateRefresherVectorElements (UDPSocket &);
	
	// PRE: Assumes that there is no case where we could have pinged the same IP more than once and have more than one timeout corresponding
	// POST: check timeouts & clear the timeout corresponding to the received message using IP.
	void handlePINGRESP(uint32_t & IP, Message &);
	
	// PRE: Assumes that the indices i & j (i = KBucket index, j = Index of a triple within the KBucket) are reset each time refresh flag is reset to true. The function takes the current KBucket we are refreshing, the socket through which we send the PING messages, lastRefresh which times when the Node starts refreshing the routingTable which is reset here after going through all triples in the RoutingTable, and the refresh flag which indicates if we are currently refreshing.
	// POST: Check if it is time to refresh the routingTable or if we are already in the process of refreshing it check if we can send more messages.
	void routingTableRefresh(UDPSocket & socket, KBucket & curKBucket, MsgTimer & lastRefresh, bool & refresh, uint32_t & i, uint32_t &j);
	
};
