#include "Cube.h"
#include <cmath>
#include "Hit.h"
#include "Ray.h"
#include "RenderException.h"
#include "Vector.h"
#include "Color.h"
#include "Texture.h"

const double Cube::EPSILON = 1.0e-10;

vector<double> Cube::getCubeMin() {
  return cubeMin;
}

vector<double> Cube::getCubeMax() {
  return cubeMax;
}
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
    // if ray positive, min and max bounds match t bounds
    // otherwise, ray is going other way, so swap cube bounds
    // use reciprocal dir, since will force -0.0 case into else-block.
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
  t0 = fmax(tMinCoord[Z], tmin);
  t1 = fmin(tMaxCoord[Z], tmax);
  return true;
}

Vector Cube::calculateNormal(const Point& hitpoint) const {
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
    throw RenderException("Unable to calculate normal for " + hitpoint.toString());
  }
}

Color Cube::textureColor(const Point& hitpoint) const {
  if (!texture) {
    return Color();
  }
  if (fabs(hitpoint[X] - cubeMin[X]) < EPSILON || fabs(hitpoint[X] - cubeMax[X]) < EPSILON) {
    double u = (hitpoint[Z] - cubeMin[Z]) / (cubeMax[Z] - cubeMin[Z]);
    double v = (cubeMax[Y] - hitpoint[Y]) / (cubeMax[Y] - cubeMin[Y]);
    return texture->colorAt(u, v);
  }
  else if (fabs(hitpoint[Y] - cubeMin[Y]) < EPSILON || fabs(hitpoint[Y] - cubeMax[Y]) < EPSILON) {
    double u = (hitpoint[X] - cubeMin[X]) / (cubeMax[X] - cubeMin[X]);
    double v = (hitpoint[Z] - cubeMin[Z]) / (cubeMax[Z] - cubeMin[Z]);
    return texture->colorAt(u, v);
  }
  else if (fabs(hitpoint[Z] - cubeMin[Z]) < EPSILON || fabs(hitpoint[Z] - cubeMax[Z]) < EPSILON) {
    double u = (hitpoint[X] - cubeMin[X]) / (cubeMax[X] - cubeMin[X]);
    double v = (cubeMax[Y] - hitpoint[Y]) / (cubeMax[Y] - cubeMin[Y]);
    return texture->colorAt(u, v);
  }
  else {
    throw RenderException("Unable to calculate normal for " + hitpoint.toString());
  }
}
