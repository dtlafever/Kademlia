#define PORT 6666

class Node {

 private:
  
  RoutingTable RT;
  vector<int> keys();
  uint32_t ID;
  bool inNetwork;
  
 public:


  //Pre: id is a valid node id that is not yet taken
  //Post: RT is initalized, ID = id, inNetwork = true
  //      RT is empty since this node is starting a new network
  Node(uint32_t id) : RT(id);

  Node(int id, int contactID, int contactIP);

  bool joined();

  void startListener();
  
  void startRefresher();
  void startUIListener();
};
