//Justin Giguere
//Routing Table cc file

#include "KBucket.h"
#include "constants.h"
#include "RoutingTable.h"
#include <stdint.h>
#include <iostream>
#include <stdio.h>

using namespace std;

//Pre: id refers to a valid node object
//Post: table is an array of K kBuckets, that are each empty
RoutingTable::RoutingTable(uint32_t id) {
  myId = id;
}

//Pre: The RoutingTable exists
//Post: RV = true if no Triples exists in the kBuckets
//      False otherwise
bool RoutingTable::isEmpty() {
  bool empty = true;
  int index = 0;
  while (empty) {
    empty = table[index].isEmpty();
    index++;
  }
  return (empty);
}

//Pre: N/A
//Post: Prints the contents of the Routing Table
void RoutingTable::printTable() {
  printf("Routing Table \n \n");
  for (int index = 0; (index < NUMBITS); index++) {
    printf("nthBucket: %d \n", index);
    table[index].printBucket();
  }
}

//Pre: id1 and id2 are two identifiers
//Post: RV = id1 XOR id2
uint32_t RoutingTable::findDist(uint32_t id1, uint32_t id2) {
  return (id1 ^ id2);
}

//Pre: id is some valid node or key
//Post: RV = the nth kBucket such that d = findDist(id) where 2^n <= d < 2^n+1
int RoutingTable::findKBucket(uint32_t id) {
  uint32_t myDist = findDist(myId, id);
  uint32_t tempt = myDist;
  int twoPower = 0;
  while (tempt != 1) {
    tempt = tempt / 2;
    twoPower++;
  }
  printf("in findKBucket, id = %u, dist = %u nthBucket = %d \n",
	 id, myDist, twoPower);
  return (twoPower);
}

//Pre: target is some id, closeNodes is an array Triples with node = -1
//Post: closeNodes = array of the K closest nodes to the target
//      Less than K are returned iff less than K nodes are in the table
//      The list is ordered by distance, that is, the closest node is
//      at the head
//      RV = number nodes in closeNodes
int RoutingTable::getKClosetNodes(uint32_t target, Triple* closeNodes) {
  int size = 0;
  for (int index = 0; (index < NUMBITS); index++) {
    table[index].getKClosestNodes(target, closeNodes, size);
  }
  return (size);
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
  KBucket* currBucket = &(table[nthBucket]);
  Triple* currTriple = currBucket->getHead();
  bool added = false;
  Triple* newTriple = createTriple(node, address);
  if (currBucket->getNumTriples() == K) {  //the bucket is full
    printf("Rout addNode: bucketOverFlow \n");
    currBucket->deleteNode(currTriple->node);
  }
  else {
    printf("Rout addNode: bucket not full \n");
    currBucket->addNode(newTriple);
    added = true;
  }
  return (added);
}

//Pre: nodeID exists in the table, and is known to be dead
//Post: Removes the respected Triple from the table
void RoutingTable::deleteNode(uint32_t nodeID) {
  int nthBucket = findKBucket(nodeID);
  table[nthBucket].deleteNode(nodeID);
}
  
//Pre: myNode exists inside table
//Post: myNode is placed at the tail of its respected kBucket
//      the other triples are left shifted as needed
void RoutingTable::updateTable(uint32_t nodeID) {
  int nthBucket = findKBucket(nodeID);
  table[nthBucket].adjustNode(nodeID);
}

//Pre: 0 <= index < NUMBITS
//Post: RV =  a deep copy of table[index]
KBucket RoutingTable::operator [] (int index) {
  KBucket copy = table[index];
  return (copy);
}
