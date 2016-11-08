//
//  Message.cpp
//  Kademlia
//
//  Created by Ingy on 9/25/16.
//  Copyright © 2016 Roanoke. All rights reserved.
//

#include "Message.hpp"
#include <cstdlib>
#include <stdexcept>
#include <iostream>

// Constructor that takes a string to parse and initializes member data
Message::Message (std::string msg) : msgType(NONE), msg(msg), size(0)
{
	parse(msg); // call the parsing function
}

// Constructor that takes the member data and create the string
Message::Message(MsgType type, uint32_t nodeID, uint32_t ID, bool UI):size(0)
{
	toString(type, nodeID, ID, UI); // Initialize data and create string
}

// Copy constructor
Message::Message(const Message & rhs) : msg(rhs.msg), msgType(rhs.msgType), ID(rhs.ID), isUI(rhs.isUI), size(rhs.size)
{
	for (int i =0; i<K; ++i)
		this->kclos[i] = rhs.kclos[i];
}

// Default constructor
Message::Message(): msgType(NONE), size(0) {}

// Destructor.
Message::~Message(){}

// This function is to help checking for the type of the Message
MsgType Message::getMsgType() const
{
	return msgType;
}

//This function parses the message and updates the appropriate private members. This function alters the string and
void Message::parse (std::string message)
{
	// Initializing parsing variables
	unsigned long index = -1;
	msgType = NONE;
	Message::msg = message;

	// Getting the sender ID
	index=message.find_first_of(" ");
	if(index!= -1)
	{
		// Reading the nodeID
		std::string s = message.substr(0, index+1);
		nodeID = atoi(s.c_str());

		// erasing the ID to continue parsing
		message.erase(message.begin(), message.begin()+index+1);
	}
	else printf( "Error in message, nodeID of sender not found\n");

	// Check if the message is a UI message.
	index = -1;
	index=message.find("UI");
	if (index!= -1)
	{
		isUI = true;
		message.erase(message.begin()+index, message.begin()+index+2);
	}
	else isUI = false;

	// Checking and parsing the different types.
	if (message.find(msgStrings[PING]) != -1) //PING type
	{
		msgType = PING;
	}
	else if ((index = message.find(msgStrings[STORE])) != -1) // STORE type
	{
		msgType = STORE;
		// remove the STORE in the message
		message.erase(message.begin(), message.begin()+msgStrings[STORE].size());

		index = message.find(" "); // Find the space before the ID

		std::string id = message.substr(0, index); // Convert the ID to a number.
		ID = atoi(id.c_str());
	}
	else if ( message.find(msgStrings[FINDNODE])!= -1) // FINDNODE type
	{
		msgType = FINDNODE;
		// Delete the type word in the string
		message.erase(message.begin(), message.begin()+msgStrings[FINDNODE].size());
		// Convert the ID we are looking for to a number
		ID = atoi (message.c_str());
	}
	else if (message.find(msgStrings[FINDVALUE])!= -1) // FINDVALUE type
	{
		msgType = FINDVALUE;
		// Delete the type word in the string
		message.erase(message.begin(), message.begin()+msgStrings[FINDVALUE].size());
		// Convert the ID we are looking for to a number
		ID = atoi(message.c_str());
	}
	else if (message.find(msgStrings[KCLOSEST])!= -1) // KCLOSEST type
	{
		msgType = KCLOSEST;
		// Delete the type word in the string
		message.erase(message.begin(), message.begin()+msgStrings[KCLOSEST].size());
		// Call the function to parse the elements in the array.
		parseKClosest(message);

	}
	else if(message.find(msgStrings[PINGRESP]) != -1) // PINRESP type
	{
		msgType = PINGRESP; // set the appropriate type
	}
	else if (message.find(msgStrings[FVRESPP])!= -1) // FVRESPP type
	{
		msgType = FVRESPP;// set the appropriate type
	}
	else if (message.find(msgStrings[FVRESPN])!= -1) //FVRESPN type
	{
		msgType = FVRESPN;// set the appropriate type
	}
	else if(message.find(msgStrings[FVRESP]) != -1) // FVRESP
	{
		msgType = FVRESP;// set the appropriate type
	}
	else if(message.find(msgStrings[STORERESP]) != -1) // STORERESP
	{
		msgType = STORERESP;// set the appropriate type
	}
	else printf("Parsing error: unrecognized type in string %s", msg.c_str());

	if(msgType == NONE) msg = ""; // If we haven't found any of the type make it NONE.

}

