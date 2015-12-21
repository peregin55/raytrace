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
#ifndef PLANE_H
#define  PLANE_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Matrix4.h"
#include "Point.h"
#include "Vector.h"
#include "Surface.h"
#include "BoundingBox.h"
using namespace std;
class BoundingBox;
class Material;
class Texture;
class Ray;

/** Infinite plane surface. */
class Plane : public Surface {
  public:
    Plane(shared_ptr<Material> material,
          shared_ptr<Texture> texture,
          const Point& p0,
          const Vector& normal,
          const Matrix4& world2plane) :
      Surface(material, texture),
      p0(p0),
      normal(normal),
      world2plane(world2plane),
      boundingBox(createBoundingBox(p0, normal)){ }
    Plane(shared_ptr<Material> material,
          shared_ptr<Texture> texture,
          const Point& p0,
          const Point& p1,
          const Point& p2) :
      Surface(material, texture), p0(p0) {
        Vector v0 = p1 - p0;
        Vector u0 = v0.normalize();
        Vector v1 = p2 - p0;
        Vector proj = (v1.dot(u0)) * u0;
        Vector v0Perp = v1 - proj;
        Vector v2 = v0.cross(v0Perp);
        normal = v2.normalize();
        world2plane = Matrix4(v0[X], v0Perp[X], v2[X], p0[X],
                              v0[Y], v0Perp[Y], v2[Y], p0[Y],
                              v0[Z], v0Perp[Z], v2[Z], p0[Z],
                              0.0,   0.0,   0.0,   1.0).inverse();
        boundingBox = createBoundingBox(p0, normal);
    }

    virtual bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const;
    virtual bool intersectAll(const Ray& ray, Hit& in, Hit& out) const;
    virtual const BoundingBox& getBoundingBox() const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Vector calculateNormal(const Point& hitpoint, const Hit& hit) const;
    virtual Color textureColor(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint, const Hit& hit) const;
    const static double EPSILON;
  private:
    double mapInfinitely(double x) const;
    BoundingBox createBoundingBox(const Point& p0, const Vector& normal) const;
    Point p0;
    Vector normal;
    Matrix4 world2plane;
    BoundingBox boundingBox;
};
#endif
