#include "constants.h"

int Sign(float Val)
{
  if(Val >= 0)  return 1;
  else if(Val < 0) return -1;
  else return 0;
}

double abs(double Val)
{
  if (Val >= 0.)  return Val;
  else return -Val;
}
