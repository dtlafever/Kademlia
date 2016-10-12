#ifndef INCLUDE_CONSTANTS
#define INCLUDE_CONSTANTS

//list of contants

#include <stdint.h>

#define K 5
#define UDPPORT 6666
#define UIPORT 9999
#define NUMBITS 32
#define ALPHA 3
#define RESPONDTIME 2000 //in milliseconds, the time to wait before
                        //moving on for an operation (such as findNode)
#define PINGTIME 3600000 //in milliseconds, the time to wait before
                         //pinging a node
#define MAXRECV 500

struct Triple {
  //a myTriples represents a node who does not own this routing table
  //address is the ip address of where the node is
  //port is where the node is listening 
	uint32_t address; // IP Address
	uint32_t port; // UDP Port
	uint32_t node; // Node ID
};

#endif
