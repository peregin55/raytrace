#include "TransformSurface.h"
#include "Ray.h"
#include "Hit.h"
#include "Surface.h"

unique_ptr<Hit> TransformSurface::intersect(const Ray& ray, double t0, double t1) const {
  return surface->intersect(createLocalRay(ray), t0, t1);
}

Point TransformSurface::calculateHitpoint(const Ray& ray, double t) const {
  Point hitpoint = surface->calculateHitpoint(createLocalRay(ray), t);  
  return obj2world * hitpoint;
}

Vector TransformSurface::calculateNormal(const Ray& ray, double t) const {
  Vector normal = surface->calculateNormal(createLocalRay(ray), t);
  return (obj2worldTranspose * normal).normalize();
}

Ray TransformSurface::createLocalRay(const Ray& ray) const {
  Point objCamera = obj2worldInverse * ray.getPoint();
  Vector objDirection = (obj2worldInverse * ray.getDirection()).normalize();
  return Ray(objCamera, objDirection);
}
