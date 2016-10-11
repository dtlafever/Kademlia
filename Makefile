RoutingTableTest: RoutingTable.o KBucket.o main.o
	g++ RoutingTable.o KBucket.o -o main.o RoutingTableTest
main.o: main.cc RoutingTable.h
	g++ -c main.cc
RoutingTable.o: KBucket.h constants.h RoutingTable.h RoutingTable.cc
	g++ -c RoutingTable.cc
KBucket.o: KBucket.h KBucket.cc constants.h
	g++ -c KBucket.cc
clean:
	rm RoutingTableTest
	rm *.o
	rm *~
