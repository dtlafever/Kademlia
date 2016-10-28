#include "JoinNetworkQueue.h"
//#include <algorithm>

JoinNetworkQueue::JoinNetworkQueue(){

}

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
  if (!this->search_seen(new_contact)) {
    // actually add it to the vector.
    seen_nodes.push_back(new_contact);
    node_checked.push_back(false);
    // set joined true
    joined = true;
  }
  return joined;
}

// PRE:
//POST: returns true iff there exists an element in an index, i, of seen_nodes
//      where index i in node_checked contains the value false.
bool JoinNetworkQueue::hasNext() {
  bool hasNext;
  int index;
  //std::vector<bool>::iterator next_unseen;
  //std::vector<bool>::iterator front;
  //next_unseen = std::find(node_checked.begin(), node_checked.end(), false);

  bool found = false;
  for (int i=0; (i < node_checked.size()) && (!found); i++){
		if(false == node_checked[i]){
			found = true;
		}
	}

  // front = node_checked.begin();
  // if (next_unseen != node_checked.end()) {
  //   index = next_unseen - front;
  //   hasNext = true;
  // }
  // else {
  //   hasNext = false;
  // }
  //return hasNext;
  return found;
}

// PRE: assumes that hasNext will return true if the function is called.
//POST: returns the triple of the first node in seen_nodes [index i]
//      where the i index of node_checked is false.
Triple JoinNetworkQueue::getNext() {
  //std::vector<bool>::iterator front = node_checked.begin();
  //td::vector<bool>::iterator next_unseen;
  //next_unseen = std::find(node_checked.begin(), node_checked.end(), false);
  bool found = false;
  int index = 0;
  for (int i=0; (i < node_checked.size()) && (!found); i++){
		if(false == node_checked[i]){
			found = true;
			index = i;
		}
	}
  //index = next_unseen - front;
  node_checked[index] = true; // sets the node to be ineligible to get again
  return (seen_nodes[index]);
}

// PRE: takes a Triple of the node we are attempting to add.
//POST: returns true iff comparison is not already in seen_nodes
bool JoinNetworkQueue::search_seen(Triple comparison) {
  bool found = false;
 // std::vector<Triple>::iterator element;
 // element = std::find(seen_nodes.begin(), seen_nodes.end(), comparison);
 // if (element != seen_nodes.end()) {
 //   found = true;
 // }
	for (int i=0; (i < seen_nodes.size()) && (!found); i++){
		if(comparison.address == seen_nodes[i].address &&
			 comparison.node == seen_nodes[i].node &&
			 comparison.port == seen_nodes[i].port){
			found = true;
		}
	}
	
  return found;
}

Triple JoinNetworkQueue::operator [](int index) {
  return this->seen_nodes[index];
}
