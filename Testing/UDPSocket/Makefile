GCC = g++ -std=c++11
RM = rm

RECV: Socket.o UDPSocket.o mainRECV.o
	$(GCC) Socket.o UDPSocket.o mainRECV.o -o RECV

SEND: Socket.o UDPSocket.o mainSEND.o
	$(GCC) Socket.o UDPSocket.o mainSEND.o -o SEND

Socket.o: Socket.cpp constants.h
	$(GCC) -c Socket.cpp

UDPSocket.o: UDPSocket.cpp SocketException.h Socket.h
	$(GCC) -c UDPSocket.cpp

mainRECV.o: mainRECV.cpp
	$(GCC) -c mainRECV.cpp

mainSEND.o: mainSEND.cpp
	$(GCC) -c mainSEND.cpp

clean:
	$(RM) Socket.o UDPSocket.o mainRECV.o mainSEND.o RECV SEND *~
