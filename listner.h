#ifndef LISTNER_INCLUDE
#define LISTNER_INCLUDE

// Johnathan Getz
// A rough header file for the Listner Functions

#include "Message.h"      // The class for encoding and decoding messages sent
                          // between Node objects. used for rapid access of
                          // data stored in the messages sent.
 
#include "sendMessage.h"  // This is the helper file for the functions involved
                          // in sending messages to other nodes. Uses the
                          // Message class for this.

#include "RoutingTable.h" // This is the RoutingTable class which stores
                          // functions for editing k-buckets and the Node's
                          // Routing table

#include <thread>         // Used when spliting our child thread after hearing
                          // the request for connection.

#include <future>         // Used so that threads may be run asychronously
                          // without forcing main_listener to wait for a single
                          // thread before splitting off another.

#include "Socket.h"       // We use the base socket class to make creating
                          // sockets for sending and recieving data from
                          // other Node objects.

/*
 PRE: Assumes that the Node has been properly instantiated and has valid
      k-buckets. takes a boolean value, exit, by reference.
POST: Main thread function which listens to incoming communication from other
      Nodes in the network and splits it's own threads to communicate with
      other Nodes
*/
void main_listener(bool& exit);

/*
 PRE: 
POST: initializes the Socket and waits to accept a connection
*/
void start_listen_socket();

/*
 PRE: Takes the message that was recieved and sorts it into the proper response
POST: Sends the message to the proper sendMessage function to execute the
      proper response
*/
void sort_message(Message message);
#endif
