#ifndef PLANE_H
#define  PLANE_H

#include <iostream>
#include <memory>
#include <string>
#include "Matrix4.h"
#include "Point.h"
#include "Vector.h"
#include "Surface.h"
using namespace std;

class Material;
class Texture;
class Ray;

/** Infinite plane surface. */
class Plane : public Surface {
  public:
    Plane(shared_ptr<Material> material,
          shared_ptr<Texture> texture,
          const Point& p0,
          const Vector& normal,
          const Matrix4& world2plane) :
      Surface(material, texture),
      p0(p0),
      normal(normal),
      world2plane(world2plane) { }
    Plane(shared_ptr<Material> material,
          shared_ptr<Texture> texture,
          const Point& p0,
          const Point& p1,
          const Point& p2) :
      Surface(material, texture), p0(p0) {
        Vector v0 = p1 - p0;
        Vector u0 = v0.normalize();
        Vector v1 = p2 - p0;
        Vector proj = (v1.dot(u0)) * u0;
        Vector v0Perp = v1 - proj;
        Vector v2 = v0.cross(v0Perp);
        normal = v2.normalize();
        world2plane = Matrix4(v0[X], v0Perp[X], v2[X], p0[X],
                              v0[Y], v0Perp[Y], v2[Y], p0[Y],
                              v0[Z], v0Perp[Z], v2[Z], p0[Z],
                              0.0,   0.0,   0.0,   1.0).inverse();
    }

    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint) const;
  private:
    double mapInfinitely(double x) const;
    Point p0;
    Vector normal;
    Matrix4 world2plane;
};
#endif
