#include "UDPSocket.h"
#include "SocketException.h"
#include "constants.h"

//PRE: the port we want to listen
//POST: creates the socket and binds to the specified port.
//      throws error if could not create or bind
UDPSocket::UDPSocket(uint32_t port)
{
	fileLog.open(LOG_NAME, std::ofstream::app);
	
	if (!Socket::create(UDP)) {
		fileLog << "Could not create server socket.\n";
		throw SocketException("Could not create server socket.");
	}

	if (!Socket::bind(port)) {
		fileLog<< "Could not bind port.\n";
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
		fileLog << "Could not create server socket.\n";
		throw SocketException("Could not create server socket.");
	}

	if (!Socket::bind(port)) {
		fileLog << "Could not bind port.\n";
		throw SocketException("Could not bind port.");
	}
}

//PRE: a message, host ip, and the port
//POST: sends the message to that host.
//      throws error if couldn't send
void UDPSocket::sendMessage(const std::string s, const std::string host,
						const uint32_t port) {
	if (!Socket::sendTo(s, host, port)) {
		fileLog << "Could not send message '" << s << "'.\n";
		throw SocketException("Could not send message.");
	}
	else {
		fileLog << "Sent Message '" << s << "'.\n";
	}
}

//PRE: a message, host ip, and the port
//POST: sends the message to that host.
//      throws error if couldn't send
void UDPSocket::sendMessage(const std::string s, const int host,
	const uint32_t port) {
	if (!Socket::sendTo(s, host, port)) {
		fileLog << "Could not send message '" << s << "'.\n";
		throw SocketException("Could not send message.");
	}
	else {
		fileLog << "Sent Message '" << s << "'.\n";
	}
}

//PRE: the buffer we want to store the message in
//POST: store the message in the string and return the size of the message
//NOTE: returns -1 if there was an error in recieving the message
int UDPSocket::recvMessage(std::string& s) {
	int length = Socket::recvFrom(s);
	if (length > 0) {
		fileLog << "Recieved Message '" << s << "'.\n";
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
