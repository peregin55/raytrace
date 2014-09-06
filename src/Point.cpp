#include "Point.h"
#include <sstream>

Point::~Point() {
}

string Point::toString() const {
  ostringstream s;
  s << "Point" << AbstractPosition::toString();
  return s.str();
}

ostream& operator<<(ostream& os, const Point& p) {
  os << p.toString();
  return os;
}
