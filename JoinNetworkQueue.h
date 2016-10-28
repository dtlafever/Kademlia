#ifndef INCLUDED_JoinNetworkQueue
#define INCLUDED_JoinNetworkQueue

#include <vector>
#include "constants.h"
// maybe other stuff included

class JoinNetworkQueue {

private:
  std::vector<Triple> seen_nodes;
  std::vector<bool> node_checked;

  // PRE: takes a Triple of the node we are attempting to add.
  //POST: returns true iff comparison is not already in seen_nodes
  bool search_seen(Triple comparison);

public:

  // PRE:
  //POST: initializes the vector of the nodes we've communicated with with our
  //      contact in the network we are joining. seen_nodes will hold the
  //      triple of our contact in index i. node_checked vector will also hold
  //      false in the index i.
  JoinNetworkQueue(Triple contact);

  // PRE: takes a Triple of a node we are attempting to add to our vector
  //      seen_nodes
  //POST: returns true iff it was able to add the Triple to the vector.
  bool add(Triple new_contact);

  // PRE:
  //POST: returns true iff there exists an element in an index, i, of seen_nodes
  //      where index i in node_checked contains the value false.
  bool hasNext();

  // PRE: assumes that hasNext will return true if the function is called.
  //POST: returns the triple of the first node in seen_nodes [index i]
  //      where the i index of node_checked is false.
  Triple getNext();

  Triple operator [](int index);
};

#endif
