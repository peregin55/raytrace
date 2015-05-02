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
