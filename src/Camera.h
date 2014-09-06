#ifndef CAMERA_H
#define CAMERA_H
#include "Point.h"
#include "Vector.h"

class Camera {
  public:
    Camera(Point pos, Point ref, Vector up,
      double b, double t, double l, double r,
      double n) :
       position(pos), reference(ref), up(up),
       top(t), bottom(b), left(l), right(r), near(n) { }

    const Point& getPosition() { return position; }
    const Point& getReference() { return reference; }
    const Vector& getUp() { return up; }
    double getFrameTop() { return top; }
    double getFrameBottom() { return bottom; }
    double getFrameLeft() { return left; }
    double getFrameRight() { return right; }
    double getFrameNear() { return near; }

  private:
    Point position, reference;
    Vector up;
    double top, bottom, left, right, near;
};
#endif
