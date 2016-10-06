//Justin Giguere
//Routing Table cc file

#include "KBucket.h"
#include "constants.h"
#include "commonFunctions.h"
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
  
//Pre: id is some valid node or key
//Post: RV = the nth kBucket such that d = findDist(id) where 2^n <= d < 2^n+1
KBucket RoutingTable::findKBucket(int id) {
  int myDist = findDist(myId, id);
  int tempt = myDist;
  int twoPower = 0;
  while (tempt % 2 == 0) {
    tempt = tempt / 2;
    twoPower++;
  }
  return (meBuckets[twoPower]);
}

//Pre: target is some id, closeNodes is an array of K -1's
//Post: RV = array of the K closest nodes to the target
//      Less than K are returned iff less than K nodes are in the table
//      The list is ordered by distance, that is, the closest node is
//      at the head
void RoutingTable::getNodes(uint32_t target, myTriples* closeNodes) {
  /* Given some target id, return a list of the K-closest nodes to the target
     all can come from the same kBucket
     Logic would imply that the nodes closest to the target would be in the
     K-Bucket that the target would be in
     Search that K-Bucket
     The target could theroretically be at the edge, so check the 
     next and preivious buckets
     still not K, search the next two
   */
  
  
  /*
  myTriples* closeNodes = new myTriples[K];
  KBucket myBucket = findKBucket(target);
  myTriples* obtainedNodes 
  */
}

//Pre: node is a valid node object not in the routing table (id)
//     address correlates to where node is from
//Post: RV = true if node was added to the table
//           false otherwise
bool RoutingTable::addNode(int node, int address);
  
//Pre: myNode exists inside meBuckets
//Post: myNode is placed at the tail of its respected kBucket
//      the other triples are left shifted as needed
void RoutingTable::updateTable(myTriple* myNode);


//get function
