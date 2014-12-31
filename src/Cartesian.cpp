#include "Cartesian.h"
#include <sstream>

Cartesian::Cartesian() {
  coord[X] = coord[Y] = coord[Z] = 0.0;
}

Cartesian::Cartesian(double x, double y, double z) { 
  coord[X] = x;
  coord[Y] = y;
  coord[Z] = z;
}

Cartesian::~Cartesian() {
}

double Cartesian::operator[](Coordinate c) const {
  return coord[c];
}

string Cartesian::toString() const {
  ostringstream s;
  s << "(" << coord[X] << ", " << coord[Y] << ", " << coord[Z] << ")";
  return s.str();
}
