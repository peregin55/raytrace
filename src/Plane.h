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
    Plane(shared_ptr<Material> material, const Point& p, const Vector& normal) :
      Surface(material), point(p), normal(normal.normalize()) {}
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
  private:
    Point point;
    Vector normal;
};
#endif
