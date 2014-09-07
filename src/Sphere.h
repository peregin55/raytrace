#ifndef SPHERE_H
#define  SPHERE_H

#include <iostream>
#include <string>
#include "Point.h"
#include "Surface.h"
using namespace std;

class Ray;

class Sphere : public Surface {
  public:
    Sphere(const string& materialName, const Point& c, double r) : Surface(materialName), center(c), radius(r) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Vector calculateNormal(const Point& surfacePoint) const;
  private:
    const Point center;
    const double radius;
};
#endif