//The function parses the string to retrieve each element (Node IP, UDP Port, and Node ID)
void Message::parseKClosest (std::string & message)
{
	// "Retrieving the size from the message
	int ind = 0;
	std::string temp;
	
	ind = message.find("\n"); // get the first line
	
	if(ind == -1)
		throw std::invalid_argument("Error in parsing message invalid format");
	
	temp = message.substr(0, ind); // take the size
	message.erase(message.begin(), message.begin()+ind);
	
	size = atoi(temp.c_str()); // Converting the size to an integer.
	
	/// Iterate and insert in Kclos
	/// One line is of the form :
	/// IP   UDPPort   NodeID
	
	for (int i=0; i<size; ++i) // for all the nodes stored in the string
	{
		ind = message.find(" ");
		
		if(ind == -1)
			throw std::invalid_argument("Error in parsing message invalid format");
		
		// IP
		temp = message.substr(0, ind);
		kclos[i].address = atoi(temp.c_str()); // Update in kclos array
		
		message.erase(message.begin(), message.begin()+ind+1); // Erase IP
		
		ind = message.find(" ");
		if(ind == -1)
			throw std::invalid_argument("Error in parsing message invalid format");
		
		//UDPPort
		temp = message.substr(0, ind);
		kclos[i].port = atoi(temp.c_str()); // Update in kclos array
		
		message.erase(message.begin(), message.begin()+ind+1); // Erase UDP Port
		
		ind = message.find("\n");
		if(ind == -1)
			throw std::invalid_argument("Error in parsing message invalid format");
		
		// Node ID
		temp = message.substr(0, ind);
		kclos[i].node = atoi(temp.c_str()); // Update in kclos array
		
		message.erase(message.begin(), message.begin()+ind+1); // Erase the nodeID
	}
}

// Creates a message  as a string to be able to send it through the socket.
std::string Message::toString(MsgType type, uint32_t nodeID, uint32_t ID, bool UI)
{
	msg = "" ;

	// assigning member data
	this->nodeID = nodeID;
	this->ID = ID;
	this->isUI = UI;
	this->msgType = type;

	// Adding the Node ID before the rest of the message
	char temp[32];
	sprintf(temp, "%u", nodeID); // convert to string
	msg+= std::string(temp)+ " ";

	switch (type) // Switch on the type
	{
		case PING:
			msg += msgStrings[PING]; // Add the PING type to message
			break;

		case STORE:
			if(ID != -1) // if the ID is valid, add it to the message
			{
				sprintf(temp, "%u", ID); // convert to string
				msg += msgStrings[STORE]+ std::string(temp);
			}
			break;

		case FINDNODE:
			if (ID != -1)// if the ID is valid, add it to the message
			{
				sprintf(temp, "%u", ID); // convert to string
				msg += msgStrings[FINDNODE]+ std::string(temp);
			}
			break;

		case FINDVALUE:
			if(ID != -1)// if the ID is valid, add it to the message
			{
				sprintf(temp, "%u", ID); // convert to string
				msg += msgStrings[FINDVALUE]+ std::string(temp);
			}
			break;

		case KCLOSEST:
			createKClosestMessage(); // call the KClosest message
			break;

		case PINGRESP:
			msg += msgStrings[PINGRESP]; // Add the PINGRESP type to message
			break;

		case FVRESP:
			msg += msgStrings[FVRESP];// Add the FVRESP type to message
			break;

		case STORERESP:
			msg += msgStrings[STORERESP]; // Add the STORERESP type to message
			break;

		case FVRESPP:
			msg += msgStrings[FVRESPP]; // Add the FVRESPP type to message
			break;

		case FVRESPN:
			msg += msgStrings[FVRESPN]; // Add the FVRESPN type to message
			break;

		case NONE: // If the message is uninitialized
			msg="";
			break;

		default: // Unrecognized type.
			msg = "";
			printf("Message create error: Unable to create string properly!\n");
			break;
	}

	if(UI) msg += " UI"; // Add the UI tag if appropriate

	return msg;
}

