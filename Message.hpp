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
// ========================================================
// PING 		: Message sent to a node we want to ping.
// STORE 		: Message sent to a node that should store some value.
// FINDNODE : Message sent to a node that should return its K closest nodes.
// FINDVALUE: Message sent to a node to ask if that node has the desired value.
// KCLOSEST : Message the a node may return in case it fails to find a value or respoinding to a findnode request.
// PINGRESP : response to a ping request. (Acknowledging we are alive)
// FVRESP   : Positive response to a findvalue.
// STORERESP: Acknowledgement of the completion of the UI's STORE request.
// FVRESPP	: Positive response to the UI for a FINDVALUE request.
// FVRESPN	: Negative response to the UI for a FINDVALUE request.
// ========================================================
enum MsgType { PING, FINDNODE, FINDVALUE, STORE, KCLOSEST, PINGRESP, FVRESP, STORERESP, FVRESPP, FVRESPN, NONE};

// IMPORTANT: This class assumes that anything passed to it is valid. It will not check if the contents in the array are garbage or not, if the size is wrong there is no garantee how the program is going to react.

class Message
{
	// Contents of the message
	std::string msg = "";
	
	// NodeID of sender
	uint32_t nodeID = -1;
	
	// Type of the message
	MsgType msgType;
	
	// ID represents the NodeID we're looking up or the key to the file
	uint32_t ID =-1;
	
	// This boolean flag is set to true when the message came from the User Interface.
	bool isUI = false;
	
	// Represents the KClosest Array
	Triple kclos [K];
	
	// Number of valid elements in KClos
	uint32_t size =0;
	
	// This array stores the names of the different types to simplify the parsing
	const std::string msgStrings [11] = {"PING ", "FINDNODE ",  "FINDVALUE ", "STORE ", "KCLOSEST ", "PINGRESP", "FVRESP", "STORERESP", "FVRESPP", "FVRESPN",  "NONE "};
		
public:
	
	// Default constructor
	Message ();
	
	// PRE: Takes the message as a parameter
	// POST: Constructor that will call the parse function
	Message (std::string msg);
	
	// PRE:
	// POST:
	Message(MsgType type, uint32_t nodeID=-1, uint32_t ID = -1, bool UI = false);
	
	// Copy constructor
	Message (const  Message & rhs);
	
	// Destructor
	~Message();

	// This function is to help checking for the type of the Message
	MsgType getMsgType() const;
	
	// PRE: takes the message received as a string.
	// POST: This function parses the message and updates the appropriate private members. This function alters the string and
	void parse (std::string );
	
	// PRE: takes a MsgType object and an ID which depends on the type of the message
	// POST: Creates a message to be able to send it in the appropriate format.
	std::string toString (MsgType type, uint32_t nodeID=-1, uint32_t ID = -1, bool UI=false);
	
	// PRE: this function is to be used if the Message object is already created and we want to retrieve the string format.
	// POST: This function creates a string relative to its attributes that can be used when communicating. The function will return an empty string if it fails.
	std::string toString ();
	
	// PRE:
	// POST: returns the Node of File ID.
	uint32_t getID();
	
	// PRE:
	// POST: Returns the ID of the sender node.
	uint32_t getNodeID();
	
	// PRE:
	// POST: returns true if the Message is from the UI and false if it is not.
	bool getUI();
	
	// PRE: this function takes a MsgType to set to.
	// POST:  This function does not check the types, it assumes it's taking the right input and just sets the internal message type.
	void setType(MsgType type);
	
	// PRE:
	// POST:
	void setID(uint32_t id);
	
	// PRE:
	// POST:
	void setNodeID(uint32_t id);
	
	// PRE:
	// POST:
	void setUI(bool UI);
	
	// PRE: Takes the array of triples to put in the message and the size of the array
	// POST: the function sets the internal member KClos with the array, the function does not do any type of checking.
	void setKClos (Triple clos [K], uint32_t s);
	
	// PRE: takes an array of triple to updated by reference.
	// POST: returns the size of the array and updates the clos array passed by reference
	uint32_t getKClos (Triple clos[K]);
	
	void printMessageType ();
	
	bool includes(uint32_t & id);

};
#endif /* Message_cpp */
