#include "Plane.h"
#include "Hit.h"
#include "Ray.h"

unique_ptr<Hit> Plane::intersect(const Ray& ray, double t0, double t1) const {
  Vector v = ray.getDirection();
  Point q = ray.getPoint();

  double t = (normal.dot(point - q)) / (normal.dot(v));
  if (t > t0 && t < t1) {
    return unique_ptr<Hit>(new Hit(*this, t));
  }
  return unique_ptr<Hit>();
}

Vector Plane::calculateNormal(const Point& surfacePoint) const {
  return normal;
}
