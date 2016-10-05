//
//  sendMessage.hpp
//  Kademlia
//
//  Created by Ingy on 10/5/16.
//  Copyright © 2016 Roanoke. All rights reserved.
//

#ifndef MessageCom_cpp
#define MessageCom_cpp

#include "Message.hpp" // This includes the class which represents a Message objects which is responsible for creating the appropriate messages between nodes.

#include "kBucket.h"

/*
 PRE: Assumes that the Node has been initialized properly. It takes a pointer to a boolean which is assumed to be set to true only when the program should exit. The msgType will enable the function to decide what type of message to create. IP is the IP address of the Node we want to contact. ID is the file or Node ID that we are looking for.
 
 ****I am not sure yet what parameters it should take
 
 POST: This function is called when spawning a thread to send a message to another node. This function creates the message depending on the parameters passed to it. Creates a connection to the destination node. sends the message to the node and waits for the response which will be in the form of a kBucket.
 */

KBucket sendMessage (bool * exit, MsgType msgType, int IP, int ID  = -1);

/*
 PRE: Assumes that the Node has been initialized properly. 
 
 ****I am not done with these comments yet. We need to discuss this function.
 
 POST: This function is called when spawning a thread to respond to a message from another node.
 */

void respondToMessage (bool * exit, Message message);

#endif /* MessageCom_cpp */
