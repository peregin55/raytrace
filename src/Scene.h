#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <string>
#include <memory>
#include <string>
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
    Scene(vector<Light> lights,
          vector<unique_ptr<Surface>> surfaces,
          const Color& backgroundColor,
          unsigned int maxTrace):
          lights(lights),
          surfaces(std::move(surfaces)),
          backgroundColor(backgroundColor),
          maxTrace(maxTrace) { }
    Color calculateColor(const Ray& ray) const;

  private:
    Color calculateColor(const Ray& ray, unsigned int traceCount) const;
    unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    Color colorFromHit(const Ray& r, const Hit& h, unsigned int traceCount) const;
    Color calculateLocalColor(const Vector& incident, const Vector& normal, const Point& hitpoint,
        const Material& material, const Light& light) const;
    Color calculateReflectedColor(const Vector& incident, const Vector& normal,
        const Point& hitpoint, const Material& material, unsigned int traceCount) const;
    Color calculateTransmittedColor(const Vector& incident, const Vector& normal,
        const Point& hitpoint, double t, const Material& material, unsigned int traceCount) const;
    Ray calculateReflectedRay(const Vector& incident, const Vector& normal,
        const Point& hitpoint) const;
    unique_ptr<Ray> calculateTransmittedRay(const Vector& incident, const Vector& normal, double n1,
        double n2, const Point& hitpoint) const;

    vector<Light> lights;
    vector<unique_ptr<Surface>> surfaces;
    Color backgroundColor;
    unsigned int maxTrace;
};

#endif
