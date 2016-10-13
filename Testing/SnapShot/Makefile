GCC = g++ -std=c++11
RM = rm

MAIN: SnapShot.o main.o 
	$(GCC) SnapShot.o main.o -o MAIN

SnapShot.o: SnapShot.cc constants.h
	$(GCC) -c SnapShot.cc

main.o: main.cpp constants.h
	$(GCC) -c main.cpp

clean:
	$(RM) SnapShot.o main.o MAIN *~
