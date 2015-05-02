/* Copyright 2015 Stephen H. Johnson

  This file is part of raytrace.

  raytrace is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  raytrace is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with raytrace.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CAMERA_H
#define CAMERA_H
#include <string>
#include "Point.h"
#include "Vector.h"

/** Camera.
 * Includes camera position, reference point where camera is pointed,
 * up-vector to orient the camera, and  bottom, top, left, right and
 * near dimensions of the camera frame.
 */
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
