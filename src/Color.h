#ifndef COLOR_H
#define  COLOR_H

#include <iostream>
using namespace std;

class Color {
  public:
    Color() : red(0.0), green(0.0), blue(0.0) { }
    Color(double r, double g, double b) :
      red(r), green(g), blue(b) { }
    double getRed() const { return red; }
    double getGreen() const { return green; }
    double getBlue() const {return blue; }
    Color operator+(const Color& other) const {
      return Color(red + other.red, green + other.green, blue + other.blue);
    }
    Color operator*(double d) const {
      return Color(red*d, green*d, blue*d);
    }
    Color operator/(double d) const {
      return Color(red/d, green/d, blue/d);
    }
    Color operator*(Color c) const {
      return Color(red*c.red, green*c.green, blue*c.blue);
    }
    bool operator==(const Color& other) const {
      return red == other.red && green == other.green &&
        blue == other.blue;
    }
    bool operator!=(const Color& other) const {
      return !(*this == other);
    }

  private:
    double red, green, blue;

  friend ostream& operator<<(ostream& os, const Color& color) {
    os << "Color(" << color.getRed() << ", " << color.getGreen() << ", " << color.getBlue() << ")";
    return os;
  }
};

#endif
