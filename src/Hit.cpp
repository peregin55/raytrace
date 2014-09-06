#include "Hit.h"

const Surface& Hit::getSurface() const {
  return surface;
}

double Hit::getT() const {
  return t;
}
