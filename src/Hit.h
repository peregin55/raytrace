#ifndef HIT_H
#define HIT_H

#include "Point.h"
#include "Vector.h"

class Surface;

/** Representation of a ray-surface intersection.
 *  Includes Surface involved in intersection,
 *  distance (t) between camera and hitpoint,
 *  hitpoint on Surface,
 *  and normal vector at Surface hitpoint.
 */
class Hit {
  public:
    Hit(const Surface& s, double t, const Point& hitpoint, const Vector& normal) :
      surface(s), t(t), hitpoint(hitpoint), normal(normal) { }
    const Surface& getSurface() const { return surface; }
    double getT() const { return t; }
    const Point& getHitpoint() const { return hitpoint; }
    const Vector& getNormal() const { return normal; }
  private:
    const Surface& surface;
    double t;
    Point hitpoint;
    Vector normal;
};

#endif
