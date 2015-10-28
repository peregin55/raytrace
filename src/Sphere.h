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
#ifndef SPHERE_H
#define  SPHERE_H

#include <iostream>
#include <memory>
#include <string>
#include "Point.h"
#include "Matrix4.h"
#include "Surface.h"
using namespace std;

class Hit;
class Texture;
class Matrix4;
class Ray;
class Vector;

/** Sphere surface defined by center location and radius. */
class Sphere : public Surface {
  public:
    Sphere(shared_ptr<Material> material, shared_ptr<Texture> texture, const Point& c, double r):
      Surface(material, texture), center(c), radius(r) { }
    virtual bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint) const;
  private:
    Point center;
    double radius;
    Matrix4 world2obj;
};
#endif
