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
#include "Light.h"
#include "Vector.h"
#include <cmath>

const double Light::DELTA = 1e-5;
const unsigned int Light::NUM_SHADOW_RAYS = 16;

/** Light.
 * Representation of a simple point light source.
 */
const Point& Light::getPosition() const {
  return position;
}

const Color& Light::getColor() const {
  return color;
}

double Light::getScale() const {
  return scale;
}

vector<Point> Light::calculatePositions(const Vector& normal) const {
  if (scale < DELTA && scale > -DELTA) {
    return vector<Point>{position};
  }
  Vector u;
  Vector v;
  if (normal.equals(Vector(0,0,1), DELTA) || normal.equals(Vector(0,0,-1), DELTA)) {
    u = Vector(1,0,0);
    v = Vector(0,1,0);
  } else {
    u = Vector(-normal[Y], normal[X], 0.0).normalize();
    v = normal.cross(u).normalize();
  }
  int numPositions = scale*NUM_SHADOW_RAYS;
  double multiplier = scale/100.0;
  Point origin =  position - (scale/2.0)*u - (scale/2.0)*v;
  vector<Point> randomPositions;
  for (int i=0; i < numPositions; i++) {
    double uscalar = (randCache.next() % 100)*multiplier;
    double vscalar = (randCache.next() % 100)*multiplier;
    randomPositions.push_back(origin + (uscalar*u) + (vscalar*v));
  }
  return randomPositions;
}

ostream& operator<<(ostream& os, const Light& light) {
  os << "Light(" << light.getPosition() << ", " << light.getColor() << ")";
  return os;
}
