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

class Color;
class Hit;
class Material;
class Ray;

class Scene {
  public:
    Scene(vector<Light> lights,
          vector<shared_ptr<Surface>> surfaces,
          unordered_map<string, Material> materialMap) : 
          lights(lights), surfaces(surfaces), materialMap(materialMap) { }
    Color render(const Ray& ray) const;

  private:
    unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    Color renderColor(const Hit& h, const Ray& r) const;
    Color renderColor(const Hit& h, const Ray& r, const Light& l) const;

    vector<Light> lights;
    vector<shared_ptr<Surface>> surfaces;
    unordered_map<string, Material> materialMap;
};

#endif
