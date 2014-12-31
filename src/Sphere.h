#ifndef SPHERE_H
#define  SPHERE_H

#include <iostream>
#include <memory>
#include <string>
#include "Point.h"
#include "Surface.h"
using namespace std;

class Hit;
class Matrix4;
class Ray;

/** Sphere surface defined by center location and radius. */
class Sphere : public Surface {
  public:
    Sphere(const string& materialName, const Point& c, double r) : Surface(materialName), center(c), radius(r) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
  private:
    unique_ptr<Hit> buildHit(const Ray& ray, double t) const;
    Point center;
    double radius;
};
#endif
