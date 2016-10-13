#include "dummy.h"
#include <thread>
#include <iostream>
#include <future>
#include <chrono>

#include "stupid.h"

Dummy::Dummy() {
  number1 = 3;
  number2 = 13;
}

int Dummy::start_thread() {
  
  // std::thread t(&Dummy::switchval, this);
  // std::thread t2(&Dummy::other, this);
  // t.join();
  // t2.join();
  // std::cout << "n1: " << number1 << " n2: " << number2 << std::endl;
  return(0);
}
  
void Dummy::switchval() {
  number1 = 20;
  number2 = 40;
  std::cerr << "t" << std::endl;
  std::cerr << "==========" << std::endl;
  std::cerr << "n1: " << number1 << " n2: " << number2 << std::endl;
  std::cerr << "==========" << std::endl;
  int temp = number1;
  number1 = number2;
  number2 = temp;
}

void Dummy::other() {
  number1 = -1;
  std::cerr << "t2" << std::endl;
  int temp = number1;
  number1 = number2;
  number2 = temp;
}

void Dummy::fuuture() {
  //
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::chrono::duration<int, std::milli> wait_duration (20);
  std::chrono::system_clock::time_point wait_time = now + wait_duration;
  int counter = 0;
  bool looping = false;
  std::future<void> badname = std::async(&Dummy::other, this);
  while (std::future_status::ready != badname.wait_until(wait_time)) {
    counter++;
  }
  badname.get();
  std::cerr << "number1: " << number1 << " number2: " << number2 << std::endl;
  std::cerr << "and it took us " << counter << " wait_until's!" << std::endl;
}
