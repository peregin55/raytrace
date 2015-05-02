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
#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include <memory>
#include <vector>
using namespace std;

class Color;

/** Texture.
 */
class Texture {
  public:
    Texture(const vector<unsigned char>& image,
            unsigned int width,
            unsigned int height) :
        image(image), width(width), height(height) {}
    static Texture* fromFile(const string& filename);
    Color fileColorAt(unsigned int x, unsigned int y) const;
    Color colorAt(double u, double v) const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
  private:
    static const unsigned char BITDEPTH;
    static const double NORM;
    static const double EPSILON;
    double bound(double x) const;
    vector<unsigned char> image;
    unsigned int width;
    unsigned int height;
};
#endif
