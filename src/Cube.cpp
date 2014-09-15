#include "Cube.h"
#include <cmath>
#include "Hit.h"
#include "Ray.h"
#include "RenderException.h"
#include "Vector.h"

const double Cube::EPSILON = 1.0e-4;

unique_ptr<Hit> Cube::intersect(const Ray& ray, double t0, double t1) const {
  double tmin = 0.0, tmax = 0.0;
  if (isBounded(ray, tmin, tmax)) {
    if (tmin > t0 && tmin < t1) {
      return unique_ptr<Hit>(new Hit(*this, tmin));
    }
    else if (tmax > t0 && tmax < t1) {
      return unique_ptr<Hit>(new Hit(*this, tmax));
    }
  }
  return unique_ptr<Hit>();
}

bool Cube::isBounded(const Ray& ray, double& t0, double& t1) const {
  const Vector& dir = ray.getDirection();
  const Point& eye = ray.getPoint();

  vector<double> tMinCoord(3);
  vector<double> tMaxCoord(3);
  double dirReciprocal = 0.0;
  for (int i = X; i <= Z; i++) {
    // use reciprocal, since avoids 0.0 == -0.0 issue
    // if ray positive, min and max bounds match t bounds
    // otherwise, ray is going other way, so swap cube bounds
    Coordinate coord = (Coordinate) i;
    dirReciprocal = 1.0/dir[coord];
    if (dirReciprocal >= 0.0) {
      tMinCoord[coord] = (cubeMin[coord] - eye[coord]) * dirReciprocal;
      tMaxCoord[coord] = (cubeMax[coord] - eye[coord]) * dirReciprocal;
    }
    else {
      tMinCoord[coord] = (cubeMax[coord] - eye[coord]) * dirReciprocal;
      tMaxCoord[coord] = (cubeMin[coord] - eye[coord]) * dirReciprocal;
    }
  }

  // tmin,tmax = [tmin[X], tmax[X]] intersect [tmin[Y], tmax[Y]]
  if (tMinCoord[X] > tMaxCoord[Y] ||
      tMaxCoord[X] < tMinCoord[Y]) {
    return false;
  }
  double tmin = fmax(tMinCoord[X], tMinCoord[Y]);
  double tmax = fmin(tMaxCoord[X], tMaxCoord[Y]);

  // extend to z => tmin, tmax = [tmin[Z], tmax[Z]] intersect [tmin, tmax]
  if (tMinCoord[Z] > tmax || tMaxCoord[Z] < tmin) {
    return false;
  }
  // final min,max t-values where enter and leaving the bounding box
  t0 = fmax(tMinCoord[Z], tmin);
  t1 = fmin(tMaxCoord[Z], tmax);
  return true;
}
  
Vector Cube::calculateNormal(const Point& surfacePoint) const {
  if (fabs(surfacePoint[X] - cubeMin[X]) < EPSILON) {
    return Vector(-1.0, 0.0, 0.0);
  }
  else if (fabs(surfacePoint[X] - cubeMax[X]) < EPSILON) {
    return Vector(1.0, 0.0, 0.0);
  }
  else if (fabs(surfacePoint[Y] - cubeMin[Y]) < EPSILON) {
    return Vector(0.0, -1.0, 0.0);
  }
  else if (fabs(surfacePoint[Y] - cubeMax[Y]) < EPSILON) {
    return Vector(0.0, 1.0, 0.0);
  }
  else if (fabs(surfacePoint[Z] - cubeMin[Z]) < EPSILON) {
    return Vector(0.0, 0.0, -1.0);
  }
  else if (fabs(surfacePoint[Z] - cubeMax[Z]) < EPSILON) {
    return Vector(0.0, 0.0, 1.0);
  }
  else {
    throw RenderException("Unable to calculate normal for " + surfacePoint.toString());
  }
}