#ifndef INCLUDED_UIerror
#define INCLUDED_UIerror

#include <iostream>

using namespace std;

#define MAXERRORSIZE 40

class UIerror {

 private:
  char * msg;
  
 public:
  UIerror (char * errorMsg);

  void print_error () { cout << msg << endl; };

};

#endif
