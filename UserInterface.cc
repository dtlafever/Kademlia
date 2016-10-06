#include "UserInterface.h"

#include <iostream>

using namespace std;

#define MAXCHAR 80

//default constructor
//PRE:
//POST: Command and givenID initialized to empty and zero.
//      isRunning is set to true until user inputs 'exit'.x
UserInterface::UserInterface(){
  command = "";
  givenID = 0;
  isRunning = true;
}


//PRE: Object defined
//POST: Run the UI where the user will have the option
//      to enter input
void UserInterface::runUI(){
  while(isRunning){
    cout << ">>";

    getline(cin, command);

    cout << "the command: " << command << endl;

    if(command == "exit"){
      isRunning = false;
    }
  }
}

//PRE: Object defined. Input is a string entered by user.
//POST: Parses through input to find any errors, and once
//      a specific command is validated, calls the
//      appropriate function to start creating the message.
void UserInterface::parseInput(string input){
  //stuff hereeeee
}

//PRE: User entered 'exit" as input.
//POST: Set isRunning to false, thus closing the UI.
void UserInterface::exit(){
  isRunning = false;
}
