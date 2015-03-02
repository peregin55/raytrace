#ifndef SURFACE_H
#define SURFACE_H
#include <memory>
#include <string>
#include "Material.h"
#include "Ray.h"
using namespace std;
class Hit;
class Point;
class Texture;
class Vector;

/** Abstract Surface object supported for rendering. */
class Surface {
  public:
    Surface(shared_ptr<Material> material) : material(material), texture(shared_ptr<Texture>()) { }
    Surface(shared_ptr<Material> material, shared_ptr<Texture> texture):
      material(material), texture(texture) {}
    /** Intersect ray with this object.
     *  Returns closest Hit with distance > t0 and distance < t1, or nullptr
     *  if none found.
     */
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const = 0;
    virtual Vector calculateNormal(const Point& hitpoint) const = 0;
    virtual const Material* getMaterial() const { return material.get(); }
    virtual const Texture* getTexture() const { return texture.get(); }
    virtual Color textureColor(const Point& hitpoint) const { return Color(); }
    virtual ~Surface() { }
  protected:
    shared_ptr<Material> material;
    shared_ptr<Texture> texture;
};
#endif
