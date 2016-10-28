//#include "SnapShot.h"
//#include "constants.h"
//#include <iostream>
//
//using namespace std;
//
//int main(int argc, char * argv[]){
//
//  Triple kClos[3];
//  uint32_t kClosSize = 3;
//  uint32_t nodeID = 0;
//
//  for(int i=5; i < kClosSize+5; i++){
//    Triple temp;
//    temp.node = i;
//    copyTriple(kClos[i-5], temp);
//  }
//  
//  SnapShot SS(kClos, kClosSize, nodeID);
//
//  SS.print();
//  
//  Triple nextTriple;
//  
//  cout << "getNext: " << SS.getNext(nextTriple);
//  cout << " | "
//       << nextTriple.node << " | "
//       << nextTriple.address << " | "
//       << nextTriple.port << endl;
//
//  cout << "nextExist: " << SS.nextExist() << endl;
//
//  Triple new_kClos[1];
//  for (int i=0; i < 1; i++){
//    Triple temp;
//    temp.node = i;
//    copyTriple(new_kClos[i], temp);
//  }
//
//  cout << "added {0}" << endl;
//  
//  SS.addClosest(new_kClos, 1);
//
//  SS.print();
//
//  Triple new_kClos2[3];
//  for (int i=3; i < 6; i++){
//    Triple temp;
//    temp.node = i;
//    copyTriple(new_kClos2[i-3], temp);
//  }
//
//  cout << "added {3,4,5}" << endl;
//
//  SS.addClosest(new_kClos2, 3);
//
//  SS.print();
//  
//  SS.clear();
//
//  cout << "CLEARED." << endl;
//  
//  SS.print();
//  
//  Triple zero;
//  
//  cout << "getNext: " << SS.getNext(zero);
//  cout << " | "
//       << zero.node << " | "
//       << zero.address << " | "
//       << zero.port << endl;
//  
//  return (0);
//}
