//Justin Giguere
//Routing Table cc file

#include "KBucket.h"
#include "constants.h"
#include "RoutingTable.h"

//Pre: id refers to a valid node object
//Post: meBuckets is an array of K kBuckets, that are each empty
RoutingTable(int id) {
  myId = id;
  meBuckets = KBucket[K];
}

//Pre: The respected node is going poof
//Post: deletes each kBucket
~RoutingTable() {
  for (int index = 0; (index < K); index++) {
    delete meBuckets[index];
  }
}

//Pre: id1 and id2 are two identifiers
//Post: RV = id1 XOR id2
int RoutingTable::findDist(int id1, int id2) {
  //TBD if we are going to use masks
}
  
//Pre: id is some valid node or key
//Post: RV = the nth kBucket such that d = findDist(id) where 2^n <= d < 2^n+1
KBucket RoutingTable::findKBucket(int id) {
  int myDist = findDist(myId, id);
  //find greatest power of 2, b, such that b < myDist
  //return meBuckets[b]
}

//Pre: target is some id
//Post: RV = array of the K closest nodes to the target
//      Less than K are returned iff less than K nodes are in the table
//      If the target is found, it is placed at the head
myTriples*[K] RoutingTable::getNodes(int target);

//Pre: node is a valid node object not in the routing table (id)
//     address correlates to where node is from
//Post: RV = true if node was added to the table
//           false otherwise
bool RoutingTable::addNode(int node, int address);
  
//Pre: myNode exists inside meBuckets
//Post: myNode is placed at the tail of its respected kBucket
//      the other triples are left shifted as needed
void RoutingTable::updateTable(myTriple* myNode);


