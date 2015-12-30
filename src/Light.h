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
#ifndef LIGHT_H
#define LIGHT_H

#include "Point.h"
#include "Color.h"
#include "RandCache.h"
using namespace std;
class Vector;

/** Light.
 * Representation of a simple point light source.
 */
class Light {
  public:
    Light(const Point &position, const Color &color, double scale) :
      position(position), color(color), scale(scale), randCache() { }
    const Point& getPosition() const;
    const Color& getColor() const;
    double getScale() const;
    vector<Point> calculatePositions(const Vector& normal) const;
    int getNextIndex() const;
  private:
    Point position;
    Color color;
    double scale;
    RandCache randCache;
    static const double DELTA;
    static const unsigned int NUM_SHADOW_RAYS;
  friend ostream& operator<<(ostream& os, const Light& light);
};


#endif
