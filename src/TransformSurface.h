#ifndef TRANSFORM_SURFACE_H
#define TRANSFORM_SURFACE_H
#include <memory>
#include "Matrix4.h"
#include "Surface.h"
using namespace std;

class Ray;
class Surface;

/** TransformSurface.
 * Decorator Surface to introduce local object-coordinate system for a Surface.
 * Transforms referenced Surface by obj2world transformation-matrix.
 */
class TransformSurface : public Surface {
  public:
    TransformSurface(const Matrix4& obj2world, unique_ptr<Surface> surface) :
      Surface(surface->getMaterial()),
      obj2world(obj2world),
      obj2worldInverse(obj2world.inverse()),
      obj2worldTranspose(obj2world.transpose()),
      surface(std::move(surface)) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
  private:
    Matrix4 obj2world;
    Matrix4 obj2worldInverse;
    Matrix4 obj2worldTranspose;
    unique_ptr<Surface> surface;
};
#endif
