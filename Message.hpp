#ifndef Message_cpp
#define Message_cpp
#include <string>
#include <stdio.h>
#include "constants.h"

// Enumerated data type describing the type of message. Accessible outside the class.
// ========================================================
// PING 		: Message sent to a node we want to ping.
// STORE 		: Message sent to a node that should store some value.
// FINDNODE : Message sent to a node that should return its K closest nodes.
// FINDVALUE: Message sent to a node to ask if that node has the desired value.
// KCLOSEST : Message the a node may return in case it fails to find a value or respoinding to a findnode request.
// PINGRESP : response to a ping request. (Acknowledging we are alive)
// FVRESP   : Positive response to a findvalue.
// STORERESP: Acknowledgement of the completion of the UI's STORE request.
// FVRESPP	: Positive response to the UI for a FINDVALUE request.
// FVRESPN	: Negative response to the UI for a FINDVALUE request.
// ========================================================
enum MsgType { PING, FINDNODE, FINDVALUE, STORE, KCLOSEST, PINGRESP, FVRESP, STORERESP, FVRESPP, FVRESPN, NONE};

// IMPORTANT: This class assumes that anything passed to it is valid. It will not check if the contents of the KClosest array are garbage or not, if the string itself is manipulated when sending to the parse function there is no guarantee how the message will be parsed.

class Message
{
  // Contents of the message
  std::string msg = "";
	
  // NodeID of sender
  uint32_t nodeID = -1;
	
  // Type of the message
  MsgType msgType;
	
  // ID represents the NodeID we're looking up or the key to the file
  uint32_t ID =-1;
	
  // This boolean flag is set to true when the message came from the User Interface.
  bool isUI = false;
	
  // Represents the KClosest Array
  Triple kclos [K];
	
  // Number of valid elements in KClos
  uint32_t size =0;
	
  // This array stores the names of the different types to simplify the parsing and string creation
  const std::string msgStrings [11] = {"PING ", "FINDNODE ",  "FINDVALUE ", "STORE ", "KCLOSEST ", "PINGRESP", "FVRESP", "STORERESP", "FVRESPP", "FVRESPN",  "NONE "};
	
	
	// PRE: Takes a string that contains the KClosest array to be parsed and put in the internal data member (KClos []). This function should only be called by the parse function.
	// POST: The function parses the string to retrieve each element (Node IP, UDP Port, and Node ID)
	void parseKClosest (std::string & message);
	
	// PRE: This function is called by the toString functions. It creates the string for the KCLOSEST message type
	// POST: The function takes each triple (Node IP, UDP Port, and Node ID) and converts them to strings before adding them to the string msg.
	void createKClosestMessage();

public:

  // This function prints the type of the current Message object, used for testing.
	void printMessageType ();
  
  // Default constructor
  Message ();
	
  // PRE: Takes the message as a parameter
  // POST: Constructor that will call the parse function
  Message (std::string msg);
	
  // PRE: Takes the type of the message and the ID of the sender at least. The ID is dependent on the type of message: STORE, FINDNODE, FINDVALUE are the messages that need an ID. The UI boolean is by default set to false and is set to true only by the UserInterface program.
  // POST: This Constructor initializes the internal members and calls to string to verify.
  Message(MsgType type, uint32_t nodeID, uint32_t ID = -1, bool UI = false);
	
  // Copy constructor
  Message (const  Message & rhs);
	
  // Destructor
  ~Message();

  // This function is to help checking for the type of the Message
  MsgType getMsgType() const;
	
  // PRE: takes the message received as a string.
  // POST: This function parses the message and updates the appropriate private members. This function alters the string and
  void parse (std::string );
	
  // PRE: takes a MsgType object and an ID which depends on the type of the message. This function should only be used when the Message object was initialized previously with at least the nodeID of the sender and the message type.
  // POST: Creates a message to be able to send it in the appropriate format.
  std::string toString (MsgType type, uint32_t nodeID, uint32_t ID = -1, bool UI=false);
	
  // PRE: this function is to be used if the Message object is already created and we want to retrieve the string format.
  // POST: This function creates a string relative to its attributes that can be used when communicating. The function will return an empty string if it fails.
  std::string toString ();
	
  // PRE:
  // POST: returns the Node of File ID.
  uint32_t getID();
	
  // PRE:
  // POST: Returns the ID of the sender node.
  uint32_t getNodeID();
	
  // PRE:
  // POST: returns true if the Message is from the UI and false if it is not.
  bool getUI();
	
  // PRE: this function takes a MsgType to set to.
  // POST:  This function does not check the types, it assumes it's taking the right input and just sets the internal message type.
  void setType(MsgType type);
	
  // PRE: this function takes the ID we are searching for (for Message objects of type : STORE, FINDNODE and FINDVALUE).
  // POST: Sets the internal member ID. This function assumes the ID is valid.
  void setID(uint32_t id);
	
  // PRE: This function takes the nodeID we want the Message object to keep as a senderID.
  // POST: The function sets the internal member NodeID to the id that is passed. The function assumes it is taking the right input for the senderID.
  void setNodeID(uint32_t id);
	
  // PRE:takes a boolean to set the Message as a UI message or change it to non UI
  // POST: Because the Message object is by default non UI, only the UserInterface program should use this function.
  void setUI(bool UI);
	
  // PRE: Takes the array of triples to put in the message and the size of the array
  // POST: the function sets the internal member KClos with the array, the function does not do any type of checking.
  void setKClos (Triple clos [K], uint32_t s);
	
  // PRE: takes an array of triple to updated by reference.
  // POST: returns the size of the array and updates the clos array passed by reference
  uint32_t getKClos (Triple clos[K]);
	
	// PRE: This function only applies to Message objects of type KCLOSEST. This function does not check if the message type is correct and it does not check that the id passed is valid.
	// POST:It checks if a nodeID is present in the array returned in the message. It returns true only if the nodeID we are checking for is found.
  bool includes(uint32_t & id);

};
#endif /* Message_cpp */
