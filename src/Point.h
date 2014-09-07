#ifndef POINT_H
#define POINT_H

#include <string>
#include "AbstractPosition.h"

class Point : public AbstractPosition {
  public:
    Point(double x, double y, double z) : AbstractPosition(x, y, z) { }
    virtual ~Point();
    virtual string toString() const;
};
ostream& operator<<(ostream& cout, const Point& p);
#endif