// The function takes each triple (Node IP, UDP Port, and Node ID) and converts them to strings before adding them to the string msg.
void Message::createKClosestMessage()
{
	msg += msgStrings[KCLOSEST];
	char temp[32];
	
	// Adding the size of the array to the message
	sprintf(temp, "%u", size);
	msg += std::string(temp);
	
	/// iterate over all elements of kclosest and put them in the message.
	/// One line is of the form :
	/// IP   UDPPort   NodeID
	
	for (int i =0; i<size; ++i) // For all elements in the array.
	{
		msg += "\n";
		
		// Adding the IP
		char temp [32];
		sprintf(temp, "%u", kclos[i].address); // convert to string
		msg += std::string (temp) + " ";
		
		// Adding the UDP Port
		sprintf(temp, "%u", kclos[i].port); // convert to string
		msg += std::string (temp)+" ";
		
		// Adding the Node ID
		sprintf(temp, "%u", kclos[i].node); // convert to string
		msg += std::string (temp);
	}
	msg += "\n";
}

//This function creates a string relative to its attributes that can be used when communicating. The function will return an empty string if it fails.
std::string Message::toString()
{
	std::string m = "";

	// call toString() with internal data members
	m=toString(msgType, nodeID, ID, isUI);

	return m;
}

// Returns the Node of File ID.
uint32_t Message::getID()
{
	return ID;
}

// Returns the ID of the sender node.
uint32_t Message::getNodeID()
{
	return nodeID;
}

// This function does not check the types, it assumes it's taking the right input and just sets the internal message type
void Message::setType(MsgType type)
{
	this->msgType = type;
}

// Sets the internal member ID. This function assumes the ID is valid.
void Message::setID(uint32_t id)
{
	this->ID= id;
}

// The function sets the internal member NodeID to the id that is passed. The function assumes it is taking the right input for the senderID.
void Message::setNodeID(uint32_t nodeID)
{
	this->nodeID= nodeID;
}

// takes a boolean to set the Message as a UI message or change it to non UI
void Message::setUI(bool UI)
{
	this->isUI = UI;
}

// the function sets the internal member kclos with the array it is passed, the function does not do any type of checking.
void Message::setKClos ( Triple clos [K], uint32_t s)
{
	size = s;
	for (int i =0; i<s; ++i) // copy all elements into kclos[]
	{
		kclos[i]= clos [i];
	}
}

// Retrieve the elements in the kclos array after parsing
uint32_t Message::getKClos(Triple *clos)
{
	for(int i=0; i < size; i++) // Copying all items
	{
    clos[i] = kclos[i];
  }
  return size;
}

// returns true if the Message is from the UI and false if it is not.
bool Message::getUI()
{
	return isUI;
}

// This function is to help checking for the type of the Message
void Message::printMessageType()
{
	printf("Type: %s \n",  msgStrings[msgType].c_str());
}

// It checks if a nodeID is present in the array returned in the message. It returns true only if the nodeID we are checking for is found.
bool Message::includes(uint32_t & id)
{
	bool found = false;
	for (int i=0; i<K && i<size && !found; ++i) // check if our kclos array contains the node searched for.
	{
		if(kclos[i].node == id)
			found = true;
	}
	return found;
}
