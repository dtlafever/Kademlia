
GCC = g++ -std=c++11
RM = rm

UI: UIerror.o UserInterface.o UImain.o Message.o Socket.o UDPSocket.o
	$(GCC) UIerror.o UserInterface.o UImain.o Message.o Socket.o UDPSocket.o -o UI

Message.o: ../Message.cpp ../Message.hpp
	$(GCC) -c ../Message.cpp

Socket.o: ../Socket.cpp ../Socket.h
	$(GCC) -c ../Socket.cpp

UDPSocket.o: ../UDPSocket.cpp ../UDPSocket.h
	$(GCC) -c ../UDPSocket.cpp

UIerror.o: UIerror.cc UIerror.h
	$(GCC) -c UIerror.cc

UserInterface.o: UserInterface.h UserInterface.cc
	$(GCC) -c UserInterface.cc

UImain.o: UImain.cc
	$(GCC) -c UImain.cc

clean:
	$(RM) *.o ./LOGS/* UI *~
