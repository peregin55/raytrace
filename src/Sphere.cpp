#include <cmath>
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"

unique_ptr<Hit> Sphere::intersect(const Ray& ray, double t0, double t1) const {
  Vector d = ray.getDirection();
  Point e = ray.getPoint();
  Vector centerToEye = e - center;
  double dDotted = d.dot(d);
  double determinant = sqrt(pow(d.dot(e - center), 2.0) - (dDotted) * 
                            ((centerToEye).dot(centerToEye) - pow(radius, 2.0)));
  if (determinant >= 0) {
    double minusB = (-d).dot(centerToEye);
    double tPos = (minusB + determinant) / dDotted;
    double tNeg = (minusB - determinant) / dDotted;
    if (tPos > t0 && tPos < t1 && tNeg > t0 && tNeg < t1) {
      return unique_ptr<Hit>(new Hit(*this, fmin(tNeg, tPos)));
    }
    else if (tPos > t0 && tPos < t1) {
      return unique_ptr<Hit>(new Hit(*this, tPos));
    }
    else if (tNeg > t0 && tNeg < t1) {
      return unique_ptr<Hit>(new Hit(*this, tNeg));
    }
  }
  return unique_ptr<Hit>();
}

Vector Sphere::calculateNormal(const Point& surfacePoint) const {
  return (surfacePoint - center).normalized();
}
