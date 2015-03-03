#include "TransformSurface.h"
#include "Ray.h"
#include "Hit.h"
#include "Surface.h"

unique_ptr<Hit> TransformSurface::intersect(const Ray& ray, double t0, double t1) const {
  Point objCamera = obj2worldInverse * ray.getPoint();
  Vector objDirection = (obj2worldInverse * ray.getDirection()).normalize();
  unique_ptr<Hit> hit = surface->intersect(Ray(objCamera, objDirection), t0, t1);
  if (hit) {
    return unique_ptr<Hit>(new Hit(*this, hit->getT()));
  }
  return unique_ptr<Hit>();
}

Vector TransformSurface::calculateNormal(const Point& hitpoint) const {
  Vector normal = surface->calculateNormal(obj2worldInverse * hitpoint);
  return (obj2worldInverseTranspose * normal).normalize();
}

const Material* TransformSurface::getMaterial() const {
  return surface->getMaterial();
}

const Texture* TransformSurface::getTexture() const {
  return surface->getTexture();
}

Color TransformSurface::textureColor(const Point& hitpoint) const {
  return surface->textureColor(obj2worldInverse * hitpoint);
}
