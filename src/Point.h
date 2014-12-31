#ifndef POINT_H
#define POINT_H

#include <string>
#include "Cartesian.h"

/** Point in xyz space. */
class Point : public Cartesian {
  public:
    Point() : Cartesian() { }
    Point(double x, double y, double z) : Cartesian(x, y, z) { }
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    virtual string toString() const;
  friend ostream& operator<<(ostream& cout, const Point& p);
};
#endif
