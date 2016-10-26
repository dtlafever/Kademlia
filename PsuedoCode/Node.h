#ifndef INCLUDED_Node
#define INDLUDED_Node


class Node{

 private:
  //TODO: RefresherQueue as member data
  
  RoutingTable RT;
  vector<int> keys();
  int ID;
  bool inNetwork;
  
 public:

  Node(int id);

  Node(int id, int contactID, int contactIP);

  bool joined();

  void startListener();
  
};


#endif
