#include "UserInterface.h"
#include "UIconstants.h"
#include "UIerror.h"
#include "Message.hpp"
#include "UDPSocket.h"
#include "constants.h"

#include <stdlib.h>
#include <string.h>

#include <iostream>

using namespace std;


//default constructor
//PRE: Main.cc is running
//POST: Command and givenID initialized to empty and zero.
//      isRunning is set to true until user inputs 'exit'
UserInterface::UserInterface(){
  isRunning = true;
}


//PRE: Object defined
//POST: Run the UI where the user will have the option
//      to enter input
void UserInterface::runUI(){

  int recNum;
  UDPSocket UIsocket(UIPORT);

  string newMessage;
  string receivedMessage;

  char command[MAXCHAR];
  
  string input;
  while(isRunning){
    try{
      cout << "> ";
      getline(cin, input);
      strcpy(command, input.c_str());
      
      newMessage = parseInput(command);
      cout << "sendMessage: " << newMessage << endl;

      if(isRunning){
	UIsocket.sendMessage(newMessage, "127.0.0.1", UDPPORT);
	
	recNum = -1;
	while(recNum < 0){
	  cout << "Listening" << endl;
	  recNum = UIsocket.recvMessage(receivedMessage);
	}

	//handleMessage(receivedMessage);
      }

      
    }
    catch(UIerror & error){
      error.print_error();
    }
  }
}

//PRE: inputWords is a reference to a 2D array, numArguments is
//     a reference to an integer.
//POST: Tokenizes the given input into separate words and stores
//      in inputWords. For each argument, numArguments is incremented.
void UserInterface::getTokens(char commands[], char inputWords[MAXARGUMENTS][MAXCHAR],
			      int & numArguments){
  char * token;
  token = strtok(commands, " ");
  while(token != NULL){
    //ASSERT: not all tokens have been extracted
    strcpy(inputWords[numArguments], token);
    token = strtok(NULL, " ");
    numArguments = numArguments + 1;
    //ASSERT: increment each argument as it comes in
  }
}


//PRE: Object defined. Input is a string entered by user.
//POST: Parses through input to find any errors, and once
//      a specific command is validated, creates the
//      Message object/string and returns it.
string UserInterface::parseInput(char command[]){
  char inputWords[MAXARGUMENTS][MAXCHAR];
  int numArguments = 0;
  getTokens(command, inputWords, numArguments);

  string finalRequest;
  try{
    if(numArguments == MAXARGUMENTS){
      uint32_t givenID = atoi(inputWords[IDPOS]);
      //ASSERT: user entered two arguments.
      if(strcmp(inputWords[COMMANDPOS], allCommands[STOREPOS]) == 0){
	finalRequest = storeKey(givenID);
      }
      else if(strcmp(inputWords[COMMANDPOS], allCommands[FINDPOS]) == 0){
	finalRequest = findKey(givenID);
      }
      else{
	//ASSERT: the input didn't match the correct commands
	throw(UIerror((char *)"Error: Incorrect input."));
      }
    }
    else if(numArguments == MINARGUMENTS){
      //ASSERT: user entered one argument
      if (strcmp(inputWords[COMMANDPOS], allCommands[EXITPOS]) == 0){
	cout << "exit" << endl;
	exit();
      }
      else{
	//ASSERT: user did not input "exit"
	throw(UIerror((char *) "Error: Incorrect input."));
      }
    }
    else{
      //ASSERT: too many or too little arguments given
      throw(UIerror((char *) "Error: Incorrect number of arguments."));
    }
  }
  catch (UIerror & error){
    throw(error);
  }
  
  return(finalRequest);
}

//PRE: User entered 'exit' as input.
//POST: Set isRunning to false, thus closing the UI.
void UserInterface::exit(){
  isRunning = false;
}

//PRE: The user entered STORE along with an unsigned integer key.
//POST: From here, the UI will know that the Node wants to store
//      the given key. The UI will create the correct Message and
//      return the string.
string UserInterface::storeKey(uint32_t ID){
  MsgType type = STORE;
  Message storeMessage(type, ID, true);
  string store = storeMessage.toString();

  return(store);
}

//PRE: The user entered FIND along with an unsigned integer key.
//POST: From here, the UI will know that the Node wants to store
//      the given key. The UI will create the correct Message and
//      return the string.
string UserInterface::findKey(uint32_t ID){
  MsgType type = FINDVALUE;
  Message findMessage(type, ID, true);
  string find = findMessage.toString();

  return(find);
}


//PRE: newMsg is a string received from the Listener socket
//POST: Parses through the string using the Message class
//      to determine whether our request was a success
//      or failure.
void UserInterface::handleMessage(string newMsg){
  Message receivedMsg(newMsg);
  if(receivedMsg.getMsgType() == STORERESP){
    //ASSERT: Store was successful
    cout << "Success!" << endl;
  }
  else if(receivedMsg.getMsgType() == FVRESPP){
    //ASSERT: Key was found
    cout << "Success!" << endl;
  }
  else if(receivedMsg.getMsgType() == FVRESPN){
    //ASSERT: Key was not found
    cout << "Failure." << endl;
  }
}
