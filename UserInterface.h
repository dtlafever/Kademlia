#include <string>
#include <chrono>
#include "UIconstants.h"

#ifndef include_UserInterface
#define include_UserInterface

using namespace std;

class UserInterface{

 private:
  
  bool isRunning;
  //ASSERT: isRunning is a boolean that is a flag for the running
  //        of the User Interface. When false, the UI has stopped.

  string IPaddress;
  //ASSERT: IPaddress is the IP of the node we want to connect to.

  chrono::system_clock::time_point timeStamp;
  //ASSERT: timeStamp is the point in time when a request is
  //        generated and sent
  
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
  //      a specific command is validated, creates the
  //      Message object/string and returns it.
  string parseInput(char command[]);

  //PRE: User entered 'exit" as input.
  //POST: Set isRunning to false, thus closing the UI.
  void exit();

  //PRE: The user entered STORE along with an unsigned integer key.
  //POST: From here, the UI will know that the Node wants to store
  //      the given key. The UI will create the correct Message and
  //      return the string.
  string storeKey(uint32_t ID);

  //PRE: The user entered FIND along with a key ID.
  //POST: From here, the UI will know that the Node wants to find
  //      the given key. The UI will create the correct Message and
  //      return the string.
  string findKey(uint32_t ID);

  //PRE: newMsg is a string received from the Listener socket
  //POST: Parses through the string using the Message class
  //      to determine whether our request was a success
  //      or failure.  
  void handleMessage(string newMsg);

  //PRE: Object defined
  //POST: Computes the point in time where the message we send
  //      will time out. The current point in time is compared to
  //      the timeout stamp. If current < timeout, RV is false.
  //      Else RV is true.
  bool checkTimeout();
};

#endif




