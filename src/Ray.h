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
#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "Point.h"
#include "Vector.h"
using namespace std;

/** Ray in xyz space, defined by a starting location and direction vector. */
class Ray {
  public:
    Ray() { }
    Ray(const Point& p, const Vector& v) : point(p), direction(v) { }
    const Point& getPoint() const { return point; }
    const Vector& getDirection() const { return direction; }
    Point calculatePoint(double t) const { return point + direction * t; }
  private:
    Point point;
    Vector direction;

  friend ostream& operator<<(ostream& os, const Ray& other) {
    os << "[" << other.getPoint() << ", " << other.getDirection() << "]";
    return os;
  }
};
#endif
