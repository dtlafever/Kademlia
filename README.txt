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
     	node's Routing table is full. All such nodes are still alive.
	A new node n attempts to join, it fails to join.
     7. Suppose that some node n is in the network. Further suppose that
     	n is dead. After a few moments n should not exist in any Routing Table.
     8. Suppose that for 0 <= i < 10 that node i exists within the network.
     	Let k be a key where k = 7. Suppose that node 5 is storing k.
	Then nodes 2, 3, 4, 6, and 7 contains k.
     9. Suppose that 10 nodes exist in the network as above. And suppose that
     	no nodes contains the key 4. Then node n calling find value for 4
	will fail.
     10. Suppose that 10 nodes exist in the network as above. And suppose that
     	 key 5 exists within the network. Node n calling for key 5 will succed
	 (we really don't care which node gives us the value).
     11. Suppose that nodes 0 and 1 exists in the network where node 1 is
     	 storing the key 5. Further suppose that node0 is trying to find the
	 value associated with key 5 and that during the process, node 1 dies.
	 The find value should fail.
     12. Suppose that node 0 exists in the network. And suppose that two nodes
     	 are trying to join the network at the same time with the same id
	 (for example, 1). One of the two nodes started first, call the first
	 one 1-A and the second one 1-B. 1-A succeeds in joining the network,
	 and 1-B fails in joining the network.
     13. Suppose node 0 is expecting a response from node 5. Node 5 dies,
     	 node 5 is ultimately removed from node 0's routing table.
     14. Suppose that node 3 has initalized FindNode to find node 11 when node
     	 11 does not exist in the network. Return a fail after querying every
	 node.
     15. 

Note: It would probably be best to use K = 2 as a test. 

For a network with 10 nodes (ie, ids are between 0 and 9), this is what
the network should look like:

After Node 0 joins: The Routing Table of Node 0 is empty
After Node 1 joins:
      Node0: RT[0] = 1
      Node1: RT[0] = 0
After Node2 joins:
      Node0: RT[0] = 1, RT[1] = 2
      Node1: RT[0] = 0, RT[1] = 2
      Node2: RT[1] = 0, 1 (order depends on which node responded back 1st)
After Node3 joins:
      Node0: RT[0] = 1, RT[1] = 2, 3
      Node1: RT[0] = 0, RT[1] = 2, 3
      Node2: RT[0] = 3, RT[1] = 0, 1
      Node3: RT[0] = 2, RT[1] = 0, 1
After Node4 joins:
      Node0: RT[0] = 1, RT[1] = 2, 3, RT[2] = 4
      Node1: RT[0] = 0, RT[1] = 2, 3, RT[2] = 4
      Node2: RT[0] = 3, RT[1] = 0, 1, RT[2] = 4
      Node3: RT[0] = 2, RT[1] = 0, 1, RT[2] = 4
      Node4: RT[2] = 0, 1, 2, 3 (depends on which ones responded first)
After Node5 joins:
      Node0: RT[0] = 1, RT[1] = 2, 3, RT[2] = 4, 5
      Node1: RT[0] = 0, RT[1] = 2, 3, RT[2] = 4, 5
      Node2: RT[0] = 3, RT[1] = 0, 1, RT[2] = 4, 5
      Node3: RT[0] = 2, RT[1] = 0, 1, RT[2] = 4, 5
      Node4: RT[0] = 5, RT[2] = 0, 1, 2, 3
      Node5: RT[0] = 4, RT[2] = 0, 1, 2, 3 (which one responded first)
After Node6 joins:
      Node0: RT[0] = 1, RT[1] = 2, 3, RT[2] = 4, 5, 6
      Node1: RT[0] = 0, RT[1] = 2, 3, RT[2] = 4, 5, 6
      Node2: RT[0] = 3, RT[1] = 0, 1, RT[2] = 4, 5, 6
      Node3: RT[0] = 2, RT[1] = 0, 1, RT[2] = 4, 5, 6
      Node4: RT[0] = 5, RT[1] = 6, RT[2] = 0, 1, 2, 3
      Node5: RT[0] = 4, RT[1] = 6, RT[2] = 0, 1, 2, 3
      Node6: RT[1] = 2, 3, RT[2] = 0, 1, 2, 3 (which one responded first)
After Node7 joins:
      Node0: RT[0] = 1, RT[1] = 2, 3, RT[2] = 4, 5, 6, 7
      Node1: RT[0] = 0, RT[1] = 2, 3, RT[2] = 4, 5, 6, 7
      Node2: RT[0] = 3, RT[1] = 0, 1, RT[2] = 4, 5, 6, 7
      Node3: RT[0] = 2, RT[1] = 0, 1, RT[2] = 4, 5, 6, 7
      Node4: RT[0] = 5, RT[1] = 6, 7, RT[2] = 0, 1, 2, 3
      Node5: RT[0] = 4, RT[1] = 6, 7, RT[2] = 0, 1, 2, 3
      Node6: RT[0] = 7, RT[1] = 2, 3, RT[2] = 0, 1, 2, 3
      Node7: RT[0] = 6, RT[1] = 3, 2, RT[2] = 0, 1, 2, 3 (which one 1st)

      blah blah blah, that whole pattern thing continues
