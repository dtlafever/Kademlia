#include "MsgTimer.h"
#include <iostream>

using namespace std;

int main() {

  MsgTimer m(2, 1, 10);
  sleep(1);
  cerr << m.timedOut() << endl;
  cerr << "ID" << (int)m.getNodeID() << endl;
  cerr << "IP" << (int)m.getNodeIP() << endl;
  sleep(1);
  cerr << m.timedOut() << endl;
  m.resetTimer();
  sleep(1);
  cerr << m.timedOut() << endl;
  sleep(1);
  cerr << m.timedOut() << endl;

  return 0;

}
