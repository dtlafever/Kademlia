

class Node{

 private:
  RoutingTable RT;
  vector<int> keys();
  int ID;
  bool inNetwork;
  vector<Triple> refresherVector;

 public:

  Node(int id);

  Node(int id, int contactID, int contactIP);

  bool joined();

  void startListener();
  
  void startRefresher();
  void startUIListener();
};
