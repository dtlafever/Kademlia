#ifndef UDPSocket_class
#define UDPSocket_class

#include "Socket.h"
#include <string>

class UDPSocket : private Socket {
public:
	//PRE: the port we want to listen and send messages on
	//POST: creates the socket and binds to the specified port.
	//      throws error if could not create or bind
	UDPSocket(uint32_t port);
	UDPSocket() {};
	virtual ~UDPSocket();

	//PRE: a message, host ip, and the port
	//POST: sends the message to that host and then closes the connection.
	//      throws error if couldn't send
	void sendMessage(const std::string s, const std::string host,
						const uint32_t port);

	//PRE: a message, host ip, and the port
	//POST: sends the message to that host and then closes the connection.
	//      throws error if couldn't send
	void sendMessage(const std::string s, const std::uint32_t host,
		const uint32_t port);

	//PRE: the buffer we want to store the message in
	//POST: store the message in the string and return the size of that junk
	int recvMessage(std::string& s);

	//PRE: assumes that remaddr has a value (AKA recvMessage called)
	//POST: returns the IP address of the remote address
	int getRemoteIP();

	//same as above except return the string formatted IP and butts
	std::string getRemoteIP(int dummy);
private:
	
};

#endif // !UDPSocket_class
