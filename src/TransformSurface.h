#ifndef TRANSFORM_SURFACE_H
#define TRANSFORM_SURFACE_H
#include <memory>
#include "Surface.h"
#include "Matrix4.h"
using namespace std;

class Point;
class Ray;
class Vector;

/** TransformSurface.
 * Decorator Surface to introduce local object-coordinate system for a Surface.
 * Transforms referenced Surface by obj2world transformation-matrix.
 */
class TransformSurface : public Surface {
  public:
    TransformSurface(const Matrix4& obj2world, unique_ptr<Surface> surface) :
      Surface(shared_ptr<Material>()),
      obj2world(obj2world),
      obj2worldInverse(obj2world.inverse()),
      obj2worldInverseTranspose(obj2world.inverse().transpose()),
      surface(std::move(surface)) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    Vector calculateNormal(const Point& hitpoint) const;
    virtual const Material* getMaterial() const;
    virtual const Texture* getTexture() const;
    virtual Color textureColor(const Point& hitpoint) const;
  private:
    Matrix4 obj2world;
    Matrix4 obj2worldInverse;
    Matrix4 obj2worldInverseTranspose;
    unique_ptr<Surface> surface;
};
#endif
