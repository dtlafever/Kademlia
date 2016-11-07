#include "UIerror.h"
#include <string.h>

//PRE: Object defined with char * array
//POST: Create a new character array with the input
//      and errorMsg is copied into array
UIerror::UIerror (char * errorMsg) {
  msg = new char[MAXERRORSIZE];
  strcpy (msg, errorMsg);
}
