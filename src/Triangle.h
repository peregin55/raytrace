#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>
#include <string>
#include "Plane.h"
#include "Point.h"
#include "Surface.h"
#include "Vector.h"
#include "Matrix4.h"
using namespace std;

class Hit;
class Matrix4;
class Ray;
class Texture;

/** Triangle surface. */
class Triangle : public Surface {
  public:
    Triangle(shared_ptr<Material> material, shared_ptr<Texture> texture, const Point& p0, const Point& p1, const Point& p2) :
      Surface(material, texture), p0(p0), p1(p1), p2(p2), plane(material, texture, p0, p1, p2), normal(plane.calculateNormal(p0)) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint) const;
  private:
    bool isContained(const Point& q0, const Point& q1, const Point& intersect) const;
    Point p0, p1, p2;
    Plane plane;
    Vector normal;
};
#endif
