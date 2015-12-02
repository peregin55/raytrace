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

bool BoundingBox::intersect(const Ray& ray, double t0, double t1) const {
  double hitT;
  return intersect(ray, t0, t1, &hitT);
}

bool BoundingBox::intersect(const Ray& ray, double t0, double t1, double* hitT) const {
  double intersects[2];
  if (calculateBothIntersects(ray, intersects)) {
    double tmin = intersects[0];
    double tmax = intersects[1];
    if (tmin > t0 && tmin < t1) {
      *hitT = tmin;
      return true;
    } else if (tmax > t0 && tmax < t1) {
      *hitT = tmax;
      return true;
    }
  }
  return false;
}
      
    
bool BoundingBox::calculateBothIntersects(const Ray& ray, double intersects[]) const {
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
  intersects[0] = fmax(tMinCoord[Z], tmin);
  intersects[1] = fmin(tMaxCoord[Z], tmax);
  return true;
}
