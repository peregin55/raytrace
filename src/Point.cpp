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
#include "Point.h"
#include <sstream>

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
