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
#include "Vector.h"
#include <cmath>
#include <sstream>

Vector Vector::operator*(double scalar) const {
  return Vector(scalar * coord[X],
               scalar * coord[Y],
               scalar * coord[Z]);
}

Vector Vector::operator/(double scalar) const {
  return Vector(coord[X] / scalar,
                coord[Y] / scalar,
                coord[Z] / scalar);
}

Vector Vector::operator+(const Vector& other) const {
  return Vector(coord[X] + other[X],
                coord[Y] + other[Y],
                coord[Z] + other[Z]);
}

Vector Vector::operator+(const Point& other) const {
  return Vector(coord[X] + other[X],
                coord[Y] + other[Y],
                coord[Z] + other[Z]);
}

Vector Vector::operator-(const Vector& other) const {
  return Vector(coord[X] - other[X],
                coord[Y] - other[Y],
                coord[Z] - other[Z]);
}

Vector Vector::operator-() const {
  return Vector(-coord[X], -coord[Y], -coord[Z]);
}

bool Vector::equals(const Vector& other, double err) const {
  return (coord[X] <= other[X]+err && coord[X] >= other[X]-err) &&
         (coord[Y] <= other[Y]+err && coord[Y] >= other[Y]-err) &&
         (coord[Z] <= other[Z]+err && coord[Z] >= other[Z]-err);
}

bool Vector::operator==(const Vector& other) const {
  return coord[X] == other[X] &&
         coord[Y] == other[Y] &&
         coord[Z] == other[Z];
}

bool Vector::operator!=(const Vector& other ) const {
  return !(*this == other);
}

double Vector::dot(const Vector& other) const {
  return coord[X] * other[X] +
         coord[Y] * other[Y] +
         coord[Z] * other[Z];
}

Vector Vector::cross(const Vector& other) const {
  return Vector(coord[Y]*other.coord[Z] - coord[Z]*other.coord[Y],
                coord[Z]*other.coord[X] - coord[X]*other.coord[Z],
                coord[X]*other.coord[Y] - coord[Y]*other.coord[X]);
}
double Vector::length() const {
  return sqrt(coord[X] * coord[X] +
              coord[Y] * coord[Y] +
              coord[Z] * coord[Z]);
}

Vector Vector::normalize() const {
  return (*this) / length();
}

string Vector::toString() const {
  ostringstream s;
  s << "Vector" << Cartesian::toString();
  return s.str();
}

ostream& operator<<(ostream& os, const Vector& p) {
  os << p.toString();
  return os;
}

Vector operator*(double scalar, const Vector& other) {
  return other * scalar;
}

Vector operator-(const Point& p1, const Point& p2) {
  return Vector(p1[X] - p2[X],
                p1[Y] - p2[Y],
                p1[Z] - p2[Z]);
}

Point operator-(const Point& p, const Vector& v) {
  return Point(p[X] - v[X],
               p[Y] - v[Y],
               p[Z] - v[Z]);
}


Point operator+(const Point& p, const Vector& v) {
  return Point(p[X] + v[X],
               p[Y] + v[Y],
               p[Z] + v[Z]);
}
