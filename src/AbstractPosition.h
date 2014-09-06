#ifndef ABSTRACTPOSITION_H
#define  ABSTRACTPOSITION_H

#include <string>
using namespace std;

enum Coordinate {X,Y,Z};

class AbstractPosition {
  public:
    virtual ~AbstractPosition();
    double operator[](Coordinate c) const;
    virtual string toString() const;
  protected:
    AbstractPosition(double x, double y, double z);
    double coord[3];
};
#endif
