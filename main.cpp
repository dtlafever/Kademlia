//Main Program for Starting a Kademlia Node

#include "Node.h"
#include <stdlib.h>
#include <stdio.h>
#include "Socket.h"

#define MY_ID 1
#define CONTACT_ID 2
#define CONTACT_IP 3

#define CREATE_NETWORK 2
#define JOIN_NETWORK 4

//Pre: argc = 2 or 4, if 4 then argv[2], argv[3] represents an existing node
//     argv[1] is a valid node id not yet taken
//Post: Throws an error if the above is not met or failed to join the network
//      At this stage, just prints error, adjust if we want too
int main(int argc, char * argv[]){
  uint32_t myID = atoi(argv[MY_ID]);
  if (argc == CREATE_NETWORK) {
    Node newNode(myID);
		if(newNode.joined())
		{
			printf("We have started a network.\n");
			newNode.startListener();
		}
		else{
			//ERROR OUT (start error)
			printf("This node has failed to start the network \n");
		}
  }
  else if (argc == JOIN_NETWORK) {
    uint32_t contactID = atoi(argv[CONTACT_ID]);
    std::string strIP(argv[CONTACT_IP]);
    uint32_t contactIP = getIPInt(strIP);
    Node newNode(myID, contactID, contactIP);
    if (newNode.joined()) {
      printf("We have joined a network.\n");
      newNode.startListener();
    }
    else{
      //ERROR OUT (join error)
      printf("This node has failed to join the network \n");
    }
  }
  else {
    //Error Out (argument error)
    printf("Invalid Arguments were given \n");
  }
  return (0);
}
