//
//  Message.cpp
//  Kademlia
//
//  Created by Ingy on 9/25/16.
//  Copyright © 2016 Roanoke. All rights reserved.
//

#include "Message.hpp"
#include <cstdlib>
#include <iostream>

Message::Message (std::string msg) : msgType(NONE), msg(msg)
{
	size= 0;
	parse(msg);
}

Message::Message(MsgType type, uint32_t nodeID, uint32_t ID, bool UI)
{
	size= 0;
	toString(type, nodeID, ID, UI);
}

Message::Message(const Message & rhs) : msg(rhs.msg), msgType(rhs.msgType), ID(rhs.ID), isUI(rhs.isUI)
{
	size=0;
	for (int i =0; i<K; ++i)
		this->kclos[i] = rhs.kclos[i];
}

Message::Message(): msgType(NONE)
{
	size = 0;
}

Message::~Message(){}

MsgType Message::getMsgType() const
{
	return msgType;
}

void Message::parse (std::string message)
{
	int index = -1;
	
	msgType = NONE;
	msg ="";
	
	if(msg == "")
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
	else
	{
		printf( "Error in message, nodeID of sender not found\n");
	}
	index = -1;
	index=message.find("UI");
	if (index!= -1)
	{
		isUI = true;
		message.erase(message.begin()+index, message.begin()+index+2);
	}
	
	if (message.find(msgStrings[PING]) != -1)
	{
		msgType = PING;
	}
	else if ((index = message.find(msgStrings[STORE])) != -1)
	{
		msgType = STORE;
		
		message.erase(message.begin(), message.begin()+msgStrings[STORE].size());
		
		index = message.find(" ");
		
		std::string id = message.substr(0, index);
		ID = atoi(id.c_str());
	}
	else if ( message.find(msgStrings[FINDNODE])!= -1)
	{
		msgType = FINDNODE;
		
		message.erase(message.begin(), message.begin()+msgStrings[FINDNODE].size());
		
		ID = atoi (message.c_str());
	}
	else if (message.find(msgStrings[FINDVALUE])!= -1)
	{
		msgType = FINDVALUE;
		
		message.erase(message.begin(), message.begin()+msgStrings[FINDVALUE].size());
		
		ID = atoi(message.c_str());
	}
	else if (message.find(msgStrings[KCLOSEST])!= -1)
	{
		msgType = KCLOSEST;
		message.erase(message.begin(), message.begin()+msgStrings[KCLOSEST].size());

		// "Retrieving the size from the message
		int ind = 0;
		std::string temp;
	
		ind = message.find("\n");
		
		if(ind == -1)
			throw std::invalid_argument("Error in parsing message invalid format");
		
		temp = message.substr(0, ind); // take the size
		message.erase(message.begin(), message.begin()+ind);
		
		size = atoi(temp.c_str()); // Converting the size to an integer.
		
		/// Iterate and insert in Kclos
		/// One line is of the form :
		/// IP   UDPPort   NodeID
		
		for (int i=0; i<size; ++i)
		{
			ind = message.find(" ");
			
			if(ind == -1)
				throw std::invalid_argument("Error in parsing message invalid format");
			
			// IP
			temp = message.substr(0, ind);
			kclos[i].address = atoi(temp.c_str());
			
			message.erase(message.begin(), message.begin()+ind+1);
			
			ind = message.find(" ");
			
			if(ind == -1)
				throw std::invalid_argument("Error in parsing message invalid format");
			
			//UDPPort
			temp = message.substr(0, ind);
			kclos[i].port = atoi(temp.c_str());
			
			message.erase(message.begin(), message.begin()+ind+1);

			ind = message.find("\n");
			if(ind == -1)
				throw std::invalid_argument("Error in parsing message invalid format");
			
			// Node ID

			temp = message.substr(0, ind);
			kclos[i].node = atoi(temp.c_str());
			
			message.erase(message.begin(), message.begin()+ind+1);
		}
		
	}
	else if(message.find(msgStrings[PINGRESP]) != -1)
	{
		msgType = PINGRESP;
	}
	else if (message.find(msgStrings[FVRESPP])!= -1)
	{
		msgType = FVRESPP;
	}
	else if (message.find(msgStrings[FVRESPN])!= -1)
	{
		msgType = FVRESPN;
	}
	else if(message.find(msgStrings[FVRESP]) != -1)
	{
		msgType = FVRESP;
	}
	else if(message.find(msgStrings[STORERESP]) != -1)
	{
		msgType = STORERESP;
	}

	
	if(msgType == NONE)
		msg = "";
		
}

