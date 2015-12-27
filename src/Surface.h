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
#ifndef SURFACE_H
#define SURFACE_H
#include <memory>
#include <string>
#include <vector>
#include "Color.h"
#include "Material.h"
#include "Ray.h"
#include "Shading.h"
using namespace std;
class BoundingBox;
class Hit;
class Point;
class Shading;
class Texture;
class Vector;

/** Abstract Surface object supported for rendering. */
class Surface {
  public:
    Surface(shared_ptr<Material> material, shared_ptr<Texture> texture):
      material(material), texture(texture) {}
    /** Intersect ray with this object.
     *  Returns closest Hit with distance > t0 and distance < t1
     */
    virtual bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const = 0;
    virtual bool intersectAll(const Ray& ray, Hit& in, Hit& out) const = 0;
    virtual const BoundingBox& getBoundingBox() const = 0;
    virtual Shading shading(const Point& hitpoint, vector<const Surface*> surfaceStack) const = 0;
    virtual ~Surface() { }
  protected:
    shared_ptr<Material> material;
    shared_ptr<Texture> texture;
};
#endif
