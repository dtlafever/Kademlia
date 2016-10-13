#include "SnapShot.h"
#include <cstdio>

//PRE: a key we want to find the distance from n1 to n2
//POST: returns the bitwise XOR distance from n1 to n2
uint32_t getDistance(uint32_t n1, uint32_t n2){
  return (n1 ^ n2);
}

//PRE: two Pair objects we want to compare distances
//POST: returns whether lhs's distance is closer than rhs
bool sortByDistance(const Quint &lhs, 
		    const Quint &rhs) { 
  return (getDistance(lhs.compareID, lhs.node) < getDistance(rhs.compareID, rhs.node));
}

//PRE: an array of triples up to nodeSize.
//     We are assuming the nodes array is sorted
//POST: Triples from node placed into pair array
SnapShot::SnapShot(Triple nodes[], uint32_t nodesSize, uint32_t NodeID){
  for(int i = 0; i < nodesSize; i++){
    copyQuintFromTriple(closest[i], nodes[i]);
    closest[i].queried = false;
    closest[i].compareID = NodeID;
    size++;
  }
}


//PRE: a k closest array already in closest to least closest order,
//     as well as its size (not greater than K)
//POST: add to the snapshot any items that are closer
//      then what we already know, if any
void SnapShot::addClosest(Triple * kClos, uint32_t kClosSize) {
  int curKClosIndex = 0;
  //ASSERT: we need to just add to the closest because
  //        we don't have k closest yet in the snapshot
  while ((size < K) && (curKClosIndex < kClosSize)) {
    copyQuintFromTriple(closest[size], kClos[curKClosIndex]);
    closest[size].queried = false;
    size++;
    curKClosIndex++;
  }
  //ASSERT: we have added everything we can, sort closest
  sort(closest, closest + size, sortByDistance);

  //ASSERT: that means size < K or size = K and
  //        we still might have items to
  //        consider from kClos
  while (curKClosIndex < kClosSize) {
    if (getDistance(closest[size - 1].compareID, kClos[curKClosIndex].node) <
	getDistance(closest[size - 1].compareID, closest[size - 1].node)) {
      //ASSERT: change last item to our new one and re sort
      
      copyQuintFromTriple(closest[size - 1], kClos[curKClosIndex]);
      closest[size - 1].queried = false;

      sort(closest, closest + size, sortByDistance);
    }
    curKClosIndex++;
  }
 
  //ASSERT: we have added everything we can, and closest
  //        now has added the appropriate triples from kClos
}

//PRE: Object defined
//POST: set t to the value of the next non queried Triple.
//      a true for its boolean. It will set the bool to true.
//      Returns true if there is a next, false otherwise
bool SnapShot::getNext(Triple & t)
{
  int index = -1;
  for (int i = 0; (i < size) && (index == -1); i++) {
    if (closest[i].queried == false) {
      index = i;
    }
  }
  
  if (index == -1) {
    return (false);
  }
  else {
    copyTripleFromQuint(t, closest[index]);
    closest[index].queried = true;
    return (true);
  }
}

//PRE: Object defined
//POST: returns true if we have a triple we haven't parsed yet
bool SnapShot::nextExist() {
  bool foundNonParsed = false;
  for (int i = 0; i < size && (!foundNonParsed); i++) {
    if (closest[i].queried == false) {
      foundNonParsed = true;
    }
  }
  return (foundNonParsed);
}

//PRE: Object defined
//POST: will set all triples to 0's and all bools to trues
void SnapShot::clear() {
  Quint zero;
  zero.queried = true;
  for (int i = 0; i < K; i++) {
    copyQuint(closest[i], zero);
  }
  size = 0;
}

void SnapShot::print(){
  printf("I: N | A | P | Q | C\n");
  printf("--------------------\n");
  for (int i = 0; i < size; i++){
  printf("%d: %d | %d | %d | %d | %d\n", i,
	 closest[i].node, closest[i].address,
	 closest[i].port, closest[i].queried,
	 closest[i].compareID);
  }
  printf("--------------------\n");
}
