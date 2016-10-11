#include "Node.h"
#include <cstdlib>
#include <cstdio>

#define NEW_NETWORK 2
#define JOIN_NETWORK 5

int main(int argc, char * argv[]) {

	//we are creating a new network
	if (argc == NEW_NETWORK) {
		uint32_t nodeID = atoi(argv[1]);
		Node node(nodeID);
		node.listenerLoop();
	}
	//we are joining an existing network
	else if (argc == JOIN_NETWORK) {
		uint32_t nodeID = atoi(argv[1]);
		uint32_t contactID = atoi(argv[2]);
		uint32_t contactIP = atoi(argv[3]);
		uint32_t contactPort = atoi(argv[4]);
		Node node(nodeID, contactID, contactIP, contactPort);
		node.listenerLoop();
	}
	else {
		printf("Error for the number of arguements.\n");
		printf("USAGE: [ID]\n");
		printf("USAGE: [ID] [contactID] [contactIP] [contactPort]\n");
	}
	
return	0;
}