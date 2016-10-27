#include "JoinNetworkQueue.h"
#include <algorithm>

// PRE:
//POST: initializes the vector of the nodes we've communicated with with our
//      contact in the network we are joining. seen_nodes will hold the
//      triple of our contact in index i. node_checked vector will also hold
//      false in the index i.
JoinNetworkQueue::JoinNetworkQueue(Triple contact) {
  seen_nodes.push_back(contact);
  node_checked.push_back(false);
}

// PRE: takes a Triple of a node we are attempting to add to our vector
//      seen_nodes
//POST: returns true iff it was able to add the Triple to the vector.
bool JoinNetworkQueue::add(Triple new_contact) {
  bool joined = false;
  if (!this->find(new_contact)) {
    // actually add it to the vector.
    seen_nodes.push_back(new_contact);
    node_checked.push_back(false);
    // set joined true
    joined = true;
  }
  return joined;
}

// PRE: takes the Triple of a node, node
//POST: returns true iff find is already in seen_nodes
bool JoinNetworkQueue::searched(Triple node) {
  bool found = false;
  Triple* found_node;
  found_node = &(*std::find(seen_nodes.begin(), seen_nodes.end(), node));
  if (found_node != seen_nodes.end()) {
    found = true;
  }
  return found;
}

// PRE:
//POST: returns true iff there exists an element in an index, i, of seen_nodes
//      where index i in node_checked contains the value false.
bool JoinNetworkQueue::hasNext() {
  bool hasNext;
  bool* next_unseen;
  bool* front = &node_checked[0];
  next_unseen = &(*std::find(node_checked.begin(), node_checked.end(), false));
  if (next_unseen != node_checked.end()) {
    index = next_unseen - front;
    hasNext = true;
  }
  else {
    hasNext = false;
  }
  return hasNext;
}

// PRE: assumes that hasNext will return true if the function is called.
//POST: returns the triple of the first node in seen_nodes [index i]
//      where the i index of node_checked is false.
Triple JoinNetworkQueue::getNext() {
  int index;
  bool* front = &node_checked[0];
  bool* next_unseen;
  next_unseen = &(*std::find(node_checked.begin(), node_checked.end(), false));
  index = next_unseen - front;
  return (seen_nodes[index]);
}

// PRE: takes a Triple of the node we are attempting to add.
//POST: returns true iff comparison is not already in seen_nodes
bool JoinNetworkQueue::search_seen(Triple comparison) {
  bool found = false;
  Triple* element;
  element = &(*std::find(seen_nodes.begin(), seen_nodes.end(), comparison));
  if (element != seen_nodes.end()) {
    found = true;
  }
  return found;
}
