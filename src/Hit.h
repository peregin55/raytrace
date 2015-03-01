#ifndef HIT_H
#define HIT_H

#include "Surface.h"
#include "Point.h"
#include "Vector.h"
#include "Ray.h"


/** Representation of a ray-surface intersection.
 *  Includes Surface involved in intersection,
 *  distance (t) between camera and hitpoint,
 *  hitpoint on Surface,
 *  and normal vector at Surface hitpoint.
 */
class Hit {
  public:
    Hit(const Surface& s, double t) : surface(s), t(t) { }
    const Surface& getSurface() const { return surface; }
    double getT() const { return t; }
  private:
    const Surface& surface;
    double t;
};

#endif
