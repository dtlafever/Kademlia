=============================================
Suppose there is onle 1 node in the network

Node A
=============================================

1.STORE

From the UI we say:
>store 10

-The UI will be connected to Node A and so A will receive the request to store A.
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

***********************************************************

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





