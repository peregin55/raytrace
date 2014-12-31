#ifndef PLANE_H
#define  PLANE_H

#include <iostream>
#include <memory>
#include <string>
#include "Point.h"
#include "Vector.h"
#include "Surface.h"
using namespace std;

class Matrix4;
class Ray;

/** Infinite plane surface. */
class Plane : public Surface {
  public:
    Plane(const string& materialName, const Point& p, const Vector& normal) :
      Surface(materialName), point(p), normal(normal.normalize()) {}
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
  private:
    Point point;
    Vector normal;
};
#endif
