#include "UserInterface.h"

#include <iostream>
#include <string.h>

using namespace std;

//PRE: 
//POST: The user starts the User Interface and is asked to
//      input the IP address of the node it wants to speak to.
//      The UI object will be created and a UDP socket will be
//      connected to the given IP address. 
int main(){

  string givenIP;
  //ASSERT: IP for the node we want to talk to
  
  UserInterface UI = UserInterface();
  //ASSERT: create a UserInterface object for the user
  UI.runUI();

}
