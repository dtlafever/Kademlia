//
//  Message.hpp
//  Kademlia
//
//  Created by Ingy on 9/25/16.
//  Copyright © 2016 Roanoke. All rights reserved.
//

#ifndef Message_cpp
#define Message_cpp
#include <string>
#include <stdio.h>

// Enumerated data type describing the type of message. Accessible outside the class.
enum MsgType { PING, STORE, FINDNODE, FINDVALUE, NONE};

class Message
{
	//Contents of the message
	std::string msg = "";
	
	// Type of the message
	MsgType msgType;
	
	// ID represents the IP address of the Node we're looking up or the key to the file
	uint32_t ID =-1;
	
public:
	
	// Constructor that will call the parse function
	Message (std::string msg);
	
	// Default constructor
	Message ();
	
	// Destructor
	~Message ();

	// These functions are used to check the type of the message after parsing
	bool isSTORE ();
	bool isPING ();
	bool isFINDNODE();
	bool isFINDVALUE();
	
	// This function parses the message and updates the appropriate private members
	void parse (std::string &);
	
	// Creates a message to be able to send it in the appropriate format
	int createMessage (MsgType type, uint32_t ID = -1);
	
	// get the ID
	uint32_t getID();

};
#endif /* Message_cpp */
