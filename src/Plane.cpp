#include "Plane.h"
#include "Hit.h"
#include "Ray.h"
#include "Texture.h"
#include <cmath>

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

Color Plane::textureColor(const Point& hitpoint) const {
  if (texture) {
    Point local = world2plane * hitpoint;
    return texture->colorAt(mapInfinitely(local[X]), mapInfinitely(local[Y]));
  }
  return Color();
}

double Plane::mapInfinitely(double x) const {
  double frac = x - (long)x;
  if (x >= 0) {
    return frac;
  }
  if (1.0 + frac < -100) {
    cerr << "bad x = " << x << ", cap x = " << (long)x << " with frac = " << frac << endl;
  }
  return 1.0 + frac;
}
