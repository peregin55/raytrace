#include "TransformSurface.h"
#include "Ray.h"
#include "Hit.h"
#include "Surface.h"
#include <iostream>

unique_ptr<Hit> TransformSurface::intersect(const Ray& ray, double t0, double t1) const {
  Point objCamera = obj2worldInverse * ray.getPoint();
  Vector objDirection = (obj2worldInverse * ray.getDirection()).normalize();
  unique_ptr<Hit> hit = surface->intersect(Ray(objCamera, objDirection), t0, t1);
  if (hit) {
    Point worldHitpoint = obj2world * hit->getHitpoint();
    Vector worldNormal = (obj2worldTranspose * hit->getNormal()).normalize();
    return unique_ptr<Hit>(new Hit(hit->getSurface(), hit->getT(), worldHitpoint, worldNormal));
  }
  else {
    return hit;
  }
}
