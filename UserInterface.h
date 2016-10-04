
#ifndef include_UserInterface
#define include_UserInterface

//PRE: The node has been fully integrated into the network
//     and is ready to start storing and finding keys.
//     isRunning is a boolean passed by reference to indicate
//     the interface is currently running.
//POST: The user will now have the option to enter input
//      into the user interface.
void beginUI(bool & isRunning);

//PRE: Input is a string entered in by the user
//POST: Parses through the input to extract words and/or
//      keys given. Error is expected to be thrown if:
//      -More than two words in input
//      -The second word not being a key
//      -The first word not matching EXIT, STORE or FIND.
void parseInput(string input);

//PRE: The user entered EXIT on the command prompt.
//POST: isRunning is set to false, which will end the
//      interface and close the thread.
void exit(bool & isRunning);

//PRE: The user entered STORE along with an unsigned integer key.
//POST: From here, the UI will know that the Node wants to store
//      the given key. The UI will pass this along to the Send Message
//      header functions. It is expected for the key to be stored
//      in the appropriate node.
void storeKey(unsigned int key);

//PRE: The user entered FIND along with an unsigned integer key.
//POST: From here, the UI will know that the Node is looking for
//      the given key. The UI will give this information, the command
//      and the key, to the Send Message header functions. It is
//      expected for the key to be searched and printed if found.
void findKey(unsigned int key);


#endif
