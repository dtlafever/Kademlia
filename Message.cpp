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
	parse(msg);
}

Message::Message(): msgType(NONE)
{}

Message::~Message(){}

MsgType Message::getMsgType() const
{
	return msgType;
}

void Message::parse (std::string & msg)
{
	int index = -1;
	
	msgType = NONE;
	
	if ((index=msg.find("UI")!= -1))
	{
		isUI = true;
		msg.erase(msg.begin()+index, msg.begin()+index+2);
	}
	
	if(msg == "")
		Message::msg = msg;
	
	else if (msg.find(msgStrings[PING]) != -1)
	{
		msgType = PING;
	}
	else if ((index = msg.find(msgStrings[STORE])) != -1)
	{
		msgType = STORE;
		
		msg.erase(msg.begin(), msg.begin()+msgStrings[STORE].size());
		
		index = msg.find(" ");
		
		std::string id = msg.substr(0, index);
		ID = atoi(id.c_str());
	}
	else if ( msg.find(msgStrings[FINDNODE])!= -1)
	{
		msgType = FINDNODE;
		
		msg.erase(msg.begin(), msg.begin()+msgStrings[FINDNODE].size());
		
		ID = atoi (msg.c_str());
	}
	else if (msg.find(msgStrings[FINDVALUE])!= -1)
	{
		msgType = FINDVALUE;
		
		msg.erase(msg.begin(), msg.begin()+msgStrings[FINDVALUE].size());
		
		ID = atoi(msg.c_str());
	}
	else if (msg.find(msgStrings[KCLOSEST])!= -1)
	{
		msg.erase(msg.begin(), msg.begin()+msgStrings[KCLOSEST].size());
		/// Iterate and insert in Kclos
		/// One line is of the form :
		/// IP   UDPPort   NodeID
		
	}
	else if(msg.find(msgStrings[PINGRESP]) != -1)
	{
		msgType = PINGRESP;
	}
	else if(msg.find(msgStrings[FVRESP]) != -1)
	{
		msgType = FVRESP;
	}
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
				sprintf(temp, "%d", i.ip); // convert to string
				msg +=
			}
			
			break;
			
		case PINGRESP:
			msg = msgStrings[PINGRESP];
			break;
		
		case FVRESP:
			msg= msgStrings[FVRESP];
			break;
			
		default:
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

