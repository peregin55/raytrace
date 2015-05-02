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
#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <string>
#include "Cartesian.h"
#include "Point.h"

/** Vector in xyz space. */
class Vector : public Cartesian {
  public:
    Vector() : Cartesian() { }
    Vector(double x, double y, double z) : Cartesian(x, y, z) { }
    Vector operator*(double scalar) const;
    Vector operator/(double scalar) const;
    Vector operator+(const Vector& other) const;
    Vector operator+(const Point& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator-() const;
    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;
    double dot(const Vector& other) const;
    Vector cross(const Vector& other) const;
    double length() const;
    Vector normalize() const;
    virtual string toString() const;
  friend ostream& operator<<(ostream& os, const Vector& other);
};
Vector operator*(double scalar, const Vector& other);
Vector operator-(const Point& p1, const Point& p2);
Point operator+(const Point& p, const Vector& v);
#endif
