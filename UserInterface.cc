#include "UserInterface.h"
#include "constants.h"
#include "ErrorClass.h"
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
  try{
    while(isRunning){
      cout << "> ";
      getline(cin, input);
      strcpy(command, input.c_str());
      
      parseInput(command);
    }
  }
  catch (ErrorClass & error){
    error.print_error();
  }
}

//PRE: Object defined. Input is a string entered by user.
//POST: Parses through input to find any errors, and once
//      a specific command is validated, calls the
//      appropriate function to start creating the message.
void UserInterface::parseInput(char command[]){
  char inputWords[MAXARGUMENTS][MAXCHAR];
  int numArguments = 0;
  char * token;
  token = strtok(command, " ");
  while(token != NULL){
    strcpy(inputWords[numArguments], token);
    token = strtok(NULL, " ");
    numArguments = numArguments + 1;
  }

  try{
    if(numArguments == MAXARGUMENTS){
      if(strcmp(inputWords[COMMANDPOS], allCommands[STOREPOS]) == 0){
	cout << "You wish to store." << endl;
      }
      else if(strcmp(inputWords[COMMANDPOS], allCommands[FINDPOS]) == 0){
	cout << "You wish to find." << endl;
      }
      else{
	throw(ErrorClass((char *)"Error: Incorrect input."));
      }
    }
    else if(numArguments == MINARGUMENTS){
      if (strcmp(inputWords[COMMANDPOS], allCommands[EXITPOS]) == 0){
	exit();
      }
      else{
	throw(ErrorClass((char *) "Error: Incorrect input."));
      }
    }
    else{
      throw(ErrorClass((char *) "Error: Incorrect number of arguments."));
    }
  }
  catch (ErrorClass & error){
    throw(error);
  }
  
}

//PRE: User entered 'exit" as input.
//POST: Set isRunning to false, thus closing the UI.
void UserInterface::exit(){
  isRunning = false;
}

//PRE: The user entered STORE along with an unsigned integer key.
//POST: From here, the UI will know that the Node wants to store
//      the given key. The UI will create the correct Message and
//      send that to the Listener.
void UserInterface::storeKey(unsigned int key){
  //store key
}
