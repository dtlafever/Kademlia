//Justin Giguere
//Routing Table Header

#define NUMBITS 32
#define NUMKBUCKETS 5 //This value is based on NUMBITS and it must
                      //be that 2^NUMKBUCKETS = NUMBITS

#include "kBucket.h"

class RoutingTable {

 private:
  KBucket* meBuckets;
  int myID;

  //Pre: nodeID is the id of an existing node, address is where that
  //     node is from
  //Post: places the triple (node id, upd port, address) represented node
  //      into the tail of the approiate kBucket
  void updateTable(int nodeID, int address);

  //Pre: id is some identifier, either a key or another node id
  //Post: RV = myID XOR id
  int findDist(int id);
  
  //Pre: id is some valid node or key
  //Post: RV = the nth kBucket such that d = findDist(id) where 2^n <= d < 2^n+1
  KBucket* findKBucket(int id);

  /*
  //Pre:
  //Post:
  void findNode();
  */
}

