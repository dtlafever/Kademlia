// Implementation of the Socket class.


#include "Socket.h"
#include "string.h"
#include <string.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <cstdio>

using namespace std;

//PRE:
//POST: set the error code to -1, and set the struct m_addr to 0's
Socket::Socket() :
  m_sock ( -1 )
{

  memset ( &m_addr,
	   0,
	   sizeof ( m_addr ) );

}

//PRE:
//POST: close the socket with the error code (if there is one)
Socket::~Socket()
{
  if ( is_valid() )
    ::close ( m_sock );
}

//PRE:
//POST: create a socket that is either TCP or UDP
//      Returns false if not a valid socket creation, true otherwise
bool Socket::create(bool protocol)
{
  if(protocol == TCP){
    //Set socket to be over IPv4, blocking two way connection, no extra protocols
    m_sock = socket ( AF_INET,
		      SOCK_STREAM,
		      0 );
  }else{
    //UDP non blocking
	m_sock = socket(AF_INET, SOCK_DGRAM, 0);
  }
  if ( ! is_valid() )
    return false;


  // TIME_WAIT - argh
  int on = 1;
  //if failed to set the options for the socket, return false
  if (protocol == TCP) {
	if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) == -1)
		return false;
  }else{
	if (setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof(on)) == -1)
		return false;
  }
  return true;

}

//PRE: the port number to bind our socket to
//POST: returns false if we can't bind to that port, true otherwise
bool Socket::bind ( const int port )
{

  if ( ! is_valid() )
    {
      return false;
    }



  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock,
			     ( struct sockaddr * ) &m_addr,
			     sizeof ( m_addr ) );


  if ( bind_return == -1 )
    {
      return false;
    }

  return true;
}

//PRE:
//POST: listen to the connections on the same port, up to MAXCONNECTIONS.
//      Return false if we go past max connections or if we fail to listen
bool Socket::listen() const
{
  if ( ! is_valid() )
    {
      return false;
    }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );


  if ( listen_return == -1 )
    {
      return false;
    }

  return true;
}

//PRE: a new socket object
//POST: uses the sys/socket.h accept to allow for a connection of socket.
//      Returns true if acception worked, false otherwise
//NOTE: server function only
bool Socket::accept ( Socket& new_socket ) const
{
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
  else
    return true;
}

//PRE: given a message to send. Assumes we have a connection
//POST: uses the sys/socket.h to send message to our client, try to send message.
//      return false if failed to send message, true otherwise
bool Socket::send ( const std::string s ) const
{
  int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
  if ( status == -1 )
    {
      return false;
    }
  else
    {
      return true;
    }
}

//PRE: given a message to send. Assumes we have a connection. Also accepts flag for
//     requesting more info
//POST: uses the sys/socket.h to send message to our client, try to send message.
//      return false if failed to send message, true otherwise
bool Socket::send ( const std::string s, const bool FLAG ) const
{
  int status;
  if(FLAG == MORE){
    status = ::send ( m_sock, s.c_str(), s.size(), MSG_MORE );
  }else{
    status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
  }
  if ( status == -1 )
    {
      return false;
    }
  else
    {
      return true;
    }
}

//PRE: the message we want to send, the host ip address, and the port 
//     number of that host
//POST: sends a message to the a given host and port number. Returns true if 
//      the send succeeds, false otherwise
bool Socket::sendTo(const std::string s, std::string host, const int port) {

	//create address to send our message to
	struct sockaddr_in remaddr;
	memset((char *)&remaddr, 0, sizeof(remaddr));
	int slen = sizeof(remaddr);
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(port);

	char * ip = (char *)host.c_str();
	
	if (inet_aton(ip, &remaddr.sin_addr) == 0) {
		return false;
	}

	//send the message
	int status = ::sendto(m_sock, s.c_str(), s.size(), MSG_NOSIGNAL, 
							(struct sockaddr *)&remaddr, slen);
	if (status == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//PRE: the message we want to send, the host ip address, and the port 
//     number of that host
//POST: sends a message to the a given host and port number. Returns true if 
//      the send succeeds, false otherwise
bool Socket::sendTo(const std::string s, const int host,
	const int port) {
	//create address to send our message to
	struct sockaddr_in remaddr;
	memset((char *)&remaddr, 0, sizeof(remaddr));
	int slen = sizeof(remaddr);
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(port);

	remaddr.sin_addr = host;

	//send the message
	int status = ::sendto(m_sock, s.c_str(), s.size(), MSG_NOSIGNAL,
		(struct sockaddr *)&remaddr, slen);
	if (status == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//PRE: a string buffer to hold our message. Assumes we have a connection
//POST: uses sys/socket.h to recv a message from the paired socket. If
//      the recv succeeds, string s will hold the message, else it will return the status code
int Socket::recv ( std::string& s ) const
{
  char buf [ MAXRECV + 1 ];

  s = "";

  memset ( buf, 0, MAXRECV + 1 );

  int status = ::recv ( m_sock, buf, MAXRECV, 0 );

  if ( status == -1 )
    {
      cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
      return 0;
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      s = buf;
      return status;
    }
}

//PRE: the the string we want to store our message in
//POST: returns the number of bytes recieved and stores the message in s.
//      -1 is returned if there was an error.
//NOTE: remaddr is populated for later use if we need the IP address
int Socket::recvFrom(std::string& s) {
	char buf[MAXRECV + 1];

	s = "";
	memset(buf, 0, MAXRECV + 1);

	int recvlen = ::recvfrom(m_sock, buf, MAXRECV, 0, 
				 (struct sockaddr *)&remaddr, &addrlen);
	if (recvlen == -1) {
	  //printf("There was an error in recvfrom.");
	}
	else {
		s = buf;
	}
	return (recvlen);
}

//PRE: assumes that remaddr has a value (AKA recvMessage called)
//POST: returns the IP address of the remote address
int  Socket::getRemoteIP() {
  return (remaddr.sin_addr.s_addr);
	

}

//same as above except return the string formatted IP and butts
std::string Socket::getRemoteIP(int dummy){
  char buf[MAXRECV + 1];
  std::string s = "";
  memset(buf, 0, MAXRECV + 1);
  
  inet_ntop(AF_INET, &(remaddr.sin_addr), buf, INET_ADDRSTRLEN);

  s = buf;
  
  return(s);
}


//PRE: take the name of the host and the host's port
//POST: connects our socket with the server's socket.
//      Returns true if we successfully connected. false otherwise
//NOTE: Client function only 
bool Socket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;
  //Set to IPv4 on port
  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}

//PRE: bool to block
//POST: sets blocking based on the bool
void Socket::set_non_blocking ( const bool b )
{

  int opts;

  //manipulate a file descripter, in this case our socket
  opts = fcntl ( m_sock,
		 F_GETFL );

  if ( opts < 0 )
    {
      return;
    }

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock,
	  F_SETFL,opts );

}
