#ifndef INCLUDE_CONSTANTS
#define INCLUDE_CONSTANTS

//list of contants

#include <stdint.h>

#define MAXTHREADS 15 //maximum number of threads

#define MAINPORT 6666      //Port for listening to other nodes
#define UIPORT 6667        //Port for listening for UI requests and reponding to node requests
#define REFRESHERPORT 6668 //Port for pinging
#define TPORT 6669         //Port for sending message to UI terminal

//GLOBAL VARIABLES FOR KADEMLIA NETWORK
#define K 2
#define NUMBITS 4
#define ALPHA 3




#define PINGTIME 60         //in seconds, the time to wait before refreshing the routing table
#define RESPONDTIME_UI 10   //in seconds, the time to wait before 
	                    //we consider a message timed out
#define RESPONDTIME_PING 10 // in seconds, the time before 
	                    //we consider that a PING request has timed out.
#define DELAY_DURATION chrono::duration<int, std::milli> delay(RESPONDTIME)
#define MAXRECV 500         //pinging a node
#define TIME chrono::system_clock //lets make chrono shorter

struct Triple {
	//a Triple represents a node who does not own this routing table
	//address is the ip address of where the node is
	//port is where the node is listening
	uint32_t address; // IP Address
	uint32_t port; // UDP Port
	uint32_t node; // Node ID
	Triple() : address(0), port(0), node(0) {} //DEFAULT VALUES
	Triple (uint32_t & addressRhs, uint32_t & idRhs,  uint32_t portRhs): address(addressRhs), node(idRhs), port(portRhs) {}
	Triple(const Triple & rhs):address(rhs.address), port(rhs.port), node(rhs.node) {}
};

//PRE: the Triple we want to copy our data do and the data triple we want
//POST: deep copy from onto to
void static copyTriple(Triple & to, Triple & from) {
	to.address = from.address;
	to.port = from.port;
	to.node = from.node;
};

//=============================
//    USER INTERFACE COMMENTS
//=============================


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

//Command array for valid inputs
static char allCommands[MAXCOMMANDS][MAXCHAR] = {"exit", "store", "find"};

#endif
