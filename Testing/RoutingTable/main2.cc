//Justin Giguere
//testing routing table

#include "RoutingTable.h"
#include "KBucket.h"
#include "constants.h"
#include <iostream>
#include <stdio.h>
#include <thread>

using namespace std;

//Pre:
//Post:
void doShit1(RoutingTable* myTable, uint32_t address) {
  printf("thread1 start \n");
  myTable->addNode(2, address);
  printf("thread2 end \n");
}

void doShit2(RoutingTable* myTable, uint32_t address) {
  printf("thread2 start \n");
  myTable->addNode(3, address);
  printf("thread2 end \n");
}

//Pre: N/A
//Post: Heap stuff is deleted
int main() {
  uint32_t address = 0; //We don't care about address for this testing
                        //aka address does not affect my functionaility
  RoutingTable myTable(0);
  myTable.addNode(1, address);
  
  thread thread1(doShit1, &myTable, address);
  thread thread2(doShit2, &myTable, address);

  thread1.join();
  thread2.join();
  return (0);
}
