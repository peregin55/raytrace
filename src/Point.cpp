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

namespace std {
  const unsigned int hash<Point>::MAGIC = 0x9e3779b9;
  size_t hash<Point>::operator()(const Point& p) const {
    // rotating hash inspired by boost
    // http://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
    size_t seed = 0;
    seed ^= hash<double>()(p[X]) + MAGIC;
    seed ^= hash<double>()(p[Y]) + MAGIC + (seed << 6) + (seed >> 2);
    seed ^= hash<double>()(p[Z]) + MAGIC + (seed << 6) + (seed >> 2);
    return seed;
  }
}
