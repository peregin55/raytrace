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
#ifndef HIT_H
#define HIT_H

#include "Surface.h"
#include "Point.h"
#include "Vector.h"
#include "Ray.h"


/** Representation of a ray-surface intersection.
 *  Includes Surface involved in intersection,
 *  distance (t) between camera and hitpoint,
 *  hitpoint on Surface,
 *  and normal vector at Surface hitpoint.
 */
class Hit {
  public:
    Hit(const Surface& s, double t) : surface(s), t(t) { }
    const Surface& getSurface() const { return surface; }
    double getT() const { return t; }
  private:
    const Surface& surface;
    double t;
};

#endif
