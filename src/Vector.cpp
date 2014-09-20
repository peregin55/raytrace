#include "Vector.h"
#include <cmath>
#include <sstream>

Vector::~Vector() {
}

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

Vector Vector::normalized() const {
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

Point operator+(const Point& p, const Vector& v) {
  return Point(p[X] + v[X],
               p[Y] + v[Y],
               p[Z] + v[Z]);
}
