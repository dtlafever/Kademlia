
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

//PRE: two Pair objects we want to compare distances
//POST: returns whether lhs's distance is closer than rhs
bool SnapShot::sortByDistance(const Pair<Triple, bool> &lhs, 
										const Pair<Triple, bool> &rhs) { 
		return (getDistance(lhs.first.node) < getDistance(rhs.first.node));
}

//PRE: a k closest array already in closest to least closest order,
//     as well as its size (not greater than K)
//POST: add to the snapshot any items that are closer
//      then what we already know, if any
void SnapShot::addClosest(Triple * kClos, uint32_t kClosSize) {
	int curKClosIndex = 0;
	if (size < K) {
		//ASSERT: we need to just add to the closest because
		//        we don't have k closest yet in the snapshot
		while ((size < K) && (curKClosIndex < kClosSize)) {
			closest[size].first = kClos[curKClosIndex];
			size++;
			curKClosIndex++;
		}
		if (curKClosIndex >= kClosSize) {
			//ASSERT: we have added everything we can, sort closest
			sort(closest, closest + kCloseSize, sortByDistance);
		}
		else {
				//ASSERT: that means size = K and we still have items to
				//        consider from kClos
			while (curKClosIndex < kClosIndex) {
				if (getDistance(kClos[curKClosIndex]) <
					getDistance(closest[size - 1].first.node)) {
					//ASSERT: remove the last item and re sort

					copyTriple(closest[size - 1].first, kClos[curKClosIndex]);
					closest[size - 1].second = false;

					sort(closest, closest + kClosSize, sortByDistance);
				}
				curKClosIndex++;
			}
		}
		//ASSERT: we have added everything we can, and closest
		//        now has added the appropriate triples from kClos
	}
	
	bool noneLeftToAdd = false;
	for (int i = kClosSize - 1; (i >= 0) && (!noneLeftToAdd); i--) {
		if (getDistance(kClos[(kClosSize - 1) - i].node) > getDistance(closest[i].first.node)) {
			//ASSERT: the shortest distance in our kClos is still further than the
			//        furthest node in the array
			noneLeftToAdd = true;
		}else{
			
		}
	}
	//ASSERT: finished adding to the k closest
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
//POST: returns true if we have a triple we haven't parsed yet
bool SnapShot::nextExist() {
	bool foundNonParsed = false;
	for (int i = 0; i < size && (!foundNonParsed); i++) {
		if (closest[i].second == false) {
			foundNonParsed = true;
		}
	}
	return (foundNonParsed);
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