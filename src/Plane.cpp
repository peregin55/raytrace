#include "Plane.h"
#include "Hit.h"
#include "Ray.h"

unique_ptr<Hit> Plane::intersect(const Ray& ray, double t0, double t1) const {
  Point q = ray.getPoint();
  Vector v = ray.getDirection();

  double t = (normal.dot(p0 - q)) / (normal.dot(v));
  if (t > t0 && t < t1) {
    return unique_ptr<Hit>(new Hit(*this, t));
  }
  return unique_ptr<Hit>();
}

Vector Plane::calculateNormal(const Point& hitpoint) const {
  return normal;
}
