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
#include "Color.h"
#include <cmath>

double Color::getRed() const {
  return red;
}

double Color::getGreen() const {
  return green;
}

double Color:: getBlue() const {
  return blue;
}

Color Color::operator+(const Color& other) const {
  return Color(red + other.getRed(), green + other.getGreen(),
    blue + other.getBlue());
}

Color Color::operator*(double d) const {
  return Color(red*d, green*d, blue*d);
}

Color Color::operator/(double d) const {
  return Color(red/d, green/d, blue/d);
}

Color Color::operator*(const Color& c) const {
  return Color(red*c.getRed(), green*c.getGreen(), blue*c.getBlue());
}

bool Color::operator==(const Color& other) const {
  return red == other.getRed() && green == other.getGreen() &&
    blue == other.getBlue();
}

bool Color::operator!=(const Color& other) const {
  return !(*this == other);
}


bool Color::within(const Color& other, double threshold) const {
  return abs(red - other.getRed()) < threshold &&
    abs(green - other.getGreen()) < threshold &&
    abs(blue - other.getBlue()) < threshold;
}

ostream& operator<<(ostream& os, const Color& color) {
  os << "Color(" << color.getRed() << ", " << color.getGreen() << ", " << color.getBlue() << ")";
  return os;
}

