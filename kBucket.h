//Justin Giguere
//Header file for kBucket

class KBucket {

 private:

  //CI: 
  
  struct myTriples {
    int address;
    int port;
    int node;
  };
  
  int size;
  int numTriples;
  myTriples* bucket;

  //Pre: node exists within bucket
  //Post: triple containing the node is moved to the tail of bucket
  //      adjust triples to the left as necessary
  void adjustNode(int node);

  //Pre: node exists within bucket
  //Post: triple containing the node is removed from the list
  //      adjust remaining triples as necessary
  void deleteNode(int node);

 public:

  //Pre: routing tabKBUcle is initalized
  //Post: Creates a new KBucket where size = constant k
  //      and myTriples is an array of size k of triples
  //      numTriples = 0
  KBucket();

  //Pre: the respected routing table is being deleted
  //Post: bucket is destroyed
  ~KBucket();
}
