#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>
#include <string>
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
      Surface(material, texture), p0(p0), p1(p1), p2(p2) {
        Vector v1 = p1 - p0;
        Vector v2 = p2 - p0;
        Vector v3 = v1.cross(v2);
        Matrix4 bary2world = Matrix4(v1[X], v2[X], v3[X], p0[X],
                                    v1[Y], v2[Y], v3[Y], p0[Y],
                                    v1[Z], v2[Z], v3[Z], p0[Z],
                                    0.0,   0.0,   0.0,   1.0);
        world2Barycentric = bary2world.inverse();
        normal = v3.normalize();
    }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint) const;
  private:
    bool isContained(const Point& q0, const Point& q1, const Point& intersect) const;
    Point p0, p1, p2;
    Vector normal;
    Matrix4 world2Barycentric;
};
#endif
