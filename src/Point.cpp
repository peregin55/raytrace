#include "Point.h"
#include <sstream>

Point::~Point() {
}

bool Point::operator==(const Point& other) const {
  return coord[X] == other[X] &&
         coord[Y] == other[Y] &&
         coord[Z] == other[Z];
}

bool Point::operator!=(const Point& other) const {
  return !(*this == other);
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
