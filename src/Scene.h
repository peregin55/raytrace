#ifndef SCENE_H
#define SCENE_H
#include <memory.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "Surface.h"
#include "Light.h"
#include "Point.h"
#include "Material.h"

class Color;
class Hit;
class Ray;

class Scene {
  public:
    Scene(vector<Light> lights,
          vector<shared_ptr<Surface>> surfaces,
          unordered_map<string, Material> materialMap,
          const Color& backgroundColor,
          unsigned int maxTrace):
          lights(lights), surfaces(surfaces), materialMap(materialMap),
          backgroundColor(backgroundColor), maxTrace(maxTrace) { }
    Color render(const Ray& ray) const;

  private:
    Color render(const Ray& ray, unsigned int traceCount) const;
    unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    Color renderColor(const Ray& r, const Hit& h, unsigned int traceCount) const;
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
    const Material& getMaterial(const string& name) const;

    vector<Light> lights;
    vector<shared_ptr<Surface>> surfaces;
    unordered_map<string, Material> materialMap;
    Color backgroundColor;
    unsigned int maxTrace;
};

#endif
