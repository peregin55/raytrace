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
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>
#include <string>
#include "Plane.h"
#include "Point.h"
#include "Surface.h"
#include "Vector.h"
#include "Matrix4.h"
using namespace std;
class BoundingBox;
class Hit;
class Matrix4;
class Ray;
class Texture;

/** Triangle surface. */
class Triangle : public Surface {
  public:
    Triangle(shared_ptr<Material> material, shared_ptr<Texture> texture, const Point& p0, const Point& p1, const Point& p2) :
      Surface(material, texture), p0(p0), p1(p1), p2(p2), plane(material, texture, p0, p1, p2), normal(plane.calculateNormal(p0)) {
      boundingBox = BoundingBox(
        min(p0[X], min(p1[X], p2[X])),
        min(p0[Y], min(p1[Y], p2[Y])),
        min(p0[Z], min(p1[Z], p2[Z])),
        max(p0[X], max(p1[X], p2[X])),
        max(p0[Y], max(p1[Y], p2[Y])),
        max(p0[Z], max(p1[Z], p2[Z])));
    }
    virtual bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const;
    virtual const BoundingBox& getBoundingBox() const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint) const;
  private:
    bool isContained(const Point& q0, const Point& q1, const Point& intersect) const;
    Point p0, p1, p2;
    Plane plane;
    Vector normal;
    BoundingBox boundingBox;
};
#endif
