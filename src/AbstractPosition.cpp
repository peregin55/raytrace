#include "AbstractPosition.h"
#include <sstream>

AbstractPosition::AbstractPosition(double x, double y, double z) { 
  coord[X] = x;
  coord[Y] = y;
  coord[Z] = z;
}

AbstractPosition::~AbstractPosition() {
}

double AbstractPosition::operator[](Coordinate c) const {
  return coord[c];
}

string AbstractPosition::toString() const {
  ostringstream s;
  s << "(" << coord[X] << ", " << coord[Y] << ", " << coord[Z] << ")";
  return s.str();
}
