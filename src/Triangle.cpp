/* Copyright 2015 Stephen H. Johnson

  This file is part of raytrace.

  raytrace is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  raytrace is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with raytrace.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Color.h"
#include "Triangle.h"
#include "Plane.h"
#include "Hit.h"
#include "Ray.h"
#include "Texture.h"
#include <limits>

bool Triangle::intersect(const Ray& ray, double t0, double t1, Hit& hit) const {
  Hit planeHit;
  if (plane.intersect(ray, t0, t1, planeHit)) {
    // in-out test, if intersect is inside the triangle, the plane formed by it
    // and edgeVector will have a normal vector in same direction as the original
    // plane.  If outside the vector, the normal vector will be -planeNormal vector.
    Point intersect = ray.calculatePoint(planeHit.getT());
    if (isContained(p0, p1, intersect) &&
        isContained(p1, p2, intersect) &&
        isContained(p2, p0, intersect)) {
      hit = Hit(this, planeHit.getT());
      return true;
    }
  }
  return false;
}

bool Triangle::intersectAll(const Ray& ray, Hit& in, Hit& out) const {
  Hit hit;
  if (intersect(ray, -numeric_limits<double>::max(), numeric_limits<double>::max(), hit)) {
    in = Hit(this, hit.getT());
    out = Hit(this, hit.getT() + Plane::EPSILON);
    return true;
  }
  return false;
}
  
const BoundingBox& Triangle::getBoundingBox() const {
  return boundingBox;
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
  return plane.textureColor(hitpoint);
}
