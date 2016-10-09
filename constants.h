//list of contants

#define K 5
#define UDPPORT 6666
#define NUMBITS 32
#define ALPHA 3
#define RESPONDTIME 2000
#define PINGTIME 3600000

struct myTriples {
  //a myTriples represents a node who does not own this routing table
  //address is the ip address of where the node is
  //port is where the node is listening 
  int address;
  int port;
  int nodeID;
};


//========================
//USER INTERFACE CONSTANTS
//========================
#define MAXARGUMENTS 2
#define MINARGUMENTS 1
#define COMMANDPOS 0
#define IDPOS 1
#define MAXCHAR 30
#define COMSIZE 3

#define EXITPOS 0
#define STOREPOS 1
#define FINDPOS 2

//Constant char array for each command. 
static char allCommands[COMSIZE][MAXCHAR] = {"exit", "store", "find"};
