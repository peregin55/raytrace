#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>
#include <memory>
#include "Color.h"
#include "Texture.h"
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
        refractiveAttenuation(refractiveAttenuation),
        texture(unique_ptr<Texture>()) {}

    
    Material(const string& name,
             const Color& ambientColor,
             const Color& diffuseColor,
             const Color& specularColor,
             double specularExponent,
             const Color& reflectiveFraction,
             double refractiveIndex,
             const Color& refractiveAttenuation,
             unique_ptr<Texture> texture) :
        name(name),
        ambientColor(ambientColor),
        diffuseColor(diffuseColor),
        specularColor(specularColor),
        specularExponent(specularExponent),
        reflectiveFraction(reflectiveFraction),
        refractiveIndex(refractiveIndex),
        refractiveAttenuation(refractiveAttenuation),
        texture(std::move(texture)) {}

    const string& getName() const { return name; }
    const Color& getAmbientColor() const { return ambientColor; }
    const Color& getDiffuseColor() const { return diffuseColor; }
    const Color& getSpecularColor() const { return specularColor; }
    double getSpecularExponent() const { return specularExponent; }
    const Color& getReflectiveFraction() const { return reflectiveFraction; }
    double getRefractiveIndex() const { return refractiveIndex; }
    const Color& getRefractiveAttenuation() const { return refractiveAttenuation; }
    const Texture* getTexture() const { return texture.get(); }
  private:
    string name;
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    double specularExponent;
    Color reflectiveFraction;
    double refractiveIndex;
    Color refractiveAttenuation;
    unique_ptr<Texture> texture;
};
#endif
