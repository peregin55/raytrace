#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <string>
#include "AbstractPosition.h"
#include "Point.h"

class Vector : public AbstractPosition {
  public:
    Vector(double x, double y, double z) : AbstractPosition(x, y, z) { }
    Vector(const Vector& other) : AbstractPosition(other[X], other[Y], other[Z]) { }
    virtual ~Vector();
    Vector operator*(double scalar) const;
    Vector operator/(double scalar) const;
    Vector operator+(const Vector& other) const;
    Vector operator+(const Point& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator-() const;
    double dot(const Vector& other) const;
    Vector cross(const Vector& other) const;
    double length() const;
    Vector normalized() const;
    virtual string toString() const;
};
ostream& operator<<(ostream& os, const Vector& other);
Vector operator*(double scalar, const Vector& other);
Vector operator-(const Point& p1, const Point& p2);
Point operator+(const Point& p, const Vector& v);
#endif
