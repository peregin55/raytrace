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
#include "Plane.h"
#include "Hit.h"
#include "Ray.h"
#include "Shading.h"
#include "Texture.h"
#include <cmath>
#include <limits>

const double Plane::EPSILON = 1.0e-2;

bool Plane::intersect(const Ray& ray, double t0, double t1, Hit& hit) const {
  Point q = ray.getPoint();
  Vector v = ray.getDirection();

  double t = (normal.dot(p0 - q)) / (normal.dot(v));
  if (t > t0 && t < t1) {
    hit = Hit(this, t);
    return true;
  }
  return false;
}

bool Plane::intersectAll(const Ray& ray, Hit& in, Hit& out) const {
  Hit hit;
  if (intersect(ray, -numeric_limits<double>::max(), numeric_limits<double>::max(), hit)) {
    in = hit;
    out = Hit(this, hit.getT() + EPSILON);
    return true;
  }
  return false;
}

const BoundingBox& Plane::getBoundingBox() const {
  return boundingBox;
}

BoundingBox Plane::createBoundingBox(const Point& p0, const Vector& normal) const {
  // in y-z plane
  double normX = fabs(normal[X]);
  double normY = fabs(normal[Y]);
  double normZ = fabs(normal[Z]);
  if (normX > 1.0-EPSILON && normX < 1.0+EPSILON &&
      normY > -EPSILON && normY < EPSILON &&
      normZ > -EPSILON && normZ < EPSILON) {
     return BoundingBox(p0[X]-EPSILON, -numeric_limits<double>::max(), -numeric_limits<double>::max(),
                        p0[X]+EPSILON, numeric_limits<double>::max(), numeric_limits<double>::max());
  } // x-z plane
  else if (normX > -EPSILON && normX < EPSILON &&
           normY > 1.0-EPSILON && normY < 1.0+EPSILON &&
           normZ > -EPSILON && normZ < EPSILON) {
    return BoundingBox(-numeric_limits<double>::max(), p0[Y]-EPSILON, -numeric_limits<double>::max(),
                        numeric_limits<double>::max(), p0[Y]+EPSILON, numeric_limits<double>::max());
  } // x-y plane
  else if (normX > -EPSILON && normX < EPSILON &&
           normY > -EPSILON && normY < EPSILON &&
           normZ > 1.0-EPSILON && normZ < 1.0+EPSILON) {
    return BoundingBox(-numeric_limits<double>::max(), -numeric_limits<double>::max(), p0[Z]-EPSILON,
                        numeric_limits<double>::max(), numeric_limits<double>::max(), p0[Z]+EPSILON);
  } // not aligned to any axis
  else {
    return BoundingBox(-numeric_limits<double>::max(), -numeric_limits<double>::max(), -numeric_limits<double>::max(),
                        numeric_limits<double>::max(), numeric_limits<double>::max(), numeric_limits<double>::max());
  }
}
    
    
Shading Plane::shading(const Point& hitpoint, const Hit& hit) const {
  return Shading(normal, textureColor(hitpoint), material.get());
}
  
Color Plane::textureColor(const Point& hitpoint) const {
  if (texture) {
    Point local = world2plane * hitpoint;
    return texture->colorAt(mapInfinitely(local[X]), mapInfinitely(local[Y]));
  }
  return Color();
}

Vector Plane::getNormal() const {
  return normal;
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
