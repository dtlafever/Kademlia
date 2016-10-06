//Justin Giguere
//Routing Table Header

#include "KBucket.h"
#include "constants.h"

class RoutingTable {

 private:
  int myId;
  
  KBucket[K] meBuckets;

  //Pre: id refers to a valid node object
  //Post: meBuckets is an array of K kBuckets, that are each empty
  RoutingTable();

  //Pre: The respected node is going poof
  //Post: deletes each kBucket
  ~RoutingTable();

  //Pre: id1 and id2 are two identifiers
  //Post: RV = id1 XOR id2
  int findDist(int id1, int id2);
  
  //Pre: id is some valid node or key
  //Post: RV = the nth kBucket such that d = findDist(id) where 2^n <= d < 2^n+1
  KBucket findKBucket(int id);

  //Pre: target is some id
  //Post: RV = array of the K closest nodes to the target
  //      Less than K are returned iff less than K nodes are in the table
  //      If the target is found, it is placed at the head
  myTriples*[K] getNodes(int target);

  //Pre: node is a valid node object not in the routing table (id)
  //     address correlates to where node is from
  //Post: RV = true if node was added to the table
  //           false otherwise
  bool addNode(int node, int address);
  
  //Pre: myNode exists inside meBuckets
  //Post: myNode is placed at the tail of its respected kBucket
  //      the other triples are left shifted as needed
  void updateTable(myTriple* myNode);
  
}

