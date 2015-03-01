#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>
#include <string>
#include "Point.h"
#include "Surface.h"
#include "Vector.h"
using namespace std;

class Hit;
class Matrix4;
class Ray;

/** Triangle surface. */
class Triangle : public Surface {
  public:
    Triangle(shared_ptr<Material> material, const Point& p0, const Point& p1, const Point& p2) :
      Surface(material), p0(p0), p1(p1), p2(p2), normal((p1 - p0).cross(p2 - p0).normalize()) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
  private:
    bool isContained(const Point& p0, const Point& p1, const Point& intersect) const;
    Point p0, p1, p2;
    Vector normal;
};
#endif
