Code was added to the addNode function in RoutingTable.h to handle
ping functionality. Please check over it to ensure that it was done
correctly, and/or a better way to write it

In UDPSocket.h, the function revMessage is declared void when the Post
condition says it returns 'size of that junk'. What is junk?

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

Give it the port we want to listen and send on. This function will then create the socket and bind it to the given port. It will throw a SocketException if there is an error.

**sendMessage(string s, string host, int port)**

Give it the message you want to send, the host ip, and host port to send it to. The function will then send a non blocking message to that host. It will throw a SocketException if there is an error sending.

**recvMessage(string s)**

Give it a buffer to hold the message. The function returns the length of the message and the stores the message in the string given. If -1 is returned then recv failed.

**getRemoteIP()**

Assumes that you have called recvMessage, it will get the most recent message's host IP.
