#define PORT 6666
#include "Message.hpp"
#include <vector>
#include "RoutingTable.h"
#include "MsgTimer.h"

#define UI_TIMEOUT 0
#define PINGER_TIMEOUT 1

using namespace std;


class Node
{

 private:
  RoutingTable RT;
  vector<int> keys;
  int ID;
  bool inNetwork;
	
	vector<MsgTimer> timeouts[2];
  
 public:

	// Creating a network
  Node(int id);

	// Joining a network
  Node(int id, int contactID, int contactIP);

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
	
};
