#include <math.h>
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"
extern bool shouldDebug;
static double determineT(double tPos, double tNeg, double t0, double t1);

unique_ptr<Hit> Sphere::intersect(const Ray& ray, double t0, double t1) const {
  Vector d = ray.getDirection();
  Point e = ray.getPoint();
  Vector centerToEye = e - center;
  double dDotted = d.dot(d);
  double determinant = sqrt(pow(d.dot(e - center), 2.0) - (dDotted) * 
                            ((centerToEye).dot(centerToEye) - pow(radius, 2.0)));
  if (shouldDebug) {
    cerr << "Sphere: center: " << center << "\n";
    cerr << "Sphere: radius: " << radius << "\n";
    cerr << "Sphere: determinant: " << determinant << "\n";
    cerr << "Sphere: d: " << d << "\n";
    cerr << "Sphere: dDotted: " << dDotted << "\n";
  }
  if (determinant >= 0) {
    double minusB = (-d).dot(centerToEye);
    double tPos = (minusB + determinant) / dDotted;
    double tNeg = (minusB - determinant) / dDotted;
    if (shouldDebug) {
      cerr << "Sphere: ts: " << tPos << " " << tNeg << "\n";
    }
    double t = determineT(tPos, tNeg, t0, t1);
   if (t != t0) {
      return unique_ptr<Hit>(new Hit(*this, t));
    }
  }
  return unique_ptr<Hit>();
}

static double determineT(double tPos, double tNeg, double t0, double t1) {
  if (tPos > t0 && tPos < t1 && tNeg > t0 && tNeg < t1) {
    return fmin(tNeg, tPos);
  }
  else if (tPos > t0 && tPos < t1) {
    return tPos;
  }
  else if (tNeg > t0 && tNeg < t1) {
    return tNeg;
  }
  else {
    return t0;
  }
} 

Vector Sphere::calculateNormal(const Point& surfacePoint) const {
  return (surfacePoint - center).normalized();
}
