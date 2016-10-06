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
  bucket = myTriples*[K];
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

//Pre: node does not exist within bucket, and the bucket is not full
//Post: node is placed at the tail of bucket
//      numTriples = numTriples + 1
void KBucket::addNode(myTriple* node) {
  bucket[numTriples] = node;
  //In this regard, the tail is where the 1st NULL is at
  numTriples++;
}

//Pre: id is some identifier, ie a key or another node
//     distance > 0
//Post: RV = array of nodes within distance of id
//      Return at most K such nodes
//      ordered by distance, that is the closest is at the head
//      Calling function must delete the returned value
myTriples* KBucket::getNodes(int id, int distance) {
  myTriples* closeNodes = new myTriples[K];
  for (int index = 0; (index < numTriples); index++) {
    //obtain distance for bucket[index] = d
    //find apprioate spot in closeNodes for bucket[index], if possible
    //why order it? It is possible to have more nodes in the bucket than we
    //need, if closeNodes is ordered, then we can more easily tell if we
    //wish to include bucket[index] or not
  }
}
  
//Pre: node exists within bucket
//Post: triple containing the node is moved to the tail of bucket
//      adjust triples to the left as necessary
void KBucket::adjustNode(myTriple* node) {
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
void KBucket::deleteNode(myTriple* node) {
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
  
