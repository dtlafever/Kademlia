
#include "SnapShot.h"

//PRE: Constructor
//POST: Triples from node placed into pair array
SnapShot::SnapShot(Triple nodes[], int nodeSize, uint32_t NodeID){
  for(int i = 0; i < nodeSize; i++){
    closest[i].first = nodes[i];
    closest[i].second = false;
  }

  currentID = NodeID;
}
