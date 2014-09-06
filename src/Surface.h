#ifndef SURFACE_H
#define SURFACE_H
#include <string>
using namespace std;
class Ray;
class Hit;
class Point;
class Vector;

class Surface {
  public:
    Surface(string materialName) : materialName(materialName) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const = 0;
    virtual Vector calculateNormal(const Point& surfacePoint) const = 0;
    string getMaterialName() const { return materialName; }
  private:
    string materialName;
};
#endif
