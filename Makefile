GCC = g++ -std=c++11

RoutingTableTest: RoutingTable.o KBucket.o main.o
	$(GCC) RoutingTable.o KBucket.o -o main.o RoutingTableTest
main.o: main.cc RoutingTable.h
	$(GCC) -c main.cc
RoutingTable.o: KBucket.h constants.h RoutingTable.h RoutingTable.cc
	$(GCC) -c RoutingTable.cc
KBucket.o: KBucket.h KBucket.cc constants.h
	$(GCC) -c KBucket.cc
clean:
	rm RoutingTableTest
	rm *.o
	rm *~
