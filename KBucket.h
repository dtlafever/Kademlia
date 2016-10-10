//Justin Giguere
//Header file for kBucket

#ifndef INCLUDE_KBucket.h
#define INCLUDE_KBucket.h

#include "constants.h"

class KBucket {

 private:
  
  int numTriples;
  myTriples*[K] bucket;     //each bucket has k triples in it
                            //ease of moving triples around

  //bucket is organized by time last seen, head is least recently, tail is most

 public:

  //Pre: routing table is initalized
  //Post: Creates a new KBucket where size = constant k
  //      and myTriples is an array of size k of triples
  //      numTriples = 0
  KBucket();

  //Pre: the respected routing table is being deleted
  //Post: bucket is destroyed
  ~KBucket();

  //Pre: node does not exist within bucket, and the bucket is not full
  //Post: node is placed at the tail of bucket
  //      numTriples = numTriples + 1
  void addNode(myTriple* node);

  //Pre: target is some identifier, ie a key or another node
  //      nodeHolder is an array of size K,
  //      size is the current number of nodes in nodeHolder
  //Post: nodeHolder contains the closet nodes in this bucket ordered by
  //      distance
  void KBucket::getNodes(int target, myTriples* nodeHolder, int& size);
  
  //Pre: node exists within bucket
  //Post: triple containing the node is moved to the tail of bucket
  //      adjust triples to the left as necessary
  void adjustNode(myTriple* node);

  //Pre: node exists within bucket
  //Post: triple containing the node is removed from the list
  //      adjust remaining triples as necessary
  void deleteNode(myTriple* node);

  //Pre: nodes is in smallest distance order, currNode has a home in nodes
  //     index is where that home is
  //Post: nodes[index] = curNode and nodes is shifted as needed
  void insertNode(myTriples* nodes, int insertDex, myTriple* curNode, int size);
  
  //Pre: id1 and id2 are two identifiers
  //Post: RV = id1 XOR id2
  int findDist(int id1, int id2);
  
  //get all valid triples
}

#endif
