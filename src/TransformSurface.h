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
      Surface(surface->getMaterial()),
      obj2world(obj2world),
      obj2worldInverse(obj2world.inverse()),
      obj2worldTranspose(obj2world.transpose()),
      surface(std::move(surface)) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Point calculateHitpoint(const Ray& ray, double t) const;
    virtual Vector calculateNormal(const Ray& ray, double t) const;
  private:
    Ray createLocalRay(const Ray& ray) const;
    Matrix4 obj2world;
    Matrix4 obj2worldInverse;
    Matrix4 obj2worldTranspose;
    unique_ptr<Surface> surface;
};
#endif
