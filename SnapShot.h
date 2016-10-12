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
  
 public:

	 //PRE: an array of triples up to nodeSize.
	 //     We are assuming the nodes array is sorted
	 //POST: Triples from node placed into pair array
  SnapShot(Triple nodes[], uint32_t nodeSize, uint32_t NodeId);


  //PRE: Object defined. 
  void addClosest(Message newMsg);

	//PRE: Object defined
	//POST: set the next closest Triple that doesn't have to our t.
	//      a true for its boolean. It will set the bool to true.
	//      Returns true if there is a next, false otherwise
	bool getNext (Triple & t);
	
	//PRE: triple array of size K 
	//POST: returns the triples from	our array of pairs,
	//      as well as the size of it
	uint32_t getTriples(Triple * snapTriples);
	// THIS FUNCTION MAY NOT BE NEEDED.PERHAPS JUST USING getNext

	Triple getNext (); // Should set the one return to used

	// Maybe add a getAlphaNext function???
	// And a clear function PLZ

};


#endif
