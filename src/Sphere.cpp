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
#include <cmath>
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"
#include "Texture.h"
#include "Vector.h"

bool Sphere::intersect(const Ray& ray, double t0, double t1, Hit& hit) const {
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
      hit = Hit(this, fmin(tNeg, tPos));
      return true;
    }
    else if (tPos > t0 && tPos < t1) {
      hit = Hit(this, tPos);
      return true;
    }
    else if (tNeg > t0 && tNeg < t1) {
      hit = Hit(this, tNeg);
      return true;
    }
  }
  return false;
}

const BoundingBox& Sphere::getBoundingBox() const {
  return boundingBox;
}

Vector Sphere::calculateNormal(const Point& hitpoint) const {
  return (hitpoint - center).normalize();
}

Color Sphere::textureColor(const Point& hitpoint) const {
  if (texture) {
    Vector local = hitpoint - center;
    double theta = atan(local[X]/local[Z]);
    double phi = acos(-local[Y]/radius);
    double u = theta / M_PI + 0.5;  // u = (theta + M_PI/2.0) / M_PI;
    double v = phi / M_PI;
    return texture->colorAt(u,v);
  }
  return Color();
}
