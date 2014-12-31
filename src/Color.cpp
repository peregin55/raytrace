#include "Color.h"

double Color::getRed() const {
  return red;
}

double Color::getGreen() const {
  return green;
}

double Color:: getBlue() const {
  return blue;
}

Color Color::operator+(const Color& other) const {
  return Color(red + other.getRed(), green + other.getGreen(),
    blue + other.getBlue());
}

Color Color::operator*(double d) const {
  return Color(red*d, green*d, blue*d);
}

Color Color::operator/(double d) const {
  return Color(red/d, green/d, blue/d);
}

Color Color::operator*(const Color& c) const {
  return Color(red*c.getRed(), green*c.getGreen(), blue*c.getBlue());
}

bool Color::operator==(const Color& other) const {
  return red == other.getRed() && green == other.getGreen() &&
    blue == other.getBlue();
}

bool Color::operator!=(const Color& other) const {
  return !(*this == other);
}

ostream& operator<<(ostream& os, const Color& color) {
  os << "Color(" << color.getRed() << ", " << color.getGreen() << ", " << color.getBlue() << ")";
  return os;
}

