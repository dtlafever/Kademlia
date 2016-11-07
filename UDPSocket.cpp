#include "UDPSocket.h"
#include "SocketException.h"
#include "constants.h"
#include <unistd.h>

//PRE: the port we want to listen
//POST: creates the socket and binds to the specified port.
//      throws error if could not create or bind
UDPSocket::UDPSocket(uint32_t port, std::string fileName)
{
  gethostname(hostName, MAX_CHAR);

  fN = LOG_DIR + fileName;
  fileLog.open(fN, std::ofstream::out | std::ofstream::app);
  fileLog << hostName << ": --------START NETWORK--------" << std::endl;
  if (!Socket::create(UDP)) {
    fileLog << hostName << ": Could not create server socket." << std::endl;
    throw SocketException("Could not create server socket.");
  }
  
  if (!Socket::bind(port)) {
    fileLog << hostName << ": Could not bind port.\n" << std::endl;
    throw SocketException("Could not bind port.");
  }
  
	
}

UDPSocket::~UDPSocket()
{
	fileLog.close();
}

//PRE: the port we want to open on
//POST: closes the socket, then creates and binds on specified port.
//      throws error if could not create or bind
void UDPSocket::open(uint32_t port) {
  Socket::close();
  if (!Socket::create(UDP)) {    
    fileLog << hostName << ": Could not create server socket." << std::endl;
    throw SocketException("Could not create server socket.");
  }
  
  if (!Socket::bind(port)) {
    fileLog << hostName << ": Could not bind port." << std::endl;
    throw SocketException("Could not bind port.");
  }
}

//PRE: a message, host ip, and the port
//POST: sends the message to that host.
//      throws error if couldn't send
void UDPSocket::sendMessage(const std::string s, const std::string host,
						const uint32_t port) {
  if (!Socket::sendTo(s, host, port)) {
    fileLog << hostName << ": Could not send message '" << s << "'." << std::endl;
    throw SocketException("Could not send message.");
  }
  else {
    fileLog << hostName << ": Sent Message '" << s << "'." << std::endl;
  }
}

//PRE: a message, host ip, and the port
//POST: sends the message to that host.
//      throws error if couldn't send
void UDPSocket::sendMessage(const std::string s, const uint32_t host,
	const uint32_t port) {
	if (!Socket::sendTo(s, host, port)) {
	  fileLog << hostName << ": Could not send message '" << s << "'." << std::endl;
	  throw SocketException("Could not send message.");
	}
	else {
	  fileLog << hostName << ": Sent Message '" << s << "' to "<< IP_toString(host) <<"." << std::endl;
	}
}

//PRE: the buffer we want to store the message in
//POST: store the message in the string and return the size of the message
//NOTE: returns -1 if there was an error in recieving the message
int UDPSocket::recvMessage(std::string& s) {
  int length = Socket::recvFrom(s);
  if (length > 0) {
    fileLog << hostName << ": Recieved Message '" << s << "'." << std::endl;
  }
  
  return (length);
}

//PRE: assumes that remaddr has a value (AKA recvMessage called)
//POST: returns the IP address of the remote address
int UDPSocket::getRemoteIP() {
  return (Socket::getRemoteIP());
}

//same as above except return the string formatted IP and butts
std::string UDPSocket::getRemoteIP(int dummy){
  return (Socket::getRemoteIP(dummy));
}

//PRE: an integer IP
//POST: returns the string version of IP
std::string UDPSocket::IP_toString(uint32_t ip){
  return (Socket::IP_toString(ip));
}

//PRE: takes a file name as input
//POST: sets and opens that file.
void UDPSocket::setFileName(std::string fileName){
  fileLog.close();
  fN = fileName;
  fileLog.open(fileName, std::ofstream::out | std::ofstream::app);
}
