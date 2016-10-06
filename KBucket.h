//Justin Giguere
//Header file for kBucket

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

  //Pre: id is some identifier, ie a key or another node
  //     distance > 0
  //      Return at most K such nodes
  //      ordered by distance, that is the closest is at the head
  //Post: RV = array of nodes within distance of id
  myTriples* getNodes(int id, int distance);
  
  //Pre: node exists within bucket
  //Post: triple containing the node is moved to the tail of bucket
  //      adjust triples to the left as necessary
  void adjustNode(myTriple* node);

  //Pre: node exists within bucket
  //Post: triple containing the node is removed from the list
  //      adjust remaining triples as necessary
  void deleteNode(myTriple* node);
}
