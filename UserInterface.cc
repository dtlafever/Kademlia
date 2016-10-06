#include "UserInterface.h"

#include <iostream>
#include <string.h>

using namespace std;

//default constructor
//PRE:
//POST: Command and givenID initialized to empty and zero.
//      isRunning is set to true.
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

    cin >> command;

    cout << "the command: " << command << endl;

    if(command == "exit"){
      isRunning = false;
    }
  }
}


//PRE: The node has been fully integrated into the network
//     and is ready to start storing and finding keys.
//     isRunning is a boolean passed by reference to indicate
//     the interface is currently running.
//POST: The user will now have the option to enter input
//      into the user interface.
// void runUI(bool & isRunning){
//   //while(isRunning){
//   try{

//     //run UI
    

//   }
//   catch(string ErrorMessage){
//     cout << ErrorMessage << endl;
//   }
//   //}
// }

