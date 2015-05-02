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
#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>
#include <memory>
#include "Color.h"
using namespace std;

/** Material.
 * Representation of an object material properties,
 * including color, reflectiveness, and transparency.
 *
 * Supports local illumination with ambient, diffuse,
 * and specular terms.  Includes reflectiveFraction to
 * denote the percentage of reflection in RGB for glossy
 * materials.
 * Includes refractiveIndex and refractiveAttenuation
 * for refractive materials, to denote the refractive
 * index of the material and the amount of signal
 * reduction in the RGB components of the material.
 * (Environment is assumed to have refractiveIndex = 1.0)
 */
class Material {
  public:
    Material() { }
    Material(const string& name,
             const Color& ambientColor,
             const Color& diffuseColor,
             const Color& specularColor,
             double specularExponent,
             const Color& reflectiveFraction,
             double refractiveIndex,
             const Color& refractiveAttenuation) :
        name(name),
        ambientColor(ambientColor),
        diffuseColor(diffuseColor),
        specularColor(specularColor),
        specularExponent(specularExponent),
        reflectiveFraction(reflectiveFraction),
        refractiveIndex(refractiveIndex),
        refractiveAttenuation(refractiveAttenuation) {}

    const string& getName() const { return name; }
    const Color& getAmbientColor() const { return ambientColor; }
    const Color& getDiffuseColor() const { return diffuseColor; }
    const Color& getSpecularColor() const { return specularColor; }
    double getSpecularExponent() const { return specularExponent; }
    const Color& getReflectiveFraction() const { return reflectiveFraction; }
    double getRefractiveIndex() const { return refractiveIndex; }
    const Color& getRefractiveAttenuation() const { return refractiveAttenuation; }
  private:
    string name;
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    double specularExponent;
    Color reflectiveFraction;
    double refractiveIndex;
    Color refractiveAttenuation;
};
#endif
