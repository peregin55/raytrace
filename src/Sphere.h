#ifndef SPHERE_H
#define  SPHERE_H

#include <iostream>
#include <memory>
#include <string>
#include "Point.h"
#include "Matrix4.h"
#include "Surface.h"
using namespace std;

class Hit;
class Texture;
class Matrix4;
class Ray;
class Vector;

/** Sphere surface defined by center location and radius. */
class Sphere : public Surface {
  public:
    Sphere(const Point& c, double r, shared_ptr<Material> material, shared_ptr<Texture> texture):
      Surface(material, texture), center(c), radius(r), world2obj(translate(-c[X], -c[Y], -c[Z])) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint) const;
  private:
    Point center;
    double radius;
    Matrix4 world2obj;
};
#endif
