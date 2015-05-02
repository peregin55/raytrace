/* Copyright 2015 Stephen H. Johnson

  This file is part of raytrace.

  raytrace is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  raytrace is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with raytrace.  If not, see <http://www.gnu.org/licenses/>.
*/
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
