#ifndef CARTESIAN_H
#define CARTESIAN_H 

#include <string>
using namespace std;

enum Coordinate {X,Y,Z};

class Cartesian {
  public:
    virtual ~Cartesian();
    double operator[](Coordinate c) const;
    virtual string toString() const;
  protected:
    Cartesian(double x, double y, double z);
    double coord[3];
};
#endif
