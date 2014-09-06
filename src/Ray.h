#ifndef RAY_H
#define  RAY_H

#include <iostream>
#include "Point.h"
#include "Vector.h"
using namespace std;

class Ray {
  public:
    Ray(Point p, Vector v) : point(p), direction(v.normalized()) { }
    const Point& getPoint() const { return point; }
    const Vector& getDirection() const { return direction; }
  private:
    Point point;
    Vector direction;

  friend ostream& operator<<(ostream& os, const Ray& other) {
    os << "[" << other.getPoint() << ", " << other.getDirection() << "]";
    return os;
  }
};
#endif
