Node.o: Node.cc constants.h
	g++ -c node.cc
Message.o: Message.cpp Message.hpp constants.h
	g++ -c Message.cpp
UserInterface.o: UserInterace.cc UserInterface.h constants.h
	g++ -c UserInterface.cc
KBucket.o: KBucket.cc KBucket.h constants.h
	g++ -c KBucket.cc
RoutingTable.o: RoutingTable.cc RoutingTable.h KBucket.h constants.h
	g++ -c RoutingTable.cc

