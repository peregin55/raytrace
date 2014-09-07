#ifndef CAMERA_H
#define CAMERA_H
#include "Point.h"
#include "Vector.h"

class Camera {
  public:
    Camera(const Point& pos,
           const Point& ref,
           const Vector& up,
           double b,
           double t,
           double l,
           double r,
           double n) :
       position(pos), reference(ref), up(up),
       top(t), bottom(b), left(l), right(r), near(n) { }

    const Point& getPosition() const { return position; }
    const Point& getReference() const { return reference; }
    const Vector& getUp() const { return up; }
    double getFrameTop() const { return top; }
    double getFrameBottom() const { return bottom; }
    double getFrameLeft() const { return left; }
    double getFrameRight() const { return right; }
    double getFrameNear() const { return near; }

  private:
    Point position, reference;
    Vector up;
    double top, bottom, left, right, near;
};
#endif
