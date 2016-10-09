#ifndef INCLUDED_ErrorClass
#define INCLUDED_ErrorClass

#include <iostream>

using namespace std;

#define MAXERRORSIZE 40

class ErrorClass {

 private:
  char * msg;
  
 public:
  ErrorClass (char * errorMsg);

  void print_error () { cout << msg << endl; };

};

#endif
