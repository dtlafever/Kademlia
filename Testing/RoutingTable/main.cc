////Justin Giguere
////testing routing table
//
//#include "RoutingTable.h"
//#include "KBucket.h"
//#include "constants.h"
//#include <iostream>
//
//using namespace std;
//
////Pre: N/A
////Post: Heap stuff is deleted
//int main() {
//  uint32_t address = 0; //We don't care about address for this testing
//                        //aka address does not affect my functionaility
//  bool end = false;
//  uint32_t myID = 0;
//  int flag;
//  uint32_t id;
//  RoutingTable myTable(myID);
//  cout << "Your id is 0" << endl;
//  cout << "Entering a value of -1 ends the test" << endl;
//  while (!end) {
//    cout << "0 to add node, 1 to move node, 2 to delete node, 3 to find node"
//	 << endl;
//    cin >> flag;
//    if (flag == 0) {
//      cout << "input a node id (aka a numbe) that hasn't been seen yet" << endl;
//      cin >> id;
//      if (id == -1) {
//	end = true;
//      }
//      else {
//	cout << "adding node" << endl;
//	bool added = myTable.addNode(id, address);
//	if (added) {
//	  cout << "node was successfully added" << endl;
//	}
//	else {
//	  cout << "node failed to be added" << endl;
//	}
//      }
//    }
//    else if (flag == 1) {
//      cout << "input a node id to adjust, it must currently exist" << endl;
//      cin >> id;
//      myTable.updateTable(id);
//    }
//    else if (flag == 2) {
//      cout << "input a node id to delete, it must currently exist" << endl;
//      cin >> id;
//      myTable.deleteNode(id);
//    }
//    else if (flag == 3) {
//      cout << "input a node id to find" << endl;
//      cin >> id;
//      Triple meTriples[K];
//      for (int index = 0; (index < K); index++) {
//	meTriples[index].node = -1;
//      }
//      int numNodes = myTable.getKClosetNodes(id, meTriples);
//      cout << "Closest Nodes I know of" << endl;
//      for (int index = 0; (index < numNodes); index++) {
//	cout << "index: " << index << " " <<
//	  "NodeID: " << meTriples[index].node << endl;
//      }
//    }
//    myTable.printTable();
//  }
//  return (0);
//}
