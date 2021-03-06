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
#include "TransformSurface.h"
#include "Ray.h"
#include "Hit.h"
#include "Surface.h"

bool TransformSurface::intersect(const Ray& ray, double t0, double t1, Hit& hit) const {
  Point objCamera = obj2worldInverse * ray.getPoint();
  // don't normalize the direction vector, so t-length remains in units of the world-space.
  // if M is world2obj matrix:
  // world_hitpoint = camera + t * dir, so local_hitpoint =  M*(world_hitpoint) = M*(camera) + t * M*(dir)
  // easy to recover local_hitpoint from world_hitpoint, local_hitpoint = M*(world_hitpoint)
  Vector objDirection = obj2worldInverse * ray.getDirection();
  Hit objHit;
  if(surface->intersect(Ray(objCamera, objDirection), t0, t1, objHit)) {
    // make Hit that references this Surface and not just underlying one 
    hit = objHit.pushSurface(this);
    return true;
  }
  return false;
}

bool TransformSurface::intersectAll(const Ray& ray, Hit& in, Hit& out) const {
  Point objCamera = obj2worldInverse * ray.getPoint();
  Vector objDirection = obj2worldInverse * ray.getDirection();
  Hit objIn;
  Hit objOut;
  if (surface->intersectAll(Ray(objCamera, objDirection), objIn, objOut)) {
    in = objIn.pushSurface(this);
    out = objOut.pushSurface(this);
    return true;
  }
  return false;
}

const BoundingBox& TransformSurface::getBoundingBox() const {
  return boundingBox;
}

Vector TransformSurface::calculateNormal(const Point& hitpoint, const Hit& hit) const {
  Vector normal = surface->calculateNormal(obj2worldInverse * hitpoint, hit.popSurface());
  return (obj2worldInverseTranspose * normal).normalize();
}

Color TransformSurface::textureColor(const Point& hitpoint, const Hit& hit) const {
  return surface->textureColor(obj2worldInverse * hitpoint, hit.popSurface());
}

const Material* TransformSurface::getMaterial(const Point& hitpoint, const Hit& hit) const {
  return surface->getMaterial(hitpoint, hit.popSurface());
}

const Texture* TransformSurface::getTexture(const Point& hitpoint, const Hit& hit) const {
  return surface->getTexture(hitpoint, hit.popSurface());
}
