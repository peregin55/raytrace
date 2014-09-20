#ifndef POINT_H
#define POINT_H

#include <string>
#include "Cartesian.h"

class Point : public Cartesian {
  public:
    Point(double x, double y, double z) : Cartesian(x, y, z) { }
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    virtual ~Point();
    virtual string toString() const;
};
ostream& operator<<(ostream& cout, const Point& p);
#endif
