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

 private:

  Quint closest [K];
  uint32_t compareID;
	uint32_t creatorID;
  int size;
  
 public:

	 uint32_t getSize() const;
	 uint32_t getElementIP(int i) const;

	 void setCompareID(uint32_t nodeID);

	 uint32_t getCreatorID();
	 void setCreatorID(uint32_t nodeID);

	 //PRE: an array of triples up to nodeSize.
	 //     We are assuming the nodes array is sorted
	 //POST: Triples from node placed into pair array
  SnapShot(Triple nodes[], uint32_t nodesSize, uint32_t NodeId, uint32_t creatorNodeID);

	//PRE: the nodeID we want to set our member data compareID
	//POST: changes the quint.compareID to nodeID
	SnapShot(uint32_t nodeID);

	//DEFAULT CONSTRUCTOR
	SnapShot();

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
