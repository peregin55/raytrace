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

#include <memory>
#include <string>
#include <vector>
#include "Surface.h"
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
    Cube(shared_ptr<Material> material, shared_ptr<Texture> texture, const vector<double>& min, const vector<double>& max) :
      Surface(material, texture), cubeMin(min), cubeMax(max) { }
    /** Surface intersection. */
    virtual bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint) const;
    /* returns true if ray intersects this cube, with t0 containing the distance at which
     * ray enters the cube, and t1 the distance at which it exits.
     */
    bool isBounded(const Ray& ray, double& t0, double& t1) const;
    vector<double> getCubeMin();
    vector<double> getCubeMax();
  private:
    vector<double> cubeMin;
    vector<double> cubeMax;
    static const double EPSILON;
};
#endif
