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
  vector<Triple> refresherVector;
  bool exit = false;
	mutex outputLock;

	// Index 0 is a vector of MsgTimer to keep track of the timeouts for the UI thread.
	// Index 1 is reserved for messages that the PINGer sends for other threads.
	// Index 2 is reserved for timeouts for the refresher -> there should never be more than ALPHA
	vector<MsgTimer> timeouts[3];

  //Pre: msg, queue, and timeOut were declared in the constructor below
  //Post: the id of the node sending msg is removed from timeOut
  //      if our id is in closest times, return true, false other wise
	void handleKClosMsg(Message & msg, vector<MsgTimer>& timeOut,
		JoinNetworkQueue& queue, uint32_t & IP);

	//PRE: a node ID we want to remove from the list
	//POST: finds the node ID in the list and removes from timeout,
	//      if it exist in the list.
	void removeFromUITimeout(uint32_t ID);

	void sendUpToAlphaPing(KBucket &curKBucket, UDPSocket &socket, uint32_t & i, uint32_t & j, MsgTimer & lastRefresh, bool & refresh);

//PRE: the snapshot we are currently using, as well as the socket to 
//     send messages on.
	//POST: sends up to ALPHA nodes FINDVALUE and then add them to
	//      the timer queue.
	void sendUpToAlphaKClos(SnapShot & ss, UDPSocket & sock, uint32_t  msgID);
	
	void clearTimeOut(vector<MsgTimer>& timer);
	
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
  Node(uint32_t nodeID, uint32_t contactID, std::string contactIP);
  

  bool joined();

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
	
};
