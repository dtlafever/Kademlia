# Kademlia
peer 2 peer network, XOR metrix.

Routing Table requires the ability to ping an arbitary node, and recieve a
response.

Currently message class does not provide for this ability. Send message
returns a KBucket, always. I'm not quite sure why this is the case. Not all
response will be a KBucket.

Pretty much, I need to be able to ping a node and receive a response stating
that it is still alive.

Unless I require that the node passed to addNode in Routing Table is 100%
able to be inserted.

Also, comments are all on one line and they don't exactly make sense.

Questions
---------
- Should be just have a pointer for the least recently used item in a bucket (for pinging when bucket is full) and otherwise order the bucket by distance (which would make searching easier)?

KBucket.h Notes
---------------
- The file name should be "KBucket" with a capital "K"
- Why is the bucket array dynamic?
- What about cases where the bucket is full, what do we use then?
- We can't remove an triple from the array, why not instead have a bool determining whether or not the triple is in use or not?
- There is no #define for K, so how will the bucket know what that constant size is? Why not instead make the constructor take a parameter for an int k?
- Don't we need a function to get an item in the bucket?

RoutingTable.h Notes
--------------------
- The file name should be "RoutingTable" with a capital "R"
- I don't understand the comment for "NUMKBUCKETS". It also isn't being used so I am unsure of it's purpose. This should be in the Node class and passed to the routing table as a parameter to the constructor function
- The routing table should not have the ID of this node, the routing table does not have a ID, only the node does.
- "updateTable" needs to also have a parameter for the UDP Port number, not just address and ID.
- I am confused on the pre condition for "updateTable." Is it saying that the node ID must already exist in the table or just be a defined object?
- "findKBucket" should return just the index in the bucket array, not the KBucket pointer.
- "findDist" should take two parameters since we should not have the ID in our RoutingTable class.
- We are still missing functions for:
  - finding a node
  - constructor for the RoutingTable
 
UserInterface.h Notes
---------------------
- instead of "beginUI," should it be named "runUI"?
- for "storeKey" and "findKey," shouldn't the UI spawn a thread for send message?
-RESPONSE: The first note made sense so I changed that. Spawning a thread is a
           good idea too. I'll have to talk to Ingy to figure that out. --Kimi

Listener.h
----------
- the file name should be "Listener" with a capital "L"
- the functions should be camelCase
- should "startListenSocket" instead be in Socket class?
- should "sortMessage" spawn a thread for send message?

Message.h
---------
- Does the constructor do anything specific?
- for "parse," doesn't the parameter need a name? Why is it passed by reference?
- Why do you have a "createMessage" function when you have a class with a constructor that creates a message?
- SHouldn't the second "createMessage" be called "getMessage"?
