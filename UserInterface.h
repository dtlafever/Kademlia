#include <string>


#ifndef include_UserInterface
#define include_UserInterface

class UserInterface{

 private:
  
  bool isRunning;
  
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

  //PRE: Object defined. Input is a string entered by user.
  //POST: Parses through input to find any errors, and once
  //      a specific command is validated, calls the
  //      appropriate function to start creating the message.
  void parseInput(char command[]);

  //PRE: User entered 'exit" as input.
  //POST: Set isRunning to false, thus closing the UI.
  void exit();

  //PRE: The user entered STORE along with a key ID.
  //POST: From here, the UI will know that the Node wants to store
  //      the given key. The UI will create the correct Message.
  void storeKey(unsigned int ID);

  //PRE: The user entered FIND along with a key ID.
  //POST: From here, the UI will know that the Node wants to find
  //      the given key. The UI will create the correct Message.
  //void findKey(unsigned int ID);

  //PRE: Message object created and ready to be sent. 
  //POST: Creates a socket to communicate with the Listener.
  //void sendtoListener(Message msg);

};

#endif



//PRE: The user entered STORE along with an unsigned integer key.
//POST: From here, the UI will know that the Node wants to store
//      the given key.
//void storeKey(unsigned int key);

//PRE: The user entered FIND along with an unsigned integer key.
//POST: From here, the UI will know that the Node is looking for
//      the given key. 
//void findKey(unsigned int key);


