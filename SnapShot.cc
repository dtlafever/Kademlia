
#include "SnapShot.h"

//PRE: an array of triples up to nodeSize.
//     We are assuming the nodes array is sorted
//POST: Triples from node placed into pair array
SnapShot::SnapShot(Triple nodes[], uint32_t nodesSize, uint32_t NodeID){
  for(int i = 0; i < nodesSize; i++){
    copyTriple(closest[i].first, nodes[i]);
    closest[i].second = false;
		size++;
  }

  compareID = NodeID;
}

//PRE: a key we want to find the distance from compareID
//POST: returns the bitwise XOR distance from compareID to key
uint32_t SnapShot::getDistance(uint32_t key) {
	return (compareID ^ key);
}

//PRE: a k closest array already in closest to least closest order,
//     as well as its size (not greater than K)
//POST: add to the snapshot any items that are closer
//      then what we already know, if any
void SnapShot::addClosest(Triple * kClos, uint32_t kClosSize) {
	for (int i = kClos - 1; i <= 0; i--) {
		
	}
}

//PRE: Object defined
//POST: set t to the value of the next non queried Triple.
//      a true for its boolean. It will set the bool to true.
//      Returns true if there is a next, false otherwise
bool SnapShot::getNext(Triple & t)
{
	int index = -1;
	for (int i = 0; (i < size) && (index == -1); i++) {
		if (closest[i].second == true) {
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

//PRE: Object defined
//POST: will set all triples to 0's and all bools to trues
void SnapShot::clear() {
	Triple zero;
	for (int i = 0; i < K; i++) {
		copyTriple(closest[i].first, zero);
		closest[i].second = false;
	}
}