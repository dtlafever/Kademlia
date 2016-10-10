#include <string>
#include "UIconstants.h"

#ifndef include_UserInterface
#define include_UserInterface

class UserInterface{

 private:
  
  bool isRunning;
  //ASSERT: isRunning is a boolean that is a flag for the running
  //        of the User Interface. When false, the UI has stopped.
  
 public:

  //default constructor
  //PRE:
  //POST: Command and givenID initialized to empty and zero.
  //      isRunning is set to true until user inputs 'exit'.
  UserInterface();

  //PRE: Object defined
  //POST: Run the UI where the user will have the option to
  //      enter input.
  void runUI();

  //PRE: inputWords is a reference to a 2D array, numArguments is
  //     a reference to an integer.
  //POST: Tokenizes the given input into separate words and stores
  //      in inputWords. For each argument, numArguments is incremented.
  void getTokens(char commands[], char inputWords[MAXARGUMENTS][MAXCHAR],
		 int & numArguments);
  
  //PRE: Object defined. Input is a string entered by user.
  //POST: Parses through input to find any errors, and once
  //      a specific command is validated, calls the
  //      appropriate function to start creating the message.
  void parseInput(char command[]);

  //PRE: User entered 'exit" as input.
  //POST: Set isRunning to false, thus closing the UI.
  void exit();

  //PRE: The user entered STORE along with an unsigned integer key.
  //POST: From here, the UI will know that the Node wants to store
  //      the given key. The UI will create the correct Message and
  //      send that to the Listener.
  void storeKey(uint32_t ID);

  //PRE: The user entered FIND along with a key ID.
  //POST: From here, the UI will know that the Node wants to find
  //      the given key. The UI will create the correct Message and
  //      send that to the Listener.
  void findKey(uint32_t ID);

  //PRE: Message object created and ready to be sent. 
  //POST: Creates a socket to communicate with the Listener.
  void talkToListener();


};

#endif




