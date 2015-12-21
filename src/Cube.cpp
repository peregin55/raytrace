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
#include "Cube.h"
#include <cmath>
#include "Hit.h"
#include "Ray.h"
#include "RenderException.h"
#include "Vector.h"
#include "Color.h"
#include "Texture.h"
#include <string>

const double Cube::EPSILON = 1.0e-10;

bool Cube::intersect(const Ray& ray, double t0, double t1, Hit& hit) const {
  double ts[2];
  if (boundingBox.calculateTs(ray, ts)) {
    double tmin = ts[0];
    double tmax = ts[1];
    if (tmin > t0 && tmin < t1) {
      hit = Hit(this, tmin);
      return true;
    } else if (tmax > t0 && tmax < t1) {
      hit = Hit(this, tmax);
      return true;
    }
  }
  return false;
}

bool Cube::intersectAll(const Ray& ray, Hit& in, Hit& out) const {
  double ts[2];
  if (boundingBox.calculateTs(ray, ts)) {
    double tmin = ts[0];
    double tmax = ts[1];
    in = Hit(this, tmin);
    out = Hit(this, tmax);
    return true;
  }
  return false;
}

const BoundingBox& Cube::getBoundingBox() const {
  return boundingBox;
}

Vector Cube::calculateNormal(const Point& hitpoint, const Hit& hit) const {
  const vector<double>& cubeMin = boundingBox.getMin();
  const vector<double>& cubeMax = boundingBox.getMax();
  if (fabs(hitpoint[X] - cubeMin[X]) < EPSILON) {
    return Vector(-1.0, 0.0, 0.0);
  }
  else if (fabs(hitpoint[X] - cubeMax[X]) < EPSILON) {
    return Vector(1.0, 0.0, 0.0);
  }
  else if (fabs(hitpoint[Y] - cubeMin[Y]) < EPSILON) {
    return Vector(0.0, -1.0, 0.0);
  }
  else if (fabs(hitpoint[Y] - cubeMax[Y]) < EPSILON) {
    return Vector(0.0, 1.0, 0.0);
  }
  else if (fabs(hitpoint[Z] - cubeMin[Z]) < EPSILON) {
    return Vector(0.0, 0.0, -1.0);
  }
  else if (fabs(hitpoint[Z] - cubeMax[Z]) < EPSILON) {
    return Vector(0.0, 0.0, 1.0);
  }
  else {
    throw RenderException("Unable to calculate normal for " + hitpoint.toString() + ": " + to_string(cubeMin[Y]) + " " + to_string(cubeMax[Y]));
  }
}

Color Cube::textureColor(const Point& hitpoint, const Hit& hit) const {
  const vector<double>& cubeMin = boundingBox.getMin();
  const vector<double>& cubeMax = boundingBox.getMax();
  if (!texture) {
    return Color();
  }
  if (fabs(hitpoint[X] - cubeMin[X]) < EPSILON || fabs(hitpoint[X] - cubeMax[X]) < EPSILON) {
    double u = (hitpoint[Z] - cubeMin[Z]) / (cubeMax[Z] - cubeMin[Z]);
    double v = (hitpoint[Y] - cubeMin[Y]) / (cubeMax[Y] - cubeMin[Y]);
    return texture->colorAt(u, v);
  }
  else if (fabs(hitpoint[Y] - cubeMin[Y]) < EPSILON || fabs(hitpoint[Y] - cubeMax[Y]) < EPSILON) {
    double u = (hitpoint[X] - cubeMin[X]) / (cubeMax[X] - cubeMin[X]);
    double v = (cubeMax[Z] - hitpoint[Z]) / (cubeMax[Z] - cubeMin[Z]);
    return texture->colorAt(u, v);
  }
  else if (fabs(hitpoint[Z] - cubeMin[Z]) < EPSILON || fabs(hitpoint[Z] - cubeMax[Z]) < EPSILON) {
    double u = (hitpoint[X] - cubeMin[X]) / (cubeMax[X] - cubeMin[X]);
    double v = (hitpoint[Y] - cubeMin[Y]) / (cubeMax[Y] - cubeMin[Y]);
    return texture->colorAt(u, v);
  }
  else {
    throw RenderException("Unable to calculate texture for " + hitpoint.toString());
  }
}
