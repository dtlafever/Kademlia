#include "UIerror.h"
#include <string.h>

UIerror::UIerror (char * errorMsg) {
  msg = new char[MAXERRORSIZE];
  strcpy (msg, errorMsg);

}
