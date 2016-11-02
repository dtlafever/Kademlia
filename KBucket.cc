//Justin Giguere
//Header file for kBucket

#include "constants.h"
#include "KBucket.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <mutex>

#include <thread> //Testing the mutex
#include <chrono>

using namespace std;

//Pre: routing table is initalized
//Post: Creates a new KBucket where size = constant k
//      and myTriples is an array of size k of triples
//      numTriples = 0
KBucket::KBucket() {
  numTriples = 0;
  for (int index = 0; (index < K); index++) {
    bucket[index] = NULL;
  }
}

//Pre: This bucket is empty, it is unlocked
//Post: This bucket is a deep copy of bucket
KBucket::KBucket(KBucket& otherBucket) {
  lockBucket();
  otherBucket.lockBucket();
  numTriples = otherBucket.numTriples;
  for (int index = 0; (index < numTriples); index++) {
    Triple* tripleCopy = new Triple;
    Triple original = otherBucket[index];
    tripleCopy->node = original.node;
    tripleCopy->address = original.address;
    tripleCopy->port = original.port;
    bucket[index] = tripleCopy;
  }
  unlockBucket();
  otherBucket.unlockBucket();
}

//Pre: id is a valid id
//Post: RV = true if id is in the bucket, false otherwise
bool KBucket::isNodeInBucket(uint32_t id) {
  bool found = false;
  int index = 0;
  while ((!found) and (index < numTriples)) {
    Triple* currTriple = bucket[index];
    if (currTriple->node == id) {
      found = true;
    }
		index ++;
  }
  return (found);
}

//Pre: the respected routing table is being deleted
//Post: bucket is destroyed
KBucket::~KBucket() {
  for (int index = 0; (index < numTriples); index++) {
    delete bucket[index];
    bucket[index] = NULL;
  }
  numTriples = 0;
}

//Pre: lock is currently unlocked
//Post: lock is locked
void KBucket::lockBucket() {
  lock.lock();
}

//Pre: lock is currently locked
//Post: lock is unlocked
void KBucket::unlockBucket() {
  lock.unlock();
}

//Pre: The Bucket exists
//Post: RV = true if no triples exists in bucket
//      otherwise false
bool KBucket::isEmpty() {
  bool empty = false;
  if (numTriples == 0) {
    bool empty = true;
  }
  return (empty);
}

//Pre: N/A
//Post: Prints the contents of the Routing Table
void KBucket::printBucket() {
  printf("numTriples: %d \n \n", numTriples);
  lockBucket();
  for (int index = 0; (index < numTriples); index++) {
    printf("index: %d nodeId: %u \n", index, bucket[index]->node);
  }
  unlockBucket();
  printf("\n");
}

//Pre: N/A
//Post: RV = numTriples
int KBucket::getNumTriples() {
  return (numTriples);
}

//Pre: The Bucket has at least one triple
//Post: RV = bucket[head]
Triple* KBucket::getHead() {
  return (bucket[0]);
}

//Pre: id1 and id2 are two identifiers
//Post: RV = id1 XOR id2
uint32_t KBucket::findDist(uint32_t id1, uint32_t id2) {
  return (id1 ^ id2);
}

//Pre: node does not exist within bucket, and the bucket is not full
//Post: node is placed at the tail of bucket
//      numTriples = numTriples + 1
void KBucket::addNode(Triple* node) {
  //  printf("inside of addNode, before lock \n");
  lockBucket();
  bucket[numTriples] = node;
  //In this regard, the tail is where the 1st NULL is at
  numTriples++;
  unlockBucket();
}

