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
#ifndef SHADING_H
#define SHADING_H

#include "Color.h"
#include "Vector.h"
using namespace std;

class Material;
class Shading {
  public:
    Shading() { }
    Shading(const Vector& normal, const Color& textureColor, const Material* material) :
      normal(normal), textureColor(textureColor), material(material) { }
    Vector getNormal() const { return normal; }
    Color getTextureColor() const { return textureColor; }
    const Material* getMaterial() const { return material; }
  private:
    Vector normal;
    Color textureColor;
    const Material* material;
};

#endif
