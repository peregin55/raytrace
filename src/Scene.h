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
#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <string>
#include <memory>
#include <string>
#include "BoundingBox.h"
#include "Surface.h"
#include "Light.h"
#include "Point.h"
#include "Material.h"

class Color;
class Hit;
class Ray;

/** Scene.
 * Encapsulates lights, surfaces, background color, and max number of traces
 * (reflection/refraction ray-bounces).
 * Provides method to determine a Color given a single Ray into the Scene.
 */
class Scene {
  public:
    Scene(vector<unique_ptr<Light>> lights,
          vector<unique_ptr<Surface>> surfaces,
          unsigned int maxTrace):
          lights(std::move(lights)),
          surfaces(std::move(surfaces)),
          maxTrace(maxTrace) {
      vector<unique_ptr<Surface>>::const_iterator it = this->surfaces.begin();
      boundingBox = (*it)->getBoundingBox();
      for (it++; it != this->surfaces.end(); it++) {
        boundingBox = boundingBox + (*it)->getBoundingBox();
      }
    }
    bool calculateColor(const Ray& ray, Color& color) const;
    // remove these methods since have unique_ptr instance field
    Scene(const Scene& scene) = delete;
    Scene& operator=(const Scene& scene) = delete;

  private:
    Color traceColor(const Ray& ray, unsigned int traceCount) const;
    bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const;
    bool intersect(const Ray& ray, Hit& hit) const;
    Color colorFromHit(const Ray& r, const Hit& h, unsigned int traceCount) const;
    Color calculateLocalColor(const Vector& incident, const Vector& normal, const Point& hitpoint,
        const Material& material, const Color& textureColor, const Light& light) const;
    Color calculateReflectedColor(const Vector& incident, const Vector& normal,
        const Point& hitpoint, const Material& material, unsigned int traceCount) const;
    Color calculateTransmittedColor(const Vector& incident, const Vector& normal,
        const Point& hitpoint, double t, const Material& material, unsigned int traceCount) const;
    Ray calculateReflectedRay(const Vector& incident, const Vector& normal,
        const Point& hitpoint) const;
    bool calculateTransmittedRay(const Vector& incident, const Vector& normal, double n1,
        double n2, const Point& hitpoint, Ray& transmittedRay) const;

    vector<unique_ptr<Light>> lights;
    vector<unique_ptr<Surface>> surfaces;
    BoundingBox boundingBox;
    unsigned int maxTrace;
    static const double DELTA;
    static const Color ZERO_COLOR;
};

#endif
