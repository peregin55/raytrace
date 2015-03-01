#ifndef SURFACE_H
#define SURFACE_H
#include <memory>
#include <string>
#include "Material.h"
#include "Ray.h"
using namespace std;
class Hit;
class Point;
class Vector;

/** Abstract Surface object supported for rendering. */
class Surface {
  public:
    Surface(shared_ptr<Material> material) : material(material) { }
    /** Intersect ray with this object.
     *  Returns closest Hit with distance > t0 and distance < t1, or nullptr
     *  if none found.
     */
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const = 0;
    virtual Vector calculateNormal(const Ray& ray, double t) const = 0;
    Point calculateHitpoint(const Ray& ray, double t) const { return ray.getPoint() + ray.getDirection() * t; }
    shared_ptr<Material> getMaterial() const { return material; }
    virtual ~Surface() { }
  private:
    shared_ptr<Material> material;
};
#endif
