

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
