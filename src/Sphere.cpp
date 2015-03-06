#include <cmath>
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"
#include "Texture.h"
#include "Vector.h"

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

Vector Sphere::calculateNormal(const Point& hitpoint) const {
  return (hitpoint - center).normalize();
}

Color Sphere::textureColor(const Point& hitpoint) const {
  Point local = world2obj * hitpoint; 
  double rho = sqrt(local[X]*local[X] + local[Y]*local[Y] + local[Z]*local[Z]);
  double theta = atan(local[X]/local[Z]);
  double phi = acos(local[Y]/rho);
  if (texture) {
    double u = (theta + M_PI/2.0) / M_PI;
    double v = phi / M_PI;
    return texture->colorAt(u,v);
  }
  return Color();
}
