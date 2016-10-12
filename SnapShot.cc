
#include "SnapShot.h"

//PRE: an array of triples up to nodeSize.
//     We are assuming the nodes array is sorted
//POST: Triples from node placed into pair array
SnapShot::SnapShot(Triple nodes[], int nodeSize, uint32_t NodeID){
  for(int i = 0; i < nodeSize; i++){
    copyTriple(closest[i].first, nodes[i]);
    closest[i].second = false;
		size++;
  }

  compareID = NodeID;
}

//PRE: triple array of size K 
//POST: returns the triples from our array of pairs
void SnapShot::getTriples(Triple * snapTriples) {
	
	for (int i = 0; i < K; i++) {
		copyTriple(snapTriples, closest[i].first);
	}

	return(snapTriples);
}