std::string Message::toString(MsgType type, uint32_t nodeID, uint32_t ID, bool UI)
{
	msg = "" ;
	
	// assigning member data
	this->nodeID = nodeID;
	this->ID = ID;
	this->isUI = UI;
	this->msgType = type;
	
	// Adding the Node ID.
	char temp[32];
	sprintf(temp, "%d", nodeID); // convert to string
	msg+= std::string(temp)+ " ";
	
	//std::cout << msg <<std::endl;
	
	switch (type)
	{
		case PING:
			msg += msgStrings[PING];
			break;
			
		case STORE:
			if(ID != -1)
			{
				sprintf(temp, "%d", ID); // convert to string
				msg += msgStrings[STORE]+ std::string(temp);
			}
			break;
			
		case FINDNODE:
			if (ID != -1)
			{
				sprintf(temp, "%d", ID); // convert to string
				msg += msgStrings[FINDNODE]+ std::string(temp);
			}
			break;
			
		case FINDVALUE:
			if(ID != -1)
			{
				sprintf(temp, "%d", ID); // convert to string
				msg += msgStrings[FINDVALUE]+ std::string(temp);
				std::cout << msg <<std::endl;

			}
			break;
			
		case KCLOSEST:
			msg += msgStrings[KCLOSEST];
			
			// Adding the size of the array to the message
			sprintf(temp, "%d", size);
			msg += std::string(temp);
			
			/// iterate over all elements of kclosest and put them in the message.
			/// One line is of the form :
			/// IP   UDPPort   NodeID
			
			for (int i =0; i<size; ++i)
			{
				msg += "\n";
				
				// Adding the IP
				char temp [32];
				sprintf(temp, "%d", kclos[i].address); // convert to string
				msg += std::string (temp) + " ";
				
				// Adding the UDP Port
				sprintf(temp, "%d", kclos[i].port); // convert to string
				msg += std::string (temp)+" ";
				
				// Adding the Node ID
				sprintf(temp, "%d", kclos[i].node); // convert to string
				msg += std::string (temp);
			}
			msg += "\n";
			
			break;
			
		case PINGRESP:
			msg += msgStrings[PINGRESP];
			break;
		
		case FVRESP:
			msg += msgStrings[FVRESP];
			break;
			
		case STORERESP:
			msg += msgStrings[STORERESP];
			break;
			
		case FVRESPP:
			msg += msgStrings[FVRESPP];
			break;
			
		case FVRESPN:
			msg += msgStrings[FVRESPN];
			break;
		
		case NONE:
			msg="";
			break;
			
		default:
			msg = "";
			printf("Unable to create string properly!\n");
			break;
	}
	
	if(UI)
		msg += " UI";
	
	return msg;
}

std::string Message::toString()
{
	std::string m = "";
	
	m=toString(msgType, nodeID, ID, isUI);
	
	return m;
}

uint32_t Message::getID()
{
	return ID;
}

uint32_t Message::getNodeID()
{
	return nodeID;
}

void Message::setType(MsgType type)
{
	this->msgType = type;
}

void Message::setID(uint32_t id)
{
	this->ID= id;
}

void Message::setNodeID(uint32_t nodeID)
{
	this->nodeID= nodeID;
}

void Message::setUI(bool UI)
{
	this->isUI = UI;
}

void Message::setKClos ( Triple clos [K], uint32_t s)
{
	size = s;
	for (int i =0; i<s; ++i)
		
	{
		kclos[i]= clos [i];
	}
}

uint32_t Message::getKClos(Triple *clos)
{
	clos = kclos;
	return size;
	
}

bool Message::getUI()
{
	return isUI;
}

void Message::printMessageType()
{
	printf("Type: %s \n",  msgStrings[msgType].c_str());
}

bool Message::includes(uint32_t & id)
{
	bool found = false;
	for (int i=0; i<K && !found; ++i)
	{
		if(kclos[i].node == id)
			found = true;
	}
	
	return found;
}

