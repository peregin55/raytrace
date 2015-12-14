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
#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include "Surface.h"
#include "BoundingBox.h"
class Color;
class Matrix4;
class Point;
class Ray;
class Vector;
using namespace std;

/** Cube.
 * Axis-aligned cube or bounding box.
 */
class Cube : public Surface {
  public:
    /** Construct cube given name of material, minimum XYZ dimensions contained in min vector,
     * and maximum XYZ dimensions in max vector. (min and max should have 3 components).
     */
    Cube(shared_ptr<Material> material, shared_ptr<Texture> texture, vector<double> minXYZ, vector<double> maxXYZ) :
      Surface(material, texture), boundingBox(minXYZ, maxXYZ) { }
    /** Surface intersection. */
    virtual bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const;
    virtual bool intersectAll(const Ray& ray, Hit& in, Hit& out) const;
    virtual const BoundingBox& getBoundingBox() const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint) const;
  private:
    BoundingBox boundingBox;
    static const double EPSILON;
};
#endif
