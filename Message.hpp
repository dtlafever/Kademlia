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
	
	// ID represents the NodeID we're looking up or the key to the file
	uint32_t ID =-1;
	
	/// This implemententation of Message assumes that there is no need for getting the IP address from the file but rather from the socket.
	
public:
	
	// PRE: Takes the message as a parameter
	// POST: Constructor that will call the parse function
	Message (std::string msg);
	
	// Default constructor
	Message ();
	
	// Destructor
	~Message ();

	// These functions are used to check the type of the message after parsing
	bool isSTORE () const;
	bool isPING () const;
	bool isFINDNODE()const;
	bool isFINDVALUE()const;
	
	// PRE: takes the message received as a string.
	// POST: This function parses the message and updates the appropriate private members. This function alters the string and
	void parse (std::string &);
	
	// PRE: takes a MsgType object and an ID which is the NodeID
	// POST: Creates a message to be able to send it in the appropriate format. The return value represents the success or failure. -1 for failure and 0 for success.
	int createMessage (MsgType type, uint32_t ID = -1);
	
	// PRE: this function is to be used if the Message object is already created and we want to retrieve the string format.
	// POST: This function creates a string relative to its attributes that can be used when communicating. The function will return an empty string if it fails.
	std::string createMessage ();
	
	// PRE:
	// POST: returns the Node of File ID.
	uint32_t getID();

};
#endif /* Message_cpp */
