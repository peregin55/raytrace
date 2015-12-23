#include "Hit.h"

const Surface* Hit::getSurface() const {
  return surfacePath.back();
}

const vector<const Surface*> Hit::getSurfaceStack() const {
  return surfacePath;
}

Hit Hit::pushSurface(const Surface* s) const {
  vector<const Surface*> v = surfacePath;
  v.push_back(s);
  return Hit(v, t);
}

Hit Hit::popSurface() const {
  vector<const Surface*> v = surfacePath;
  v.pop_back();
  return Hit(v, t);
}

double Hit::getT() const {
  return t;
}

bool Hit::operator<=(const Hit& other) const {
  return getT() <= other.getT();
}

bool Hit::operator>=(const Hit& other) const {
  return getT() >= other.getT();
}

bool Hit::operator<(const Hit& other) const {
  return getT() < other.getT();
}

bool Hit::operator>(const Hit& other) const {
  return getT() > other.getT();
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
