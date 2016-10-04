//
//  Message.cpp
//  Kademlia
//
//  Created by Ingy on 9/25/16.
//  Copyright © 2016 Roanoke. All rights reserved.
//

#include "Message.hpp"
#include <cstdlib>

Message::Message (std::string msg) : msgType(NONE), msg(msg)
{
	parse(msg);
}

Message::Message(): msgType(NONE)
{}

Message::~Message (){}

bool Message::isSTORE () const
{
	return (msgType==STORE);
}

bool Message::isPING () const
{
	return (msgType==PING);
}

bool Message::isFINDNODE() const
{
	return (msgType==FINDNODE);
}

bool Message::isFINDVALUE() const
{
	return (msgType==FINDVALUE);
}

void Message::parse (std::string & msg)
{
	int index = -1;
	
	msgType = NONE;
	
	if(msg == "")
		Message::msg = msg;
	
	if (msg.find("PING") != -1)
	{
		msgType = PING;
	}
	else if ((index = msg.find("STORE")) != -1)
	{
		msgType = STORE;
		
		msg.erase(msg.begin(), msg.begin()+6);
		
		index = msg.find(" ");
		
		std::string id = msg.substr(0, index);
		ID = atoi(id.c_str());
	}
	else if ( msg.find("FINDNODE")!= -1)
	{
		msgType = FINDNODE;
		
		msg.erase(msg.begin(), msg.begin()+9);
		
		ID = atoi (msg.c_str());
	}
	else if (msg.find("FINDVALUE")!= -1)
	{
		msgType = FINDVALUE;
		
		msg.erase(msg.begin(), msg.begin()+10);
		
		ID = atoi(msg.c_str());
	}
}

int Message::createMessage(MsgType type, uint32_t ID)
{
	int returnValue = 0;
	
	switch (type)
	{
		case PING:
			msg = "PING";
			break;
			
		case STORE:
			if(ID == -1)
				returnValue = -1;
			else
			{
				char temp [32];
				sprintf(temp, "%d", ID); // convert to string
				msg = "STORE "+ std::string(temp);
			}
			break;
			
		case FINDNODE:
			if (ID == -1)
				returnValue = -1;
			else
			{
				char temp [32];
				sprintf(temp, "%d", ID); // convert to string
				msg = "FINDNODE "+ std::string(temp);
			}
			break;
			
		case FINDVALUE:
			if(ID == -1)
				returnValue = -1;
			{
				char temp [32];
				sprintf(temp, "%d", ID); // convert to string
				msg = "FINDVALUE "+ std::string(temp);
			}
			break;
			
		default:
			returnValue = -1;
			break;
	}
	
	return returnValue;
}

std::string Message::createMessage()
{
	std::string m = "";
	
	if(!createMessage(msgType, ID))
		m = msg;
	
	return m;
}

uint32_t Message::getID()
{
	return ID;
}

