#ifndef included_SnapShot
#define included_SnapShot


class SnapShot{

 private:

  pair<Triple, bool> closest[k];

  int closestSize;
  uint32_t currentID;

  sort();
  
 public:

  //PRE: Constructor 
  //POST: Triples from node placed into pair array. 
  SnapShot(Triple nodes[], int nodeSize, uint32_t NodeId);


  //PRE: Object defined. 
  addCloset(Message newMsg);





};


#endif
