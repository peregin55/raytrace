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
#include <memory>
#include "Surface.h"
#include "Matrix4.h"
using namespace std;

class Point;
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
      surface(std::move(surface)) { }
    virtual bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const;
    Vector calculateNormal(const Point& hitpoint) const;
    virtual const Material* getMaterial() const;
    virtual const Texture* getTexture() const;
    virtual Color textureColor(const Point& hitpoint) const;
  private:
    Matrix4 obj2world;
    Matrix4 obj2worldInverse;
    Matrix4 obj2worldInverseTranspose;
    unique_ptr<Surface> surface;
};
#endif
