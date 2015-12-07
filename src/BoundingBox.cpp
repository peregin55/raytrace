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
#include "BoundingBox.h"
#include <cmath>
#include "Ray.h"
#include "Point.h"
#include "Vector.h"



bool BoundingBox::intersect(const Ray& ray, double t0, double t1) const {
  double ts[2];
  // if the ray-origin is inside the bounding box, then there will always be a hitpoint with
  // an edge of the box.  However, this hitpoint may not be within the specified (t0, t1) range.
  // If multiple Surfaces are inside of the box, the t0 and t1 range will be reduced to an interval
  // that does not include any of the surrounding box edges. So ignore the (t0, t1) if we're inside.
  if (inside(ray.getPoint())) {
    return true;
  } // otherwise look for intersections between t0 and t1.
  else if (calculateTs(ray, ts)) {
    double tmin = ts[0];
    double tmax = ts[1];
    return (tmin > t0 && tmin < t1) || (tmax > t0 && tmax < t1);
  } else {
    return false;
  }
}

    
bool BoundingBox::calculateTs(const Ray& ray, double ts[]) const {
  const Vector& dir = ray.getDirection();
  const Point& eye = ray.getPoint();

  double tMinCoord[3];
  double tMaxCoord[3];
  double dirReciprocal = 0.0;
  for (int i = X; i <= Z; i++) {
    // if ray positive, min and max bounds match t bounds
    // otherwise, ray is going other way, so swap cube bounds
    // use reciprocal dir, since will force -0.0 case into else-block.
    Coordinate coord = (Coordinate) i;
    dirReciprocal = 1.0/dir[coord];
    // calculate distance between eye and box in units of the ray-direction component
    if (dirReciprocal >= 0.0) {
      tMinCoord[coord] = (boxMin[coord] - eye[coord]) * dirReciprocal;
      tMaxCoord[coord] = (boxMax[coord] - eye[coord]) * dirReciprocal;
    }
    else {
      tMinCoord[coord] = (boxMax[coord] - eye[coord]) * dirReciprocal;
      tMaxCoord[coord] = (boxMin[coord] - eye[coord]) * dirReciprocal;
    }
  }

  // tmin,tmax = [tMinCoord[X], tMaxCoord[X]] intersect [tMinCoord[Y], tMaxCoord[Y]]
  if (tMinCoord[X] > tMaxCoord[Y] ||
      tMaxCoord[X] < tMinCoord[Y]) {
    return false;
  }
  double tmin = fmax(tMinCoord[X], tMinCoord[Y]);
  double tmax = fmin(tMaxCoord[X], tMaxCoord[Y]);

  // extend to z: tmin, tmax = [tMinCoord[Z], tMaxCoord[Z]] intersect [tMinCoord, tMaxCoord]
  if (tMinCoord[Z] > tmax || tMaxCoord[Z] < tmin) {
    return false;
  }
  // final min,max t-values where enter and leaving the bounding box
  ts[0] = fmax(tMinCoord[Z], tmin);
  ts[1] = fmin(tMaxCoord[Z], tmax);
  return true;
}

bool BoundingBox::inside(const Point& p) const {
  for (int i = X; i <= Z; i++) {
    Coordinate coord = (Coordinate) i;
    if (p[coord] > boxMax[coord] || p[coord] < boxMin[coord]) {
      return false;
    }
  }
  return true;
}

const vector<double>& BoundingBox::getMin() const {
  return boxMin;
}

const vector<double>& BoundingBox::getMax() const {
  return boxMax;
}

vector<Point> BoundingBox::calculateEndpoints() const {
  vector<Point> points =
  {
    Point(boxMin[X], boxMin[Y], boxMin[Z]),
    Point(boxMin[X], boxMin[Y], boxMax[Z]),
    Point(boxMin[X], boxMax[Y], boxMin[Z]),
    Point(boxMin[X], boxMax[Y], boxMax[Z]),
    Point(boxMax[X], boxMin[Y], boxMin[Z]),
    Point(boxMax[X], boxMin[Y], boxMax[Z]),
    Point(boxMax[X], boxMax[Y], boxMin[Z]),
    Point(boxMax[X], boxMax[Y], boxMax[Z])
  };
  return points;
}

BoundingBox BoundingBox::operator+(const BoundingBox& other) const {
  vector<double> min = other.getMin();
  vector<double> max = other.getMax();
  for (int i = X; i <= Z; i++) {
    Coordinate coord = (Coordinate) i;
    min[coord] = fmin(min[coord], boxMin[coord]);
  }
  for (int i = X; i <= Z; i++) {
    Coordinate coord = (Coordinate) i;
    max[coord] = fmax(max[coord], boxMax[coord]);
  }
  return BoundingBox(min, max);
}

ostream& operator<<(ostream& os, const BoundingBox& b) {
  os << "BoundingBox(";
  os << "x=[" << b.getMin()[X] << ":" << b.getMax()[X] << "], ";
  os << "y=[" << b.getMin()[Y] << ":" << b.getMax()[Y] << "], ";
  os << "z=[" << b.getMin()[Z] << ":" << b.getMax()[Z] << "])";
  return os;
}

