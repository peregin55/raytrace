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
