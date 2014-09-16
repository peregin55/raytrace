#include <iostream>
#include <cmath>
#include "Scene.h"
#include "Color.h"
#include "Hit.h"
#include "Material.h"
#include "Point.h"
#include "RenderException.h"
#include "Ray.h"
#include "Surface.h"
#include "Vector.h"

extern bool shouldDebug;
static const Color backgroundColor(0,0,0);

Color Scene::render(const Ray& ray) const {
  unique_ptr<Hit> closest = intersect(ray, 0.0, INFINITY);
  if (closest) {
    return renderColor(*closest, ray);
  }
  else {
    return backgroundColor;
  }
}

unique_ptr<Hit> Scene::intersect(const Ray& ray, double t0, double t1) const {
  unique_ptr<Hit> h;
  unique_ptr<Hit> closest;
   for (shared_ptr<Surface> s : surfaces) {
    h = s->intersect(ray, t0, t1);
    if (h) {
      t1 = h->getT();
      closest = std::move(h);
    }
  }
  return closest;
}

Color Scene::renderColor(const Hit& hit, const Ray& ray) const {
  Color color;
  for (Light light : lights) {
    color = color + renderColor(hit, ray, light);
  }
  return color;
}

Color Scene::renderColor(const Hit& hit, const Ray& ray, const Light& light) const {
  Point hitpoint = ray.getPoint() + hit.getT() * ray.getDirection();
  Vector n = hit.getSurface().calculateNormal(hitpoint);
  Vector v = -ray.getDirection();
  Vector l = (light.getPosition() - hitpoint).normalized();
  Vector h = (v+l).normalized();
  bool isShadow = intersect(Ray(hitpoint, l), 1e-4, INFINITY) != nullptr;

  if (shouldDebug) {
    cerr << "Scene: hitpoint: " << hitpoint << "\n";
    cerr << "Scene: lightpoint: " << light << "\n";
    cerr << "Scene: normal: " << n << "\n";
    cerr << "Scene: light: " << l << "\n";
    cerr << "Scene: specV: " << v << "\n";
    cerr << "Scene: sepcH: " << h << "\n";
  }
  try {
    const Color& i = light.getColor();
    const Material &m = materialMap.at(hit.getSurface().getMaterialName());
    const Color& ka = m.getAmbientColor();
    const Color& kd = m.getDiffuseColor();
    const Color& ks = m.getSpecularColor();
    double p = m.getSpecularExponent();
    
    double r = ka.getRed()*i.getRed();
    double g = ka.getGreen()*i.getGreen();
    double b = ka.getBlue()*i.getBlue();
    if (!isShadow) {
      r += kd.getRed()*i.getRed()*fmax(0, n.dot(l)) +
           ks.getRed()*i.getRed()*pow(fmax(0, n.dot(h)), p);
      g += kd.getGreen()*i.getGreen()*fmax(0, n.dot(l)) +
           ks.getGreen()*i.getGreen()*pow(fmax(0, n.dot(h)), p);
      b += kd.getBlue()*i.getBlue()*fmax(0, n.dot(l)) +
           ks.getBlue()*i.getBlue()*pow(fmax(0, n.dot(h)), p);
    }
    return Color(r,g,b);
  } catch (const out_of_range& e) {
    throw RenderException("Unable to find surface material: " + hit.getSurface().getMaterialName());
  }
}
