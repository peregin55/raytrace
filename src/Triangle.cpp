#include "Triangle.h"
#include "Plane.h"
#include "Hit.h"
#include "Ray.h"

Triangle::Triangle(const string& materialName, const Point& p0, const Point& p1, const Point& p2) :
      Surface(materialName), p0(p0), p1(p1), p2(p2), normal((p1 - p0).cross(p2 - p0).normalized()) { }

unique_ptr<Hit> Triangle::intersect(const Ray& ray, double t0, double t1) const {
  Plane p(getMaterialName(), p0, normal);
  unique_ptr<Hit> planeHit = p.intersect(ray, t0, t1);
  if (planeHit) {
    // in-out test, if intersect is inside the triangle, the plane formed by it
    // and edgeVector will have a normal vector in same direction as the original
    // plane.  If outside the vector, the normal vector will be -planeNormal vector.
    Point intersect = ray.getPoint(planeHit->getT());
    if (isContained(p0, p1, intersect) &&
        isContained(p1, p2, intersect) &&
        isContained(p2, p0, intersect)) {
      return unique_ptr<Hit>(new Hit(*this, planeHit->getT()));
    }
  }
  return unique_ptr<Hit>();
}

bool Triangle::isContained(const Point& p0, const Point& p1, const Point& intersect) const {
  Vector edgeVector = p1 - p0;
  Vector intersectVector = intersect - p0;
  Vector intersectNormal = edgeVector.cross(intersectVector);
  return intersectNormal.dot(normal) >= 0.0;
} 

Vector Triangle::calculateNormal(const Point& surfacePoint) const {
  return normal;
}
