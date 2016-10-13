#ifndef dummy_include
#define dummy_include
// this is a dummy class which we will try in threads

class Dummy {
  
 private:
  int number1;
  int number2;

 public:
  
  //Default Constructor
  
  /*
     PRE: 
    POST: sets number1 to 3 and number2 to 13
  */
  Dummy();
  
  /*
     PRE: expects number1 to be 3 and number2 to be 13
    POST: starts a thread process to change number1 to 13 and number2 to 3
  */
  int start_thread();

  void switchval();
  void other();
  void fuuture();
};

#endif
