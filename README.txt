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








KIMI Corner

=============================================
Suppose as the UI we try to contact a Node,
but it died and was removed from the network
=============================================

The UI will send any type of request to the Node it is connected to.
There is a predefined point in time where the UI needs to give up waiting for a response.

If the predefined timeout arrives and there still is no response, the UI will print
to the user that the connection has timed out and ended.

The UI will also give the user the option to enter a new IP address to connect to.

=============================================
Suppose there is onle 1 node in the network

Node A
=============================================

1.STORE

From the UI we say:
>store 10

-The UI will be connected to Node A and so A will receive the request to store key.
-A will first attempt to find the K closest nodes by calculating the distance between itself
 and the given key.
-Since A is the only node in the network, there will be no K closest nodes returned.

A will store the given key in itself.

*******************************************

2. FIND (if we didn't STORE anything)

From the UI the only thing we say is:
>find 10

-Node A will receive the FIND request from the UI.
-A will first see if it has the given key. It does not.
-A will then try to find the K closest nodes by calcualting the distance between itself
 and the given key and send ALPHA FIND requests to those nodes.
-Since A is the only node in the network, there will be no K closest nodes returned.
-The lookup will terminate because there are no other nodes to query.

Node A will send a "Failure" message to the UI.

The UI will print out "Failure" to the user.

********************************************

3. FIND (if we actually did STORE something)

From the UI we say:
>store 10
>find 10

-Since A is the only node in the network, when the UI sent the STORE request, Node A stored
 the key in itself.
-The first step A will do is check and see if it has the given key. It does! So we do not
 need to perform a node lookup.

Node A will send a "Success" message to the UI.

The UI will print out "Success" to the user.

*********************************************************
============================================
Suppose we have Node B and we want to join the network

Node B (ID = 4) uses Node A (ID = 3) as it's contact
============================================

-Node B must have a contact to join. It provides the ID of Node A
-Node B calculates the distance between itself and Node A.
-Node B places Node A into the appropriate K-bucket. (Dist = 7, so the third bucket)

-Node B must now populate it and the rest of the network's K-buckets.
-Node B sends FINDNODE request to Node A.
-Node A receives a FINDNODE request with Node B's ID.
-Node A calculate the distance between itself and Node B. Dist = 7, so it goes to look
 in it's 3rd K-bucket. Nothing will be in there, so it'll search through the rest of the
 K-buckets. Since Node A is the only one in the network, it'll return an empty list to Node B
-Node A will then place Node B in the appropriate K-bucket (Dist =7, so the third bucket)
-Node B will receive an empty list. The lookup will terminate since there are no more nodes
 to query.
-Node B will refresh it's routing table since it got a response from Node A.

Node B is now successfully in the network!


*********************************************************

=============================================
Suppose there are only 2 nodes in the network

Node A (ID = 3) , Node B (ID = 4)

3 XOR 4 = 7

Node A is storing Node B in it's third K-bucket.
Node B is storing Node A in it's third K-bucket.
=============================================

1. STORE

From the UI we say:
>store 5

-The UI will be connected to Node A and so A will receive the request to store A.
-A calculate the distance from itself and the given key. 3 XOR 5 = 6.
-Since the distance is 6 we go to Node A's third K-bucket. We then choose ALPHA amount
 of nodes to add to our KClosest Nodes and then send KCLOSEST requests to to get more nodes.
-Since there is only one node in there, Node A will add Node B to it's list of KClosest Nodes.
-Then A will send B a KCLOSEST Request.
-Node B will receive a KCLOSEST request from A.
-It will calculate the distance from itself and the given key. 4 XOR 5 = 1
-Our distance is 1, so we go to Node B's first K-bucket. No nodes are in there. Move to the
 second K-bucket. No nodes are in there.
-Move to the third K-bucket. Node B will find Node A, add it to it's KClosest, and try to look
 for other nodes to it's list. A is the only other node in the network, so A will be the only
 one sent back.
-A will then receive itself as a closest node. The lookup will terminate since all nodes have been
 queried. Node A will then send a STORE request to both ITSELF and to Node B.

Node A and Node B will receive a STORE request, and since it received this from a fellow Node
and not the UI, then it will simply push the given Key(5) into it's list of keys. 

**********************************

2. FIND (with the UI connected to Node A - assuming we have stored a key first)

From the UI we say:
>find 5

-The UI will be connected to Node A, and so A will receive the request to find the Key.
-The first thing A will do is check itself for the key.
-There are only two nodes in the network, and due to the previous STORE test case, both
 nodes should now have the key.
-Node A will find the given Key inside it's list and so it won't have to send
 further requests.

Node A will send a "success" message to the UI.

The UI will print a "success" message to the user.

***********************************

3. Find (with the UI connected to Node B - assuming we have stored a key first)

From the UI we say:
>find 5

-The UI will be connected to Node B, and so B will receive the request to find the Key.
-The first thing B will do is check itself for the key.
-There are only two nodes in the network, and due to the previous STORE test case, both
 nodes should now have the key.
-Node B will find the given Key inside it's list so it won't have to send
 further requests.

Node B will send a "success" message to the UI.

The UI will print a "success" message to the user.

***************************************************************

========================================
Suppose there are 3 nodes in the network.
Suppose ALPHA = 3 and K = 5

Node A (ID = 3) and Node B (ID = 4) are in the network.

Node C (ID = 2) wants to join the network and uses Node A as a contact.
========================================

-Node C must have a contact to join. It provides the ID of Node A
-Node C calculates the distance between itself and Node A.
-Node C places Node A into the appropriate K-bucket. (Dist = 1, so the first bucket)

-Node C must now populate it and the rest of the network's K-buckets.
-Node C sends FINDNODE request to Node A.
-Node A receives a FINDNODE request with Node C's ID.
-Node A calculate the distance between itself and Node C. Dist = 1, so it goes to look
 in it's first K-bucket. Nothing will be in there, so it'll search through the rest of the
 K-buckets. Node A will reach the third K-bucket and find Node B and add it to the KClosest
 list. It'll look through the rest, not find any, and send back Node B to Node C.
-Node A will then place Node C in the appropriate K-bucket (Dist = 1, so the first bucket)
-Node C will receive a list of just one node. It'll select ALPHA nodes (or as much as we can,
 in this case just one) and then resend the FINDNODE request to Node B with Node C's ID.
