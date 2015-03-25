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
    Plane(shared_ptr<Material> material, const Point& p0, const Point &p1, const Point& p2) :
      Surface(material), p0(p0), p1(p1), p2(p2) {
        u0 = (p1 - p0).normalize();
        Vector v1 = p0 - p2;
        Vector proj = (v1.dot(u0)) * u0;
        u1 = (v1 - proj).normalize();
        u2 = u0.cross(u1).normalize();
        normal = -u2;
    }

    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
  private:
    Point p0;
    Point p1;
    Point p2;
    Vector u0;
    Vector u1;
    Vector u2;
    Vector normal;
};
#endif
