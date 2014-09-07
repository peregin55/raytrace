#ifndef LIGHT_H
#define LIGHT_H

#include "Point.h"
#include "Color.h"

class Light {
  public:
    Light(const Point &position, const Color &color) : position(position), color(color) { }
    const Point& getPosition() const { return position; }
    const Color& getColor() const { return color; }
  private:
    Point position;
    Color color;

  friend ostream& operator<<(ostream& os, const Light& light) {
    os << "Light(" << light.getPosition() << ", " << light.getColor() << ")";
    return os;
  }
};
#endif
