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

extern bool shouldDebug;
static const double DELTA = 1e-10;
static const Color ZERO_COLOR(0.0, 0.0, 0.0);

Color Scene::render(const Ray& ray) const {
  return render(ray, 0);
}

Color Scene::render(const Ray& ray, unsigned int traceCount) const {
  unique_ptr<Hit> closest = intersect(ray, 0.0, numeric_limits<double>::infinity());
  if (closest) {
    return renderColor(ray, *closest, traceCount);
  }
  else {
    return Color(backgroundColor);
  }
}

unique_ptr<Hit> Scene::intersect(const Ray& ray, double t0, double t1) const {
  unique_ptr<Hit> closest;
  unique_ptr<Hit> h;
   for (shared_ptr<Surface> s : surfaces) {
    h = s->intersect(ray, t0, t1);
    if (h) {
      t1 = h->getT();
      closest = move(h);
    }
  }
  return closest;
}

Color Scene::renderColor(const Ray& ray, const Hit& hit, unsigned int traceCount) const {
  Point hitpoint = ray.getPoint() + hit.getT() * ray.getDirection();
  Vector normal = hit.getSurface().calculateNormal(hitpoint);
  const Vector& incident = ray.getDirection().normalized();
  const Material &material = getMaterial(hit.getSurface().getMaterialName());
  Color color;
  for (Light light : lights) {
    color = color + calculateLocalColor(incident, normal, hitpoint, material, light);
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
    const Material& material, const Light& light) const {
  Vector v = -incident;
  Vector l = (light.getPosition() - hitpoint).normalized();
  Vector h = (v+l).normalized();
  bool isShadow = intersect(Ray(hitpoint, l), DELTA, numeric_limits<double>::infinity()) != nullptr;

  const Color& i = light.getColor();
  const Color& ka = material.getAmbientColor();
  const Color& kd = material.getDiffuseColor();
  const Color& ks = material.getSpecularColor();
  double p = material.getSpecularExponent();
  
  double r = ka.getRed()*i.getRed();
  double g = ka.getGreen()*i.getGreen();
  double b = ka.getBlue()*i.getBlue();
  if (!isShadow) {
    r += kd.getRed()*i.getRed()*fmax(0, normal.dot(l)) +
         ks.getRed()*i.getRed()*pow(fmax(0, normal.dot(h)), p);
    g += kd.getGreen()*i.getGreen()*fmax(0, normal.dot(l)) +
         ks.getGreen()*i.getGreen()*pow(fmax(0, normal.dot(h)), p);
    b += kd.getBlue()*i.getBlue()*fmax(0, normal.dot(l)) +
         ks.getBlue()*i.getBlue()*pow(fmax(0, normal.dot(h)), p);
  }
  return Color(r,g,b);
}

/**
 * see http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
 */
Color Scene::calculateReflectedColor(const Vector& incident, const Vector& normal,
    const Point& hitpoint, const Material& material, unsigned int traceCount) const {
  Ray reflected = calculateReflectedRay(incident, normal, hitpoint);
  return material.getReflectiveFraction() * render(reflected, traceCount+1);
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
    k = Color (exp(-material.getRefractiveAttenuation().getRed() * t),
            exp(-material.getRefractiveAttenuation().getGreen() * t),
            exp(-material.getRefractiveAttenuation().getBlue() * t));
    transmitted = calculateTransmittedRay(incident, -normal, n, 1.0, hitpoint);
    if (transmitted) {
      c = transmitted->getDirection().dot(normal);
    }
    else {
      return k * render(reflected, traceCount+1);
    }
  }
  // Schlick's approximation of Fresnel equations, ignoring polarization of light
  double r0 = ((n-1) * (n-1)) / ((n+1) * (n+1));
  double r = r0 + (1-r0)*(1-c)*(1-c)*(1-c)*(1-c)*(1-c);
  return k*(render(reflected, traceCount+1) * r + render(*transmitted, traceCount+1) * (1-r));
}

Ray Scene::calculateReflectedRay(const Vector& incident, const Vector& normal,
    const Point& hitpoint) const {
  Vector reflected = (incident - 2 * (incident.dot(normal)) * normal).normalized();
  Point start = hitpoint + reflected * DELTA;
  return Ray(start, reflected);
}

unique_ptr<Ray> Scene::calculateTransmittedRay(const Vector& incident, const Vector& normal, double n1,
    double n2, const Point& hitpoint) const {
  double cos = incident.dot(normal);
  double radicand = 1 - ((n1*n1)/(n2*n2) * (1 - (cos*cos)));
  if (radicand >= 0.0) {
    Vector trans = (n1/n2) * (incident - normal*cos) - normal * sqrt(radicand);
    Point start = hitpoint + trans * DELTA;
    return unique_ptr<Ray>(new Ray(start, trans));
  }
  else return unique_ptr<Ray>();
}


const Material& Scene::getMaterial(const string& name) const {
  try {
    return materialMap.at(name);
  } catch (const out_of_range& e) {
    throw RenderException("Unable to find surface material: " + name);
  }
}
