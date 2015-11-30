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
#ifndef COLOR_H
#define  COLOR_H

#include <iostream>
using namespace std;

/** Color.
 * RGB based color representation, normalized from 0.0 (absence) to 1.0 (presence).
 */
class Color {
  public:
    Color() : red(0.0), green(0.0), blue(0.0) { }
    Color(double r, double g, double b) :
      red(r), green(g), blue(b) { }
    double getRed() const;
    double getGreen() const;
    double getBlue() const;
    Color operator+(const Color& other) const;
    Color operator*(double d) const;
    Color operator/(double d) const;
    Color operator*(const Color& c) const;
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;
    bool within(const Color& other, double threshold) const;
  private:
    double red, green, blue;
  friend ostream& operator<<(ostream& os, const Color& color);
};

#endif
