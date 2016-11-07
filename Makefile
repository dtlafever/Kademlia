GCC = g++ -std=c++11

main: Message.o KBucket.o RoutingTable.o MsgTimer.o Socket.o UDPSocket.o SnapShot.o JoinNetworkQueue.o Node.o main.o
	$(GCC) Message.o KBucket.o RoutingTable.o MsgTimer.o Socket.o UDPSocket.o SnapShot.o JoinNetworkQueue.o Node.o main.o -o main -pthread
Message.o: Message.cpp
	$(GCC) -c Message.cpp
KBucket.o: KBucket.cc
	$(GCC) -c KBucket.cc
RoutingTable.o: RoutingTable.cc
	$(GCC) -c RoutingTable.cc
MsgTimer.o: MsgTimer.cpp
	$(GCC) -c MsgTimer.cpp
Socket.o: Socket.cpp
	$(GCC) -c Socket.cpp
UDPSocket.o: UDPSocket.cpp
	$(GCC) -c UDPSocket.cpp
SnapShot.o: SnapShot.cc
	$(GCC) -c SnapShot.cc
JoinNetworkQueue.o: JoinNetworkQueue.cpp
	$(GCC) -c JoinNetworkQueue.cpp
Node.o: Node.cpp
	$(GCC) -c Node.cpp
main.o: main.cpp
	$(GCC) -c main.cpp
clean:                                                                          
	rm -r *.o *.log main *~ ./Logs/*
