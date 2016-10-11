//Justin Giguere
//Routing Table cc file

#include "KBucket.h"
#include "constants.h"
#include "RoutingTable.h"
#include <stdint.h>
#include <iostream>

using namespace std;

//Pre: id refers to a valid node object
//Post: meBuckets is an array of K kBuckets, that are each empty
RoutingTable::RoutingTable(uint32_t id) {
  myId = id;
}

/*
//Pre: The respected node is going poof
//Post: deletes each kBucket
RoutingTable::~RoutingTable() {
  for (int index = 0; (index < K); index++) {
    delete meBuckets[index];
  }
}
*/

//Pre: id1 and id2 are two identifiers
//Post: RV = id1 XOR id2
uint32_t RoutingTable::findDist(uint32_t id1, uint32_t id2) {
  return (id1 ^ id2);
}

//Pre: id is some valid node or key
//Post: RV = the nth kBucket such that d = findDist(id) where 2^n <= d < 2^n+1
int RoutingTable::findKBucket(uint32_t id) {
  int myDist = findDist(myId, id);
  int tempt = myDist;
  int twoPower = 0;
  while (tempt % 2 == 0) {
    tempt = tempt / 2;
    twoPower++;
  }
  return (twoPower);
}

/*
//Pre:
//Post:
void getNodesHelper(int size, uint32_t target, Triple* closeNodes) {
  nextBucket
}
*/

//Pre: target is some id, closeNodes is an array of K -1's
//Post: RV = array of the K closest nodes to the target
//      Less than K are returned iff less than K nodes are in the table
//      The list is ordered by distance, that is, the closest node is
//      at the head
void RoutingTable::getNodes(uint32_t target, Triple* closeNodes) {
  int nthBucket = findKBucket(target);
  int nextBucket = nthBucket + 1;
  int lastBucket = nthBucket - 1;
  bool searchMore = ((nextBucket < K) or (lastBucket >= 0));
  int size = 0;
  meBuckets[nthBucket].getNodes(target, closeNodes, size);
  while ((size < K) and searchMore) {
    if (nextBucket < K) {
      meBuckets[nextBucket].getNodes(target, closeNodes, size);
    }
    if (lastBucket >= 0) {
      meBuckets[lastBucket].getNodes(target, closeNodes, size);
    }
    nextBucket++;
    lastBucket--;
    searchMore = ((nextBucket < K) or (lastBucket >= 0));
  }
}

//Pre: id and address represents a new node not seen before
//Post: RV = Triple object representing this new node
Triple* RoutingTable::createTriple(uint32_t id, uint32_t address) {
  Triple* newTriple = new Triple;
  newTriple->address = address;
  newTriple->node = id;
  newTriple->port = UDPPORT;
  return (newTriple);
}

//Pre: node is a valid node object not in the routing table (id)
//     address correlates to where node is from
//Post: RV = true if node was added to the table
//           false otherwise
bool RoutingTable::addNode(uint32_t node, uint32_t address) {
  int nthBucket = findKBucket(node);
  KBucket currBucket = meBuckets[nthBucket];
  Triple* currTriple = currBucket.getHead();
  bool added = false;
  Triple* newTriple = createTriple(node, address);
  if (currBucket.getNumTriples() == K) {  //the bucket is full
    // bool dead = ping(curTriple.address);
    bool dead = true;
    if (dead) {
      //the least recently seen node is dead
      currBucket.deleteNode(currTriple);
      currBucket.addNode(newTriple);
    }
  }
  else {
    currBucket.addNode(newTriple);
    added = true;
  }
  return (added);
}
  
//Pre: myNode exists inside meBuckets
//Post: myNode is placed at the tail of its respected kBucket
//      the other triples are left shifted as needed
void RoutingTable::updateTable(Triple* myNode) {
  uint32_t nodeID = myNode->node;
  int myKBucket = findKBucket(nodeID);
  meBuckets[myKBucket].adjustNode(myNode);
}
