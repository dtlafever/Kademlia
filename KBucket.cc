//Justin Giguere
//Header file for kBucket

#include "constants.h"
#include "KBucket.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
using namespace std;

//Pre: routing table is initalized
//Post: Creates a new KBucket where size = constant k
//      and myTriples is an array of size k of triples
//      numTriples = 0
KBucket::KBucket() {
  printf("KBucket Constructor \n");
  numTriples = 0;
  for (int index = 0; (index < K); index++) {
    bucket[index] = NULL;
  }
}

//Pre: the respected routing table is being deleted
//Post: bucket is destroyed
KBucket::~KBucket() {
  printf("KBucket Destructor called \n");
  for (int index = 0; (index < numTriples); index++) {
    delete bucket[index];
    bucket[index] = NULL;
  }
  numTriples = 0;
}

//Pre: N/A
//Post: Prints the contents of the Routing Table
void KBucket::printBucket() {
  printf("numTriples: %d \n \n", numTriples);
  for (int index = 0; (index < numTriples); index++) {
    printf("index: %d nodeId: %u \n", index, bucket[index]->node);
  }
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
  bucket[numTriples] = node;
  //In this regard, the tail is where the 1st NULL is at
  numTriples++;
}

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
}
  
//Pre: node exists within bucket
//Post: triple containing the node is moved to the tail of bucket
//      adjust triples to the left as necessary
void KBucket::adjustNode(uint32_t nodeID) {
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
}

//Pre: node exists within bucket
//Post: triple containing the node is removed from the list
//      adjust remaining triples as necessary
void KBucket::deleteNode(uint32_t nodeID) {
  //Regardless if we know what the triple is beforehand, we still don;t
  //know where it is. And having the ID is more readily available
  
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
}

/*
//Pre: The current object exists
//Post: The current object is deleted and becomes a deep copy of other
KBucket KBucket::operator= (const KBucket& other) {

  printf("KBucket Assignment Operator entered \n");
  
  for (int index = 0; (index < numTriples); index++) {
    delete bucket[index];
  }
  for (int index = 0; (index < other.numTriples); index++) {
    Triple* toCopy = other.bucket[index];
    Triple* newTriple = new Triple;
    newTriple->node = toCopy->node;
    newTriple->address = toCopy->address;
    newTriple->port = toCopy->port;
    bucket[index] = newTriple;    
    /*
          Triple* toCopy = other.bucket[index];
	  bucket[index] = new Triple;
	  bucket[index]->node = toCopy->node;
	  bucket[index]->address = toCopy->address;
	  bucket[index]->port = toCopy->port;
     
  }
  return (*this);

  } */

Triple KBucket::operator[](int i)
{
	Triple temp;
	temp.node = -1;
	temp.port= -1;
	temp.address = -1;
	
	if(i<numTriples)
		return *(bucket[i]);
	else return temp;
}

