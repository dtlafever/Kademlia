
#include "SnapShot.h"

//PRE: Constructor
//POST: Triples from node placed into pair array
SnapShot::SnapShot(Triple nodes[], int nodeSize, uint32_t NodeID){
	//TODO: sort the nodes array before insertion
  for(int i = 0; i < nodeSize; i++){
    closest[i].first = nodes[i];
    closest[i].second = false;
  }

  currentID = NodeID;
}

//PRE: triple array of size K 
//POST: returns the triples from our array of pairs
void SnapShot::getTriples(Triple * snapTriples) {
	
	for (int i = 0; i < K; i++) {
		copyTriple(snapTriples, closest[i].first);
	}

	return(snapTriples);
}