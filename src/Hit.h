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

#include <vector>
#include "Surface.h"
#include "Point.h"
#include "Vector.h"
#include "Ray.h"
#include "RenderException.h"
using namespace std;


/** Representation of a ray-surface intersection.
 *  Includes Surface involved in intersection,
 *  distance (t) between camera and hitpoint,
 *  hitpoint on Surface,
 *  and normal vector at Surface hitpoint.
 */
class Hit {
  public:
    Hit() : surfacePath(), t(0.0) { }
    Hit(const Surface* s, double t) : surfacePath{s}, t(t) { }
    Hit(const vector<const Surface*> s, double t) : surfacePath(s), t(t) { }
    Hit pushSurface(const Surface* surface) const;
    Hit popSurface() const;
    const vector<const Surface*> getSurfaceStack() const;
    const Surface* getSurface() const;
    double getT() const;
    bool operator<=(const Hit& other) const;
    bool operator>=(const Hit& other) const;
    bool operator<(const Hit& other) const;
    bool operator>(const Hit& other) const;
    static const Hit& max(const Hit& h1, const Hit& h2);
    static const Hit& min(const Hit& h1, const Hit& h2);
  private:
    vector<const Surface*> surfacePath;
    double t;
};

#endif