// TODO: Check this function. Is this still used?
//Pre: nodes is in smallest distance order, currNode has a home in nodes
//     index is where that home is
//     nodes is an array of triples, curNode is a pointer to a triple
//     size <= K
//Post: nodes[index] = curNode and nodes is shifted as needed
void KBucket::insertNode(Triple* nodes, int insertDex,
			 Triple* curNode, int size) {
  Triple myCopy;
  myCopy.node = curNode->node;
  myCopy.address = curNode->address;
  myCopy.port = curNode->port;
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
//      size <= K
//Post: nodeHolder contains the closet nodes in this bucket ordered by
//      distance
void KBucket::getKClosestNodes(uint32_t target, Triple* nodeHolder, int& size) {
  lockBucket();
  bool inserted = false;
  for (int index = 0; (index < numTriples); index++) {
    Triple* currTriple = bucket[index];
    uint32_t currDist = findDist(target, currTriple->node);
    bool found = false;
    int insertDex = 0;
    while ((!found) and (insertDex < size)) {
      uint32_t compareDist = findDist(target, nodeHolder[insertDex].node);
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
      if (size < K) {
	size++;
      }
      insertNode(nodeHolder, insertDex, currTriple, size);
    }
  }
  unlockBucket();
}
  
//Pre: node exists within bucket
//Post: triple containing the node is moved to the tail of bucket
//      adjust triples to the left as necessary
void KBucket::adjustNode(uint32_t nodeID) {
  lockBucket();
  int index = 0;
  Triple* currTriple;
  uint32_t currID;
  bool found = false;
  while (!found) {
    currTriple = bucket[index];
    currID = currTriple->node;
    if (currID == nodeID) {
      found = true;
    }
    else {
      index++;
    }
  } //At this point, currTriple is the one containing nodeID
  for (int otherDex = index; (otherDex < numTriples - 1); otherDex++) {
    bucket[otherDex] = bucket[otherDex + 1];
  } 
  bucket[numTriples - 1] = currTriple;
  unlockBucket();
}

//Pre: node exists within bucket
//Post: triple containing the node is removed from the list
//      adjust remaining triples as necessary
void KBucket::deleteNode(uint32_t nodeID) {
  //Regardless if we know what the triple is beforehand, we still don;t
  //know where it is. And having the ID is more readily available
  lockBucket();
  int index = 0;
  bool found = false;
  Triple* toDie;
  while (!found) {
    toDie = bucket[index];
    if (nodeID == toDie->node) {
      found = true;
    }
    else {
      index++;
    }
  }
  delete toDie;
  for (int otherDex = index; (otherDex < numTriples - 1); otherDex++) {
    bucket[otherDex] = bucket[otherDex + 1];
  }
  bucket[numTriples - 1] = NULL;
  numTriples--;
  unlockBucket();
}

//Pre: 0 <= index < numTriples
//Post: RV = a deep copy of bucket[index]
Triple KBucket::operator[] (int index) {
  Triple myCopy;
  myCopy.node = bucket[index]->node;
  myCopy.address = bucket[index]->address;
  myCopy.port = bucket[index]->port;
  return (myCopy);
}

//Pre: This bucket is empty, it is unlocked
//Post: This bucket is a deep copy of bucket
void KBucket::operator= (KBucket& otherBucket) {
  lockBucket();
  otherBucket.lockBucket();
  numTriples = otherBucket.numTriples;
  for (int index = 0; (index < numTriples); index++) {
    Triple* tripleCopy = new Triple;
    Triple original = otherBucket[index];
    tripleCopy->node = original.node;
    tripleCopy->address = original.address;
    tripleCopy->port = original.port;
    bucket[index] = tripleCopy;
  }
  unlockBucket();
  otherBucket.unlockBucket();
}


void KBucket::copyKBucket(KBucket &otherBucket)
{
	lockBucket();
	otherBucket.lockBucket();
	numTriples = otherBucket.numTriples;
	for (int index = 0; (index < numTriples); index++) {
		Triple* tripleCopy = new Triple;
		Triple original = otherBucket[index];
		tripleCopy->node = original.node;
		tripleCopy->address = original.address;
		tripleCopy->port = original.port;
		bucket[index] = tripleCopy;
	}
	unlockBucket();
	otherBucket.unlockBucket();

}
