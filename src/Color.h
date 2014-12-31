#ifndef COLOR_H
#define  COLOR_H

#include <iostream>
using namespace std;

/** Color.
 * RGB based color representation.
 */
class Color {
  public:
    Color() : red(0.0), green(0.0), blue(0.0) { }
    Color(double r, double g, double b) :
      red(r), green(g), blue(b) { }
    double getRed() const;
    double getGreen() const;
    double getBlue() const;
    Color operator+(const Color& other) const;
    Color operator*(double d) const;
    Color operator/(double d) const;
    Color operator*(const Color& c) const;
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;
  private:
    double red, green, blue;
  friend ostream& operator<<(ostream& os, const Color& color);
};

#endif
