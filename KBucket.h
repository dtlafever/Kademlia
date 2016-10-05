//Justin Giguere
//Header file for kBucket

#include "constants.h"

class KBucket {

 private:
  
  struct myTriples {
    //a myTriples represents a node who does not own this routing table
    //address is the ip address of where the node is
    //port is where the node is listening 
    int address;
    int port;
    int node;
  };
  
  int numTriples;
  myTriples[K] bucket;     //each bucket has k triples in it

  //bucket is organized by time last seen, head is least recently, tail is most

 public:

  //Pre: routing tabKBUcle is initalized
  //Post: Creates a new KBucket where size = constant k
  //      and myTriples is an array of size k of triples
  //      numTriples = 0
  KBucket();

  //Pre: the respected routing table is being deleted
  //Post: bucket is destroyed
  ~KBucket();

  //Pre: node does not exist within bucket
  //Post: RV = true if node was added to the bucket, false otherwise
  bool addNode(int node);

  //Pre: id is some identifier, ie a key or another node
  //     distance > 0
  //Post: RV = array of nodes within distance of id
  int* getNode(int id, int distance);
  
  //Pre: node exists within bucket
  //Post: triple containing the node is moved to the tail of bucket
  //      adjust triples to the left as necessary
  void adjustNode(int node);

  //Pre: node exists within bucket
  //Post: triple containing the node is removed from the list
  //      adjust remaining triples as necessary
  void deleteNode(int node);
}
