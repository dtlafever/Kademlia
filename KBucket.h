//Justin Giguere
//Header file for kBucket

#ifndef INCLUDE_KBucket
#define INCLUDE_KBucket

#include "constants.h"
#include <mutex>

using namespace std;

class KBucket {

 private:
  
  int numTriples;
  Triple* bucket[K];     //each bucket has k triples in it
  mutex lock;

  //Pre: lock is currently unlocked
  //Post: lock is locked
  void lockBucket();

  //Pre: lock is currently locked
  //Post: lock is unlocked
  void unlockBucket();

  //Pre: id is a valid id
  //Post: RV = true if id is in the bucket, false otherwise
  bool isNodeInBucket(uint32_t id);

  //bucket is organized by time last seen, head is least recent, tail is most

 public:

  //Pre: routing table is initalized
  //Post: Creates a new KBucket where size = constant k
  //      and myTriples is an array of size k of triples
  //      numTriples = 0
  KBucket();

  //Pre: the respected routing table is being deleted
  //Post: bucket is destroyed
  ~KBucket();

  //Pre: This bucket is empty, it is unlocked
  //Post: This bucket is a deep copy of bucket
  KBucket(KBucket& otherBucket);

  //Pre: The Bucket exists
  //Post: RV = true if no triples exists in bucket
  //      otherwise false
  bool isEmpty();
  
  //Pre: N/A
  //Post: Prints the contents of the Routing Table
  void printBucket();

  //Pre: The Bucket has at least one triple
  //Post: RV = bucket[head]
  Triple* getHead();

  //Pre: N/A
  //Post: RV = numTriples
  int getNumTriples();

  //Pre: node does not exist within bucket, and the bucket is not full
  //Post: node is placed at the tail of bucket
  //      numTriples = numTriples + 1
  void addNode(Triple* node);

  //Pre: target is some identifier, ie a key or another node
  //      nodeHolder is an array of size K,
  //      size is the current number of nodes in nodeHolder
  //Post: nodeHolder contains the closet nodes in this bucket ordered by
  //      distance
  void getKClosestNodes(uint32_t target, Triple* nodeHolder, int& size);
  
  //Pre: nodeID exists within bucket
  //Post: triple containing the node is moved to the tail of bucket
  //      adjust triples to the left as necessary
  void adjustNode(uint32_t nodeID);

  //Pre: node exists within bucket
  //Post: triple containing the node is removed from the list
  //      adjust remaining triples as necessary
  void deleteNode(uint32_t nodeID);

  //Pre: nodes is in smallest distance order, currNode has a home in nodes
  //     index is where that home is
  //Post: nodes[index] = curNode and nodes is shifted as needed
  void insertNode(Triple* nodes, int insertDex, Triple* curNode, int size);
  
  //Pre: id1 and id2 are two identifiers
  //Post: RV = id1 XOR id2
  uint32_t findDist(uint32_t id1, uint32_t id2);

  //Pre: 0 <= index < numTriples
  //Post: RV = a deep copy of bucket[index]
  Triple operator[] (int index);

  //Pre: This bucket is empty, it is unlocked
  //Post: This bucket is a deep copy of bucket
  void operator= (KBucket& otherBucket);
  
};

#endif
