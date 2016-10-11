#include "constants.h"
#include <iostream>
#include <cstdio>
#include "UDPSocket.h"
#include "SocketException.h"

#define PORT 6666
#define HOST "10.200.102.212"

using namespace std;

int main(int argc, char * argv[]){
  try{

    UDPSocket socket(PORT);
    //ASSERT: Created and Bound

    string s;
    int recvlen;

    //continually listen for messages
    for(;;){
      printf("waiting on port %d\n", PORT);
      recvlen = socket.recvMessage(s);
      char * buf = (char *)s.c_str();
      printf("Recieved %d bytes\n", recvlen);
      if (recvlen > 0){
	buf[recvlen] = 0;
	printf("Recieved message: \"%s\"\n", buf);
	printf("The remote IP is %d\n", socket.getRemoteIP());
	printf("The remote IP is %s\n", (char *)socket.getRemoteIP(45756856).c_str());
      }
    }
    
  }catch(SocketException & e){

    cout << e.description() << endl;

  }
  return (0);
}
