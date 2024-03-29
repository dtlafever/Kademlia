#include "UserInterface.h"

#include <iostream>
#include <string.h>

using namespace std;

//PRE: A network has been created. 
//POST: The user starts the User Interface and is asked to
//      input the IP address of the node it wants to speak to.
//      The UI object will be created and a UDP socket will be
//      connected to the given IP address. 
int main(){
  
  UserInterface UI = UserInterface();
  //ASSERT: create a UserInterface object for the user
  UI.runUI();

}
