
#include "SnapShot.h"

//PRE: an array of triples up to nodeSize.
//     We are assuming the nodes array is sorted
//POST: Triples from node placed into pair array
SnapShot::SnapShot(Triple nodes[], uint32_t nodeSize, uint32_t NodeID){
  for(int i = 0; i < nodeSize; i++){
    copyTriple(closest[i].first, nodes[i]);
    closest[i].second = false;
		size++;
  }

  compareID = NodeID;
}

//PRE: Object defined
//POST: set the next closest Triple that doesn't have to our t.
//      a true for its boolean. It will set the bool to true.
//      Returns true if there is a next, false otherwise
bool SnapShot::getNext(Triple & t); {
	int index = -1;
	for (int i = 0; (i < size) && (index == -1); i++) {
		if (closest.second == true) {
			index = i;
		}
	}

	if (index == -1) {
		return (false);
	}
	else {
		return (true);
		copyTriple(t, closest[index]);
	}
}



//PRE: triple array of size K 
//POST: returns the triples from our array of pairs,
//      as well as the size of it
uint32_t SnapShot::getTriples(Triple * snapTriples) {
	
	for (int i = 0; i < K; i++) {
		copyTriple(snapTriples, closest[i].first);
	}

	return(snapTriples);
}