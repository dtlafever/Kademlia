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
#include "constants.h"

// Enumerated data type describing the type of message. Accessible outside the class.
// KB is when we format the kclosest kbuckets.
enum MsgType { PING, STORE, FINDNODE, FINDVALUE, KCLOSEST, PINGRESP, FVRESP, NONE};

class Message
{
	//Contents of the message
	std::string msg = "";
	
	// Type of the message
	MsgType msgType;
	
	// ID represents the NodeID we're looking up or the key to the file
	uint32_t ID =-1;
	
	// This boolean flag is set to true when the message came from the User Interface.
	bool isUI = false;
	
	// Represents the KClosest Array
	Triple Kclos [K];
	
	// This array stores the names of the different types to simplify the parsing
	const std::string msgStrings [8] = {"PING ", "STORE ", "FINDNODE ", "FINDVALUE ", "KCLOSEST ", "PINGRESP", "FVRESP", "NONE "};
	
	void initializeKClos ();
	
public:
	
	// Default constructor
	Message ();
	
	// PRE: Takes the message as a parameter
	// POST: Constructor that will call the parse function
	Message (std::string msg);
	
	// PRE:
	// POST:
	Message(MsgType type, uint32_t ID = -1, bool UI = false);
	
	// Copy constructor
	Message (const  Message & rhs);
	
	// Destructor
	~Message();

	// This function is to help checking for the type of the Message
	MsgType getMsgType() const;
	
	// PRE: takes the message received as a string.
	// POST: This function parses the message and updates the appropriate private members. This function alters the string and
	void parse (std::string &);
	
	// PRE: takes a MsgType object and an ID which is the NodeID
	// POST: Creates a message to be able to send it in the appropriate format.
	std::string toString (MsgType type, uint32_t ID = -1, bool UI=false);
	
	// PRE: this function is to be used if the Message object is already created and we want to retrieve the string format.
	// POST: This function creates a string relative to its attributes that can be used when communicating. The function will return an empty string if it fails.
	std::string toString ();
	
	// PRE:
	// POST: returns the Node of File ID.
	uint32_t getID();
	
	// PRE: this function takes a MsgType to set to.
	// POST:  This function does not check the types, it assumes it's taking the right input and just sets the internal message type.
	void setType(MsgType type);
	
	// PRE:
	// POST:
	void setID(uint32_t id);
	
	// PRE:
	// POST:
	void setUI(bool UI);
	
	// PRE:
	// POST:
	void setKClos (Triple clos [K]);
	
	// PRE:
	// POST:
	void getKClos (Triple clos[K]);

};
#endif /* Message_cpp */
