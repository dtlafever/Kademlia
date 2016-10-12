#include "UserInterface.h"
#include "UIconstants.h"
#include "UIerror.h"
#include "Message.hpp"
#include "UDPSocket.h"

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
  char command[MAXCHAR];
  string input;
  while(isRunning){
    try{
      cout << "> ";
      getline(cin, input);
      strcpy(command, input.c_str());
      
      parseInput(command);
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
//      a specific command is validated, calls the
//      appropriate function to start creating the message.
void UserInterface::parseInput(char command[]){
  char inputWords[MAXARGUMENTS][MAXCHAR];
  int numArguments = 0;
  getTokens(command, inputWords, numArguments);
  try{
    if(numArguments == MAXARGUMENTS){
      uint32_t givenID = atoi(inputWords[IDPOS]);
      //ASSERT: user entered two arguments.
      if(strcmp(inputWords[COMMANDPOS], allCommands[STOREPOS]) == 0){
	cout << "You wish to store." << endl;
	//storeKey(givenID);
      }
      else if(strcmp(inputWords[COMMANDPOS], allCommands[FINDPOS]) == 0){
	cout << "You wish to find." << endl;
	//findKey(givenID);
      }
      else{
	//ASSERT: the input didn't match the correct commands
	throw(UIerror((char *)"Error: Incorrect input."));
      }
    }
    else if(numArguments == MINARGUMENTS){
      //ASSERT: user entered one argument
      if (strcmp(inputWords[COMMANDPOS], allCommands[EXITPOS]) == 0){
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
  
}

//PRE: User entered 'exit' as input.
//POST: Set isRunning to false, thus closing the UI.
void UserInterface::exit(){
  isRunning = false;
}

//PRE: The user entered 'store' along with a key ID.
//POST: From here, the UI will know that the Node wants to store
//      the given key. The UI will create the message.
void UserInterface::storeKey(uint32_t ID){
  MsgType type = STORE;
  Message storeMessage(type, ID, true);
  string store = storeMessage.toString();
  //talkToListener(store);
}

//PRE: The user entered 'find' along with a key ID.
//POST: From here, the UI will know that the Node wants to find
//      the given key. The UI will create the message.
void UserInterface::findKey(uint32_t ID){
  MsgType type = FINDVALUE;
  Message findMessage(type, ID, true);
  string find = findMessage.toString();

  //talkToListener(find);
}


//PRE: Message object created and ready to be sent. 
//POST: Creates a socket to communicate with the Listener.
void UserInterface::talkToListener(){
  //Create
  //Bind
  //Send
  //Listen

  //Handle what is being sent back
  //Just expecting "success" or "failure" messages.
}

