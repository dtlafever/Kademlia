#ifndef UDPSocket_class
#define UDPSocket_class

#include "Socket.h"
#include <fstream>
#include <string>

#define MAX_CHAR 80

class UDPSocket : private Socket {
 public:
  //PRE: the port we want to listen
  //POST: creates the socket and binds to the specified port.
  //      throws error if could not create or bind
  UDPSocket(uint32_t port, std::string fileName);
  UDPSocket() {};
  virtual ~UDPSocket();

  //PRE: the port we want to open on
  //POST: closes the socket, then creates and binds on specified port.
  //      throws error if could not create or bind
  void open(uint32_t port);

  //PRE: a message, host ip, and the port
  //POST: sends the message to that host.
  //      throws error if couldn't send
  void sendMessage(const std::string s, const std::string host,
		   const uint32_t port);

  //PRE: a message, host ip, and the port
  //POST: sends the message to that host.
  //      throws error if couldn't send
  void sendMessage(const std::string s, const uint32_t host,
		   const uint32_t port);

  //PRE: the buffer we want to store the message in
  //POST: store the message in the string and return the size of the message
  //NOTE: returns -1 if there was an error in recieving the message
  int recvMessage(std::string& s);

  //PRE: assumes that remaddr has a value (AKA recvMessage called)
  //POST: returns the IP address of the remote address
  int getRemoteIP();

  //same as above except return the string formatted IP and butts
  std::string getRemoteIP(int dummy);

  //PRE: takes a file name as input
  //POST: sets and opens that file.
  void setFileName(std::string fileName);
  
 private:
  std::ofstream fileLog;
  std::string fN;
  char hostName[MAX_CHAR];
};

#endif // !UDPSocket_class
