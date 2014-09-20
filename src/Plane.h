#ifndef PLANE_H
#define  PLANE_H

#include <iostream>
#include <memory>
#include <string>
#include "Point.h"
#include "Vector.h"
#include "Surface.h"
using namespace std;

class Ray;

class Plane : public Surface {
  public:
    Plane(const string& materialName, const Point& p, const Vector& normal) : Surface(materialName), point(p), normal(normal.normalized()) {}
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Vector calculateNormal(const Point& surfacePoint) const;
  private:
    Point point;
    Vector normal;
};
#endif
