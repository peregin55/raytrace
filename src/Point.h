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
#ifndef POINT_H
#define POINT_H

#include <string>
#include "Cartesian.h"

/** Point in xyz space. */
class Point : public Cartesian {
  public:
    Point() : Cartesian() { }
    Point(double x, double y, double z) : Cartesian(x, y, z) { }
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    virtual string toString() const;
  friend ostream& operator<<(ostream& cout, const Point& p);
};
namespace std {
  template<>
  struct hash<Point> {
    public:
      size_t operator()(const Point& p) const;
    private:
      static const unsigned int MAGIC;
  };
}
#endif
