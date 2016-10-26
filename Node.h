#define PORT 6666

class Node{

 private:
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
