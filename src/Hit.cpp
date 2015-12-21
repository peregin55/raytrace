#include "Hit.h"

const Surface* Hit::getSurface() const {
  return surfacePath.front();
}

vector<const Surface*> Hit::getSurfacePath() const {
  return surfacePath;
}

double Hit::getT() const {
  return t;
}

const Hit& Hit::min(const Hit& h1, const Hit& h2) {
  if (h1.getT() <= h2.getT()) {
    return h1;
  } else {
    return h2;
  }
}

const Hit& Hit::max(const Hit& h1, const Hit& h2) {
  if (h1.getT() >= h2.getT()) {
    return h1;
  } else {
    return h2;
  }
}
