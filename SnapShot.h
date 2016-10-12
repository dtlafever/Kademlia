#ifndef included_SnapShot
#define included_SnapShot

#include "Message.hpp"
#include <utility>
#include <algorithm>

using namespace std;
class SnapShot{

 private:

  pair<Triple, bool> closest [K];

  int closestSize;
  uint32_t currentID;

  void sort();
	
	bool compare ();
  
 public:

  //PRE: Constructor 
  //POST: Triples from node placed into pair array. 
  SnapShot(Triple nodes[], int nodeSize, uint32_t NodeId);


  //PRE: Object defined. 
  void addClosest(Message newMsg);

	Triple getNext ();
	
	//PRE: 
	//POST: returns the triples from our array of pairs
	void getTriples(Triple * snapTriples);

	bool nextExists ();

};


#endif
