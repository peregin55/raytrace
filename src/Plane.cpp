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
#include "Texture.h"
#include <cmath>

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
