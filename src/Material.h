#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>
#include "Color.h"
using namespace std;

class Material {
  public:
    Material() { }
    Material(const string& name,
             const Color& ambientColor,
             const Color& diffuseColor,
             const Color& specularColor,
             double specularExponent) :
        name(name),
        ambientColor(ambientColor),
        diffuseColor(diffuseColor),
        specularColor(specularColor),
        specularExponent(specularExponent) {}

    const string& getName() const { return name; }
    const Color& getAmbientColor() const { return ambientColor; }
    const Color& getDiffuseColor() const { return diffuseColor; }
    const Color& getSpecularColor() const { return specularColor; }
    double getSpecularExponent() const { return specularExponent; }
  private:
    string name;
    Color ambientColor, diffuseColor, specularColor;
    double specularExponent;
};
#endif
