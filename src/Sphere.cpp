#include <cmath>
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"

unique_ptr<Hit> Sphere::intersect(const Ray& ray, double t0, double t1) const {
  Point e = ray.getPoint();
  Vector d = ray.getDirection();
  Vector centerToEye = e - center;
  double dDotted = d.dot(d);
  double determinant = sqrt(pow(d.dot(e - center), 2.0) - (dDotted) * 
                            ((centerToEye).dot(centerToEye) - pow(radius, 2.0)));
  if (determinant >= 0) {
    double minusB = (-d).dot(centerToEye);
    double tPos = (minusB + determinant) / dDotted;
    double tNeg = (minusB - determinant) / dDotted;
    if (tPos > t0 && tPos < t1 && tNeg > t0 && tNeg < t1) {
      return buildHit(ray, fmin(tNeg, tPos));
    }
    else if (tPos > t0 && tPos < t1) {
      return buildHit(ray, tPos);
    }
    else if (tNeg > t0 && tNeg < t1) {
      return buildHit(ray, tNeg);
    }
  }
  return unique_ptr<Hit>();
}

unique_ptr<Hit> Sphere::buildHit(const Ray& ray, double t) const {
  Point hitpoint = ray.getPoint() + ray.getDirection() * t;
  Vector normal = (hitpoint - center).normalize();
  return unique_ptr<Hit>(new Hit(*this, t, hitpoint, normal));
}
