Kademlia:

An implementation of the peer to peer network that is based on
   the XOR metrix for distance.

Our implementation comes in two programs. One runs a node of the network,
    while the other runs the user interface. These are respectully called
    UserInterface and node.

To make these two, just simply type make userInterface and node. When
   desired, remove all .o and the two programs with make clean.

NOTE: the node program expects either 1 or 3 arguments. The 1st is the node's
      id. It is assumed to be unique. The 2nd and 3rd arguments are an id
      and address describing an existing node in the network, these two are
      optional.

Test Cases:

--JOINING A NETWORK--

     =. Node 0 joins the network with a contact. Node 0 fails to join
     	-the contact that you specify doesn't exist
	     -prints out "failed to join network" and quits

     =. Node 0 joins the network. Node 0 starts the network and has
     	an empty routing table.
	-just one node, by itself
	      -prints "joined the network"
	      
     =. Node 0 exists within the network. Node 1 joins the network with 0 as
     	a contact. Node 1 successfully joins with 0 in it's Routing Table. And
	Node 0 has node 1 in node 0's routing table.

     =. Suppose that enough nodes are in the network such that every
     	node's Routing table is full. All such nodes are still alive.
	A new node n attempts to join, it fails to join.
	  -the node will stil join the network, but the only k-buckets being filled
	   are it's own. since the other RTs are full, the node will be placed in others.	 


ADDING SECOND NODE
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



ADDING THIRD NODE 
========================================
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


GENERAL CASE
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
-ConNode will then place JoinNode in the right K-bucket of it's routing table.

-JoinNode will receive a list of the K closest nodes given it's ID.
  JoinNode will continuously send FINDNODE request with JoinNode's ID,
  It will keep track of every node it has queried
  Eventually, either JoinNode will find itself, or there is nobody left to send
  FINDNODE requests to.

-JoinNode will stop sending requests and terminate the lookup since it has successfully joined the
 network.


==================================================================================================

===========COMMUNICATING BETWEEN NODES=============

     =. Suppose that for 0 <= i < 10 that node i exists within the network.
     	Suppose that K = 5.
	Let key = 7. Suppose that node 5 is storing the key.
	    -Then Nodes 7,6,5,4,3 will receive the store request and will store the key,
	     because they are the k closest to the given key.

     =. Suppose that 10 nodes exist in the network as above. And suppose that
     	no nodes contains the key 4. Then node n calling find value for 4
	will fail.
	     -Node n will query all given nodes, and is unable to find the key 4.
	     -the UI listener will send a failed Find Value response through the socket.
	     -The UI process will print "Failure".

     =. Suppose that 10 nodes exist in the network as above. And suppose that
     	 key 5 exists within the network. Node n calling for key 5 will succed
	 (we really don't care which node gives us the value).
	     -The UI listener will send a success Find Value response through the socket.
	     -The UI process will print "Success".

	
     =. Suppose that nodes 0 and 1 exists in the network where node 1 is
     	 storing the key 5. Further suppose that node 0 is trying to find the
	 value associated with key 5 and that during the process, node 1 dies.
	 The find value should fail.
	     -If K >= 2, then the key 5 would have been stored in Node 0 itself.
	      So it will find the key in itself and send a success Find Value response.
	     -If K = 1, then Node 0 will not have the key and thus send KCLOSEST requests,
	      but it will not receive a response because Node 1 has died. Therefore
	      Node 0 will send a failure Find Value response.


=============================================
Suppose there is onle 1 node in the network

Node A
=============================================

STORE (WITH ONE NODE)

From the UI we say:
>store 10

-The UI will be connected to Node A and so A will receive the request to store key.
-A will first attempt to find the K closest nodes by calculating the distance between itself
 and the given key.
-Since A is the only node in the network, there will be no K closest nodes returned.

A will store the given key in itself.

==============================================

FIND (if we didn't STORE anything)

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

==============================================

FIND (if we actually did STORE something)

From the UI we say:
>store 10
>find 10

-Since A is the only node in the network, when the UI sent the STORE request, Node A stored
the key in itself.
-The first step A will do is check and see if it has the given key. It does! So we do not
need to perform a node lookup.

Node A will send a "Success" message to the UI.

The UI will print out "Success" to the user.

=============================================
Suppose there are only 2 nodes in the network

Node A (ID = 3) , Node B (ID = 4)

3 XOR 4 = 7

Node A is storing Node B in it's third K-bucket.
Node B is storing Node A in it's third K-bucket.
=============================================

STORE (WITH TWO NODES)

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

===========

FIND (with the UI connected to Node A - assuming we have stored a key first)

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

===========

Find (with the UI connected to Node B - assuming we have stored a key first)

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

===========


==================================================================================================

===REFRESHER AND TIMEOUT TESTS====

     =. Suppose that some node n is in the network. Further suppose that
     	n is dead.
	  -at most after an hour, it should not exist in any Routing Table
	  (INGY'S REFRESHER)

     =. Suppose node 0 is expecting a response from node 5. Node 5 dies,
     	 node 5 is ultimately removed from node 0's routing table.
	      -Node 0 can't get to Node 5, the request times out and it moves on
	      -The Refresher will remove Node 5 from all the routing tables.

     =. Suppose Node 5 sends the response after Node 0 times out the request.
     	  -Node 0 drops the response because it doesn't match any request in the Timer Queue.

  =============================================
  Suppose as the UI we try to contact a Node,
  but it died and  was removed from the network
  =============================================

  The UI will send any type of request to the Node it is connected to.
  There is a predefined point in time where the UI needs to give up waiting for a response.

  If the predefined timeout arrives and there still is no response, the UI will print
  to the user that the connection has timed out and ended.

  The UI will also give the user the option to enter a new IP address to connect to.


=================================
USER INTERFACE TEST CASES
=================================

NOTES BEFOREHAND:
      -The User Interface only accepts two maximum arguments.
      -It also only accepts three arguments: EXIT, STORE, and FIND.
      -EXIT can only be entered as one argument.
      -If you want to STORE or FIND, it's required to include a Key/Node ID
       as a second argument.
      -We are assuming the second argument is always an integer.

===============================================

Test Case #1 - Enter three arguments instead of one/two

Input:
>store 1234 4321

Output:
Error: Incorrect Input

==================

Test Case #2 - Enter one argument that is not EXIT

Input:
>notexit

Output:
Error: Incorrect Input

(NOTE: Even inputting "store" and "find" will produce the same
       output. Because it is still only one argument)

==================

Test Case #3 - Enter one argument that is EXIT

Input:
>exit

Output:
(program ends so we're back at the terminal prompt)

==================

Test Case #4 - Enter a request for STORE that is successful

Input:
>store 4

Output:
Success!

==================

Test Case #5 - Enter a request for FIND that is successful

Input:
>find 4

Output:
Success!

==================

Test Case #6 - Enter a request for FIND that is unsuccessful

Input:
>find 3

Output:
Failure.

==================

Test Case #7 - Enter a store/find request and the node doesn't respond

Input:
>find 3

Output:
>Error: Request timed out. Please re-enter an IP:

(Note: entering a new IP will have the socket close and reopen to the
       IP given)

============================================================
