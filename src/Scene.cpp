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
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include "Scene.h"
#include "Color.h"
#include "Hit.h"
#include "Material.h"
#include "Point.h"
#include "RenderException.h"
#include "Ray.h"
#include "Surface.h"
#include "Vector.h"
using namespace std;

static const double DELTA = 1e-10;
static const Color ZERO_COLOR(0.0, 0.0, 0.0);

bool Scene::calculateColor(const Ray& ray, Color& color) const {
  Hit hit;
  bool wasHit = intersect(ray, hit);
  if (wasHit) {
    color = colorFromHit(ray, hit, 0);
  }
  return wasHit;
}

Color Scene::traceColor(const Ray& ray, unsigned int traceCount) const {
  Hit hit;
  if (intersect(ray, hit)) {
    return colorFromHit(ray, hit, traceCount);
  }
  return ZERO_COLOR;
}

bool Scene::intersect(const Ray& ray, Hit& hit) const {
  return intersect(ray, 0.0, numeric_limits<double>::infinity(), hit);
}

bool Scene::intersect(const Ray& ray, double t0, double t1, Hit& hit) const {
  Hit closest;
  Hit h;
  bool isHit = false;
  for (const unique_ptr<Surface>& s : surfaces) {
    if(s->intersect(ray, t0, t1, h)) {
      isHit = true;
      t1 = h.getT();
      closest = h;
    }
  }
  hit = closest;
  return isHit;
}

Color Scene::colorFromHit(const Ray& ray, const Hit& hit, unsigned int traceCount) const {
  const Surface* surface = hit.getSurface();
  double t = hit.getT();
  Point hitpoint = ray.calculatePoint(t);
  Vector normal = surface->calculateNormal(hitpoint);
  const Vector& incident = ray.getDirection().normalize();
  const Material &material = *hit.getSurface()->getMaterial();
  Color textureColor = hit.getSurface()->textureColor(hitpoint);
  Color color;
  for (Light light : lights) {
    color = color + calculateLocalColor(incident, normal, hitpoint, material, textureColor, light);
  }
  if (traceCount < maxTrace) {
    if (material.getReflectiveFraction() != ZERO_COLOR) {
      color = color + calculateReflectedColor(incident, normal, hitpoint, material, traceCount);
    }
    else if (!std::isinf(material.getRefractiveAttenuation().getRed()) &&
             !std::isinf(material.getRefractiveAttenuation().getGreen()) &&
             !std::isinf(material.getRefractiveAttenuation().getBlue())) {
      color = color + calculateTransmittedColor(incident, normal, hitpoint, hit.getT(), material, traceCount);
    }
  }
  return color;
}

Color Scene::calculateLocalColor(const Vector& incident, const Vector& normal, const Point& hitpoint,
    const Material& material, const Color& textureColor, const Light& light) const {
  Vector lightVector = light.getPosition() - hitpoint;
  double lightDistance = lightVector.length();
  Vector v = -incident;
  Vector l = (lightVector).normalize();
  Vector h = (v+l).normalize();
  Hit shadowHit;
  bool isShadow = intersect(Ray(hitpoint, l), DELTA, lightDistance, shadowHit);

  const Color& i = light.getColor();
  const Color& ka = material.getAmbientColor();
  const Color& kd = material.getDiffuseColor() + textureColor;
  const Color& ks = material.getSpecularColor();
  double p = material.getSpecularExponent();
  
  double r = ka.getRed()*i.getRed();
  double g = ka.getGreen()*i.getGreen();
  double b = ka.getBlue()*i.getBlue();
  if (!isShadow) {
    r += kd.getRed()*i.getRed()*fmax(0, normal.dot(l)) +
         (p <= 0.0 ? 0.0 : ks.getRed()*i.getRed()*pow(fmax(0, normal.dot(h)), p));
    g += kd.getGreen()*i.getGreen()*fmax(0, normal.dot(l)) +
         (p <= 0.0 ? 0.0 : ks.getGreen()*i.getGreen()*pow(fmax(0, normal.dot(h)), p));
    b += kd.getBlue()*i.getBlue()*fmax(0, normal.dot(l)) +
         (p <= 0.0 ? 0.0 : ks.getBlue()*i.getBlue()*pow(fmax(0, normal.dot(h)), p));
  }
  return Color(r,g,b);
}

/**
 * see http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
 */
Color Scene::calculateReflectedColor(const Vector& incident, const Vector& normal,
    const Point& hitpoint, const Material& material, unsigned int traceCount) const {
  Ray reflected = calculateReflectedRay(incident, normal, hitpoint);
  return material.getReflectiveFraction() * traceColor(reflected, traceCount+1);
}

Color Scene::calculateTransmittedColor(const Vector& incident, const Vector& normal,
    const Point& hitpoint, double t, const Material& material, unsigned int traceCount) const {
  double n = material.getRefractiveIndex();
  Color k;
  double c;
  unique_ptr<Ray> transmitted;
  Ray reflected = calculateReflectedRay(incident, normal, hitpoint);

  if (incident.dot(normal) < 0) {
    // incident ray heading into the surface 
    // assuming environment has refractive index = 1.0 and material refractive index n >= 1.0
    // (n1 = 1.0, n2 = n-material)
    // (if it were less than 1, than would have to worry about TIR)
    transmitted = calculateTransmittedRay(incident, normal, 1.0, n, hitpoint);
    if (!transmitted) {
      // TIR, which shouldn't happen unless n < 1.0
      throw RenderException("incoming ray experienced total internal reflection.");
    }
    c = -incident.dot(normal);
    k = Color(1.0, 1.0, 1.0);
  }
  else {
    // heading out of the surface, apply Beer's law to calculate attenuated signal,
    // material refractive index n < 1.0
    // Check for TIR, in which case return reflected amount only, as refraction doesn't occur
    k = Color(exp(-material.getRefractiveAttenuation().getRed() * t),
            exp(-material.getRefractiveAttenuation().getGreen() * t),
            exp(-material.getRefractiveAttenuation().getBlue() * t));
    transmitted = calculateTransmittedRay(incident, -normal, n, 1.0, hitpoint);
    if (transmitted) {
      c = transmitted->getDirection().dot(normal);
    }
    else {
      return k * traceColor(reflected, traceCount+1);
    }
  }
  // Schlick's approximation of Fresnel equations, ignoring polarization of light
  double r0 = ((n-1) * (n-1)) / ((n+1) * (n+1));
  double r = r0 + (1-r0)*(1-c)*(1-c)*(1-c)*(1-c)*(1-c);
  return k*(traceColor(reflected, traceCount+1) * r + traceColor(*transmitted, traceCount+1) * (1-r));
}

Ray Scene::calculateReflectedRay(const Vector& incident, const Vector& normal,
    const Point& hitpoint) const {
  Vector reflected = (incident - 2 * (incident.dot(normal)) * normal).normalize();
  Point start = hitpoint + reflected * DELTA;
  return Ray(start, reflected);
}

unique_ptr<Ray> Scene::calculateTransmittedRay(const Vector& incident, const Vector& normal, double n1,
    double n2, const Point& hitpoint) const {
  double cos = incident.dot(normal);
  double radicand = 1 - ((n1*n1)/(n2*n2) * (1 - (cos*cos)));
  if (radicand >= 0.0) {
    Vector trans = ((n1/n2) * (incident - normal*cos) - normal * sqrt(radicand)).normalize();
    Point start = hitpoint + trans * DELTA;
    return unique_ptr<Ray>(new Ray(start, trans));
  }
  else return unique_ptr<Ray>();
}

