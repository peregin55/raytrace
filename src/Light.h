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

/** Light.
 * Representation of a simple point light source.
 */
class Light {
  public:
    Light(const Point &position, const Color &color) : position(position), color(color) { }
    const Point& getPosition() const { return position; }
    const Color& getColor() const { return color; }
  private:
    Point position;
    Color color;

  friend ostream& operator<<(ostream& os, const Light& light) {
    os << "Light(" << light.getPosition() << ", " << light.getColor() << ")";
    return os;
  }
};


#endif
