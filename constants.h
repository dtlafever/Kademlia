//list of contants

#define K 5
#define UDPPORT 6666
#define NUMBITS 32
#define ALPHA 3
#define RESPONDTIME 2000 //in milliseconds, the time to wait before moving on for an operation (such as findNode)
#define PINGTIME 3600000 //in milliseconds, the time to wait before pinging a node
#define MAXRECV 500

struct myTriples {
  //a myTriples represents a node who does not own this routing table
  //address is the ip address of where the node is
  //port is where the node is listening 
  int address;
  int port;
  int node;
};

//Constants for the User Interface
#define MAXARGUMENTS 2
#define MINARGUMENTS 1
#define COMMANDPOS 0
#define IDPOS 1
#define MAXCHAR 30

#define MAXCOMMANDS 3
#define EXITPOS 0
#define STOREPOS 1
#define FINDPOS 2

static char allCommands[MAXCOMMANDS][MAXCHAR] = {"exit", "store", "find"};
