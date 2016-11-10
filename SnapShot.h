#ifndef included_SnapShot
#define included_SnapShot

#include <utility>
#include <algorithm>
#include "constants.h"

struct Quint{
  uint32_t node;
  uint32_t address;
  uint32_t port;
  uint32_t compareID;
  bool     queried;
Quint() : node(-1), address(0), port(0), compareID(0), queried(true) {}
Quint(const Quint & ref) : node(ref.node), address(ref.address), port(ref.port), compareID(ref.compareID), queried(ref.queried) {}
Quint(Quint & ref) : node(ref.node), address(ref.address), port(ref.port), compareID(ref.compareID), queried(ref.queried) {}
  //copy(Triple & ref) : node(ref.node), address(ref.address), port(ref.port) {}
};

//DEEP copy of from onto to
static void copyQuint(Quint & to, Quint & from){
  to.node = from.node;
  to.address = from.address;
  to.port = from.port;
  to.compareID = from.compareID;
  to.queried = from.queried;
}

//DEEP copy from Triple onto to Quint
static void copyQuintFromTriple(Quint & to, Triple & from){
  to.node = from.node;
  to.address = from.address;
  to.port = from.port;
}

//DEEP copy from Quint onto to Triple
static void copyTripleFromQuint(Triple & to, Quint & from){
  to.node = from.node;
  to.address = from.address;
  to.port = from.port;
}

using namespace std;

class SnapShot{

//CLASS INVARIENT:
// - The array of closest is always sorted from least to greatest distance 
//   from compareID
// - size is never greater than K

 private:

  Quint closest [K];	//array of current Kclosest to compareID
  uint32_t compareID;	//the ID we want to compare distance to
	uint32_t creatorID;	//the ID of who used this snapShot
  int size;						//current size of the closest array
  
 public:

	 uint32_t getSize() const;
	 uint32_t getElementIP(int i) const;
	 Quint getElementQuint(int i) const;

	 uint32_t getCompareID();
	 void setCompareID(uint32_t nodeID);

	 uint32_t getCreatorID();
	 void setCreatorID(uint32_t nodeID);

	 //PRE: an array of triples up to nodeSize.
	 //     We are assuming the nodes array is sorted
	 //POST: Triples from node placed into pair array
  SnapShot(Triple nodes[], uint32_t nodesSize, uint32_t NodeId, uint32_t creatorNodeID);

	//PRE: the nodeID we want to set our member data creatorID
	//POST: changes the creatorID
	SnapShot(uint32_t creatorNodeID);

	//DEFAULT CONSTRUCTOR
	SnapShot();

	//PRE: takes a creatorID and compareID to change our member data
	//POST: changes the quint.compareID to nodeID and creatorID
	SnapShot(uint32_t compareNodeID, uint32_t creatorNodeID);

	//COPY CONSTRUCTOR
	SnapShot(SnapShot & ss);

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
	//POST: returns true if we have a triple we haven't parsed yet
	bool nextExist();

	//PRE: Object defined
	//POST: will set all triples to 0's and all bools to trues
	void clear();
	
	void print();
	
};


#endif
