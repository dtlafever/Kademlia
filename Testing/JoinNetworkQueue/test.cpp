#include "JoinNetworkQueue.h"
#include "../../constants.h"
#include <iostream>

using namespace std;

int main() {

  Triple t;
  t.address = 0;
  t.port = 6666;
  t.node = 15;

  Triple t2;
  t2.address = 3;
  t2.port = 6667;
  t2.node = -32;

  bool added = false;

  JoinNetworkQueue j(t);
  added = j.add(t2);
  cerr << "added: " << added << endl;
  added = j.add(t);
  cerr << "added again: " << added << endl;
  cerr << j.hasNext() << endl;
  cerr << j.getNext().node << endl;
  cerr << j.hasNext() << endl;
  cerr << j.getNext().node << endl;
  cerr << j.hasNext() << endl;
  added = j.add(t2);
  cerr << "final addition: " << added << endl;
  return 0;

}
