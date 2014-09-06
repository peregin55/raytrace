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
  private:
    double red;
    double green;
    double blue;

  friend ostream& operator<<(ostream& os, const Color& color) {
    os << "Color(" << color.getRed() << ", " << color.getGreen() << ", " << color.getBlue() << ")";
    return os;
  }
};

#endif
