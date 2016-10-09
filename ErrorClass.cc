#include "ErrorClass.h"
#include <string.h>

ErrorClass::ErrorClass (char * errorMsg) {
  msg = new char[MAXERRORSIZE];
  strcpy (msg, errorMsg);
}
