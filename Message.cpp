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
	initializeKClos();
	parse(msg);
}

Message::Message(MsgType type, uint32_t ID, bool UI)
{
	initializeKClos();
	toString(type, ID, UI);
}

Message::Message(const Message & rhs) : msg(rhs.msg), msgType(rhs.msgType), ID(rhs.ID), isUI(rhs.isUI)
{
	initializeKClos();
	for (int i =0; i<K; ++i)
		this->Kclos[i] = rhs.Kclos[i];
}

Message::Message(): msgType(NONE)
{
	initializeKClos();
}

Message::~Message(){}

MsgType Message::getMsgType() const
{
	return msgType;
}

void Message::parse (std::string & message)
{
	int index = -1;
	
	msgType = NONE;
	
	if ((index=message.find("UI")!= -1))
	{
		isUI = true;
		message.erase(message.begin()+index, message.begin()+index+2);
	}
	
	if(msg == "")
		Message::msg = message;
	
	else if (message.find(msgStrings[PING]) != -1)
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
		message.erase(message.begin(), message.begin()+msgStrings[KCLOSEST].size());
		/// Iterate and insert in Kclos
		/// One line is of the form :
		/// IP   UDPPort   NodeID
		int ind = 0;
		std::string temp;
		for (Triple & i: Kclos)
		{
			ind = message.find(" ");
			
			if(ind == -1)
				throw std::invalid_argument("Error in parsing message invalid format");
			
			// IP
			temp = message.substr(0, ind);
			i.address = atoi(temp.c_str());
			
			message.erase(message.begin(), message.begin()+ind+1);
			
			ind = message.find(" ");
			
			if(ind == -1)
				throw std::invalid_argument("Error in parsing message invalid format");
			
			//UDPPort
			temp = message.substr(0, ind);
			i.port = atoi(temp.c_str());
			
			message.erase(message.begin(), message.begin()+ind+1);

			ind = message.find("\n");
			if(ind == -1)
				throw std::invalid_argument("Error in parsing message invalid format");
			
			// Node ID

			temp = message.substr(0, ind);
			i.node = atoi(temp.c_str());
			
			message.erase(message.begin(), message.begin()+ind+1);
		}
		
	}
	else if(message.find(msgStrings[PINGRESP]) != -1)
	{
		msgType = PINGRESP;
	}
	else if(message.find(msgStrings[FVRESP]) != -1)
	{
		msgType = FVRESP;
	}
	else if(message.find(msgStrings[STORERESP]) != -1)
	{
		msgType = STORERESP;
	}
	else if (message.find(msgStrings[FVRESPP])!= -1)
	{
		msgType = FVRESPP;
	}
	else if (message.find(msgStrings[FVRESPN])!= -1)
	{
		msgType = FVRESPN;
	}
	
	if(msgType == NONE)
		msg = "";
		
}

std::string Message::toString(MsgType type, uint32_t ID, bool UI)
{
		msg = "";
	
	switch (type)
	{
		case PING:
			msg = msgStrings[PING];
			break;
			
		case STORE:
			if(ID != -1)
			{
				char temp [32];
				sprintf(temp, "%d", ID); // convert to string
				msg = msgStrings[STORE]+ std::string(temp);
			}
			break;
			
		case FINDNODE:
			if (ID != -1)
			{
				char temp [32];
				sprintf(temp, "%d", ID); // convert to string
				msg = msgStrings[FINDNODE]+ std::string(temp);
			}
			break;
			
		case FINDVALUE:
			if(ID != -1)
			{
				char temp [32];
				sprintf(temp, "%d", ID); // convert to string
				msg = msgStrings[FINDVALUE]+ std::string(temp);
			}
			break;
			
		case KCLOSEST:
			msg = msgStrings[KCLOSEST];
			
			/// iterate over all elements of kclosest and put them in the message.
			/// One line is of the form :
			/// IP   UDPPort   NodeID
			
			for (Triple i : Kclos)
			{
				msg += "\n";
				
				// Adding the IP
				char temp [32];
				sprintf(temp, "%d", i.address); // convert to string
				msg += " ";
				
				// Adding the UDP Port
				sprintf(temp, "%d", i.port); // convert to string
				msg += " ";
				
				// Adding the Node ID
				sprintf(temp, "%d", i.node); // convert to string
			}
			
			break;
			
		case PINGRESP:
			msg = msgStrings[PINGRESP];
			break;
		
		case FVRESP:
			msg= msgStrings[FVRESP];
			break;
			
		case STORERESP:
			msg = msgStrings[FINDVALUE];
			break;
			
		case FVRESPP:
			msg= msgStrings[FVRESPP];
			break;
			
		case FVRESPN:
			msg = msgStrings[FINDVALUE];
			break;
			
		default:
			msg = "";
			std::cout << "Unable to create string properly!"<<std::endl;
			break;
	}
	
	if(UI)
		msg += " UI";
	
	return msg;
}

std::string Message::toString()
{
	std::string m = "";
	
	m=toString(msgType, ID, isUI);
	
	return m;
}

uint32_t Message::getID()
{
	return ID;
}

void Message::initializeKClos ()
{
	for (Triple & i : Kclos)
	{
		i.port= -1;
		i.node = -1;
		i.address = -1;
	}
}


void Message::setType(MsgType type)
{
	this->msgType = type;
}

void Message::setID(uint32_t id)
{
	this->ID= id;
}

void Message::setUI(bool UI)
{
	this->isUI = UI;
}

void Message::setKClos ( Triple clos [K])
{
	for (int i =0; i<K; ++i)
	{
		Kclos[i]= clos [i];
	}
}

void Message::getKClos(Triple *clos)
{
	clos = Kclos;
}

bool Message::getUI()
{
	return isUI;
}

