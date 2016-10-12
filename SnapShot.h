#ifndef included_SnapShot
#define included_SnapShot

#include "Message.hpp"
#include <utility>
#include <algorithm>

using namespace std;
class SnapShot{

 private:

  pair<Triple, bool> closest [K];

  int size;
  uint32_t compareID;

  void sort();
	
	bool compare ();

	//PRE: a key we want to find the distance from compareID
	//POST: returns the bitwise XOR distance from compareID to key
	uint32_t getDistance(uint32_t key);
  
 public:

	 //PRE: an array of triples up to nodeSize.
	 //     We are assuming the nodes array is sorted
	 //POST: Triples from node placed into pair array
  SnapShot(Triple nodes[], uint32_t nodesSize, uint32_t NodeId);


	//PRE: a k closest array already in closest to least closest order,
	//     as well as its size (not greater than K)
	//POST: add to the snapshot any items that are closer
	//      then what we already know, if any
  void addClosest(Triple * kClos, uint32_t kClosSize);

	//PRE: Object defined
	//POST: set the next closest Triple that doesn't have to our t.
	//      a true for its boolean. It will set the bool to true.
	//      Returns true if there is a next, false otherwise
	bool getNext (Triple & t);

	//PRE: Object defined
	//POST: will set all triples to 0's and all bools to trues
	void clear();
	

};


#endif
