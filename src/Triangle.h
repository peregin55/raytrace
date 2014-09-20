#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>
#include <string>
#include "Point.h"
#include "Surface.h"
#include "Vector.h"
using namespace std;

class Hit;
class Ray;

class Triangle : public Surface {
  public:
    Triangle(const string& materialName, const Point& p0, const Point& p1, const Point& p2);
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Vector calculateNormal(const Point& surfacePoint) const;
  private:
    bool isContained(const Point& p0, const Point& p1, const Point& intersect) const;
    Point p0, p1, p2;
    Vector normal;
};
#endif
