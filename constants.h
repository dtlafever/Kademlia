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
  int node;
};


//Constants for the User Interface
#define MAXARGUMENTS 2
#define MINARGUMENTS 1
