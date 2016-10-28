//#include "constants.h"
//#include <iostream>
//#include <cstdio>
//#include "UDPSocket.h"
//#include "SocketException.h"
//
//#define PORT 6666
//#define HOST "10.200.102.211"
//
//using namespace std;
//
//int main(int argc, char * argv[]){
//  try{
//
//    UDPSocket socket(PORT);
//    //ASSERT: Created and Bound
//
//    char buf[1000];
//    
//    for(int i=0; i < 10; i++){
//      printf("Sending packet %d to %s port %d\n", i, HOST, PORT);
//      sprintf(buf, "This is packet %d\n", i);
//      socket.sendMessage(buf, HOST, PORT);
//    }
//    
//  }catch(SocketException & e){
//
//    cout << e.description() << endl;
//
//  }
//  return (0);
//}
