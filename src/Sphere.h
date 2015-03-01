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
class Vector;

/** Sphere surface defined by center location and radius. */
class Sphere : public Surface {
  public:
    Sphere(shared_ptr<Material> material, const Point& c, double r) : Surface(material), center(c), radius(r) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
  private:
    Point center;
    double radius;
};
#endif
