Kademlia:

An implementation of the peer to peer network that is based on
   the XOR metrix for distance.

Our implementation comes in two programs. One runs a node of the network,
    while the other runs the user interface. These are respectully called
    userInterface and node.

To make these two, just simply type make userInterface and node. When
   desired, remove all .o and the two programs with make clean.

NOTE: the node program expects either 1 or 3 arguments. The 1st is the node's
      id. It is assumed to be unique. The 2nd and 3rd arguments are an id
      and address describing an existing node in the network, these two are
      optional.

Test Cases:

     1. Node 0 joins the network with a contact. Node 0 fails to join
     2. Node 0 joins the network. Node 0 starts the network and has
     	an empty routing table.
     3. Node 0 exists within the network. Node 1 joins the network with
     	a contact that is not node 0. Node 1 fails to join the network.
	Node 0's routing table is still empty.
     4. Node 0 exists within the network. Node 1 joins the network with 0 as
     	a contact. Node 1 successfully joins with 0 in it's Routing Table. And
	Node 0 has node 1 in node 0's routing table.
     5. Node 0 and Node 1 exists within the network. A new node also known
     	as 0 joins. It fails to join.
     6. Suppose that enough nodes are in the network such that every
     	node's Routing table is full. 
