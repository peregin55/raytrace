#include "Point.h"
#include <sstream>

Point::~Point() {
}

string Point::toString() const {
  ostringstream s;
  s << "Point" << Cartesian::toString();
  return s.str();
}

ostream& operator<<(ostream& os, const Point& p) {
  os << p.toString();
  return os;
}
