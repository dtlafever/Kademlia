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

enum MsgType { PING, STORE, FINDNODE, FINDVALUE, NONE};

class Message
{
	std::string msg = "";
	MsgType msgType;
	uint32_t ID =-1;
	std::string path= "";
	
public:
	
	Message (std::string msg);
	Message ();
	~Message ();

	bool isSTORE ();
	bool isPING ();
	bool isFINDNODE();
	bool isFINDVALUE();
	
	void parse (std::string &);
	
	int createMessage (MsgType type, uint32_t ID = -1, std::string path = "");
	
	uint32_t getID();
	std::string getPath();
};
#endif /* Message_cpp */
