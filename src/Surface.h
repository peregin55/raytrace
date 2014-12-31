#ifndef SURFACE_H
#define SURFACE_H
#include <memory>
#include <string>
using namespace std;
class Ray;
class Hit;
class Point;
class Vector;

/** Abstract Surface object supported for rendering. */
class Surface {
  public:
    Surface(const string& materialName) : materialName(materialName) { }
    /** Intersect ray with this object.
     *  Returns closest Hit with distance > t0 and distance < t1, or nullptr
     *  if none found.
     */
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const = 0;
    const string& getMaterialName() const { return materialName; }
    virtual ~Surface() { }
  private:
    string materialName;
};
#endif
