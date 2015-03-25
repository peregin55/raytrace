#include "Color.h"
#include "Triangle.h"
#include "Plane.h"
#include "Hit.h"
#include "Ray.h"
#include "Texture.h"

unique_ptr<Hit> Triangle::intersect(const Ray& ray, double t0, double t1) const {
  Plane p(material, p0, p1, p2);
  unique_ptr<Hit> planeHit = p.intersect(ray, t0, t1);
  if (planeHit) {
    // in-out test, if intersect is inside the triangle, the plane formed by it
    // and edgeVector will have a normal vector in same direction as the original
    // plane.  If outside the vector, the normal vector will be -planeNormal vector.
    Point intersect = ray.calculatePoint(planeHit->getT());
    if (isContained(p0, p1, intersect) &&
        isContained(p1, p2, intersect) &&
        isContained(p2, p0, intersect)) {
      return unique_ptr<Hit>(new Hit(*this, planeHit->getT()));
    }
  }
  return unique_ptr<Hit>();
}

bool Triangle::isContained(const Point& q0, const Point& q1, const Point& intersect) const {
  Vector edgeVector = q1 - q0;
  Vector intersectVector = intersect - q0;
  Vector intersectNormal = edgeVector.cross(intersectVector);
  return intersectNormal.dot(normal) >= 0.0;
}

Vector Triangle::calculateNormal(const Point& hitpoint) const {
  return normal;
}

Color Triangle::textureColor(const Point& hitpoint) const {
  Point local = world2Barycentric * hitpoint;
  if (texture) {
    return texture->colorAt(local[X], local[Y]);
  } else {
    return Color();
  }
}
