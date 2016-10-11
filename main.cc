//Justin Giguere
//testing routing table

#include "RoutingTable.h"
#include "KBucket.h"
#include <iostream>

using namespace std;

//Pre: N/A
//Post: Heap stuff is deleted
int main() {
  uint32_t address = 0; //We don't care about address for this testing
                        //aka address does not affect my functionaility
  bool end = false;
  uint32_t myID = 0;
  uint32_t id;
  RoutingTable myTable(myID);
  cout << "Your id is 0" << endl;
  cout << "Entering a value of -1 ends the test" << endl;
  while (!end) {
    cout << "input a node id (aka a numbe) that hasn't been seen yet" << endl;
    cin >> id;
    if (id == -1) {
      end = true;
    }
    else {
      bool added = myTable.addNode(id, address);
      cout << "myTable" << endl;
      myTable.printTable();
    }
    cout << "enter 0 if you wish to adjust an existing node" << endl;
    cin >> id;
    if (id == 0) {
      cout << "input a node id to adjust, it must currently exist" << endl;
      cin >> id;
      myTable.updateTable(id);
    }
  }
  return (0);
}
