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
#ifndef TRANSFORM_SURFACE_H
#define TRANSFORM_SURFACE_H
#include <iostream>
#include <vector>
#include <memory>
#include "Surface.h"
#include "Matrix4.h"
#include "BoundingBox.h"
using namespace std;

class Point;
class Hit;
class Ray;
class Vector;

/** TransformSurface.
 * Decorator Surface to introduce local object-coordinate system for a Surface.
 * Transforms referenced Surface by obj2world transformation-matrix.
 */
class TransformSurface : public Surface {
  public:
    TransformSurface(const Matrix4& obj2world, unique_ptr<Surface> surface) :
      Surface(shared_ptr<Material>(), shared_ptr<Texture>()),
      obj2world(obj2world),
      obj2worldInverse(obj2world.inverse()),
      obj2worldInverseTranspose(obj2world.inverse().transpose()),
      surface(std::move(surface)) {
      const BoundingBox& objBox = this->surface->getBoundingBox();
      const vector<Point>& objPoints = objBox.calculateEndpoints();
      vector<Point>::const_iterator it = objPoints.begin();
      double minX = (obj2world * (*it))[X];
      double minY = (obj2world * (*it))[Y];
      double minZ = (obj2world * (*it))[Z];
      double maxX = minX;
      double maxY = minY;
      double maxZ = minZ;
      for (it++; it != objPoints.end(); it++) {
        minX = min(minX, (obj2world * (*it))[X]);
        minY = min(minY, (obj2world * (*it))[Y]);
        minZ = min(minZ, (obj2world * (*it))[Z]);
        maxX = max(maxX, (obj2world * (*it))[X]);
        maxY = max(maxY, (obj2world * (*it))[Y]);
        maxZ = max(maxZ, (obj2world * (*it))[Z]);
      }
      boundingBox = BoundingBox(minX, minY, minZ, maxX, maxY, maxZ);
    }
    virtual bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const;
    virtual bool intersectAll(const Ray& ray, Hit& in, Hit& out) const;
    virtual const BoundingBox& getBoundingBox() const;
    virtual Shading shading(const Point& hitpoint, const Hit& hit) const;
    // remove these methods since have unique_ptr instance field
    TransformSurface(const TransformSurface& surface) = delete;
    TransformSurface& operator=(const TransformSurface& surface) = delete;
  private:
    Matrix4 obj2world;
    Matrix4 obj2worldInverse;
    Matrix4 obj2worldInverseTranspose;
    unique_ptr<Surface> surface;
    BoundingBox boundingBox;
};
#endif
