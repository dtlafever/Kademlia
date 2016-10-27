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

## UDPSocket Usage
**UDPSocket(int port)**

INPUT: a port number we want to listen and send on.

OUTPUT:

SIDE EFFECTS: creates the socket and binds the socket to the given port. Throws a SocketException if there is an error.


**void sendMessage(string s, string host, int port)**

INPUT: the message we want to send, the host ip and port we want to send our message to

OUTPUT: 

SIDE EFFECTS: sends a UDP message to that host. Throws a SocketException if there is an error.


**int recvMessage(string s)**

INPUT: a buffer to hold our message

OUTPUT: returns the length of the message (in bytes). Returns -1 if recv failed.

SIDE EFFECTS: stores the message in the buffer.


**int getRemoteIP()**

INPUT: None, but assumes that you ahve called recvMessage and it returned a value > 0

OUTPUT: returns the IP address (integer) of the most recent message

SIDE EFFECTS: 


**string getRemoteIP()**

INPUT: None, but assumes that you ahve called recvMessage and it returned a value > 0

OUTPUT: returns the IP address (string) of the most recent message

SIDE EFFECTS: 
