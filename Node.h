//Kademlia Node Class

class Node {

 private:
  
  RoutingTable RT;
  vector<int> keys;
  uint32_t ID;
  bool inNetwork;
  vector<Triple> refresherVector;

  //Pre: msg, queue, and timeOut were declared in the constructor below
  //Post: the id of the node sending msg is removed from timeOut
  //      if our id is in closest times, return true, false other wise
  bool Node::handleKClosMsg(Message msg, vector<MsgTimer>& timeOut,
			    JoinNewtorkQueue& queue);

 public:

  //Pre: id is a valid node id that is not yet taken
  //Post: RT is initalized, ID = id, inNetwork = true
  //      RT is empty since this node is starting a new network
  Node(uint32_t id) : RT(id);

  //Pre: id is a valid node id that is not yet taken, contactID and contactIP
  //     belongs to a node already existing in the network
  //Post: ID = id, contact exists within our routing table, as well as
  //      other nodes our contact has told about us
  //      inNetwork = true if FindNode on ourselves succeds, false otherwise
  Node(uint32_t id, uint32_t contactID, uint32_t contactIP) : RT(id);
  

  bool joined();

  void startListener();
  
  void startRefresher();
  void startUIListener();
};