-Node C will refresh it's K-buckets so Node A is LRU in it's bucket.
 
-Node B receives a FINDNODE request with Node C's ID.
-Node B calculates the distance between itself and Node C. Dist = 6, so it goes to look in
 it's third k-bucket and finds Node A. It won't find any other nodes, so it will only return
 Node A to Node C.
-Node B will then place Node C in the appropriat K-bucket (Dist = 6, the third bucket) 

-Node C will receive a list of just one node. It's already queried this node, and there
 are no other nodes to query; therefore the lookup will terminate.
-Node C will refresh it's K-buckets so Node B is LRU in it's bucket.

Node C is now in the network!

========================================

=======

Suppose there are N nodes in the network, such that N < 2^32.

A node wants to join the network, and has a contact.

=======

-The joining node J must have a contact in order to join. It provides the ID of the Contact Node.
-JoinNode gets D = distance(JoinNode, ConNode), where 0 < D < (2^32) - 1
-JoinNode places ConNode into the tail of the appropriate K-bucket, where 2^i <= D < 2^i+1

-JoinNode must populate itself and the rest of the network's K-buckets.
-JoinNode sends FINDNODE request to ConNode.
-ConNode receives a FINDNODE request with JoinNode's ID.

-ConNode calculates the distance between itself and JoinNode.
   It goes to that K-bucket, and collects up to K of the closest nodes to the ID using the XOR metric
   If the K-bucket is not full, compare it to the distance of buckets 2^i-1 and 2^i+1. Go to the bucket
   with the closer distance until you have K nodes.
   Once it has K nodes (or as many as it can get) it will send the list back to JoinNode.
-ConNode will then place JoinNode in the right K-bucket.

-JoinNode will receive a list of the K closest nodes given it's ID. 


















