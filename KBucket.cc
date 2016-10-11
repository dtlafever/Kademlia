//Justin Giguere
//Header file for kBucket

#include "constants.h"
#include "KBucket.h"
#include <stdlib.h>
using namespace std;

//Pre: routing table is initalized
//Post: Creates a new KBucket where size = constant k
//      and myTriples is an array of size k of triples
//      numTriples = 0
KBucket() {
  numTriples = 0;
  bucket = Triple*[K];
  for (int index = 0; (index < K); index++) {
    bucket[index] = NULL;
  }
}

//Pre: the respected routing table is being deleted
//Post: bucket is destroyed
~KBucket() {
  for (int index = 0; (index < numTriples); index++) {
    delete bucket[index];
  }
}

//Pre: id1 and id2 are two identifiers
//Post: RV = id1 XOR id2
int KBucket::findDist(int id1, int id2) {
  return (id1 ^ id2);
}

//Pre: node does not exist within bucket, and the bucket is not full
//Post: node is placed at the tail of bucket
//      numTriples = numTriples + 1
void KBucket::addNode(Triple* node) {
  bucket[numTriples] = node;
  //In this regard, the tail is where the 1st NULL is at
  numTriples++;
}

//Pre: nodes is in smallest distance order, currNode has a home in nodes
//     index is where that home is
//Post: nodes[index] = curNode and nodes is shifted as needed
void KBucket::insertNode(myTriples* nodes, int insertDex,
			 myTriple* curNode, int size) {
  Triple* myCopy;
  myCopy.nodeID = curNode.nodeID;
  myCopy.port = UDPPORT;
  myCopy.address = curNode.address;
  //right shift
  for (int index = size - 1; (index > insertDex); index--) {
    nodes[index] = nodes[index - 1];
  }
  nodes[insertDex] = myCopy;
}

//Pre: target is some identifier, ie a key or another node
//      nodeHolder is an array of size K,
//      size is the current number of nodes in nodeHolder
//      nodeHolder is currently in smallest distance order
//Post: nodeHolder contains the closet nodes in this bucket ordered by
//      distance
void KBucket::getNodes(uint32_t target, myTriples* nodeHolder, int& size) {
  bool inserted = false;
  for (int index = 0; (index < numTriples); index++) {
    Triple* currTriple = bucket[index];
    uint32_t currDist = findDist(target, currTriple.nodeID);
    bool found = false;
    int insertDex = 0;
    while ((!found) and (insertDex < size)) {
      uint32_t compareDist = findDist(target, bucket[insertDex].nodeID);
      if (currDist > compareDist) {
	insertDex++;
      }
      else {
	found = true; //currDist is bigger than the preivous compareDist
      }
    }
    if ((insertDex < K) and (!found)) {
      //currDist was greater than all of nodeHolder
      //but there is still space in nodeHolder
      found = true;
    }
    if (found) {
      size++;
      insertNode(nodeHolder, insertDex, currTriple, size);
    }
  }
}
  
//Pre: node exists within bucket
//Post: triple containing the node is moved to the tail of bucket
//      adjust triples to the left as necessary
void KBucket::adjustNode(Triple* node) {
  int index = 0;
  bool found = false;
  while (!found) {
    if (node == bucket[index]) {
      found = true;
    }
    else {
      index++;
    }
  }
  //for reference, bucket[index] = node
  for (int otherDex = index; (otherDex < numTriples - 1); otherDex++) {
    bucket[otherDex] = bucket[otherDex + 1];
  } 
  bucket[numTriples - 1] = node;
}

//Pre: node exists within bucket
//Post: triple containing the node is removed from the list
//      adjust remaining triples as necessary
void KBucket::deleteNode(Triple* node) {
  int index = 0;
  bool found = false;
  while (!found) {
    if (node == bucket[index]) {
      found = true;
    }
    else {
      index++;
    }
  }
  delete node;
  for (int otherDex = index; (otherDex < numTriples - 1); otherDex++) {
    bucket[otherDex] = bucket[otherDex + 1];
  }
  bucket[numTriples - 1] = NULL;
  numTriples--;
}
  
