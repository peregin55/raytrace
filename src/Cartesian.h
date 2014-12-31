#ifndef CARTESIAN_H
#define CARTESIAN_H 

#include <string>
using namespace std;

enum Coordinate {X,Y,Z};

/** Cartesian.
 * Represents abstract concept existing in coordinate frame.
 */
class Cartesian {
  public:
    double operator[](Coordinate c) const;
    virtual string toString() const;
    virtual ~Cartesian();
  protected:
    Cartesian();
    Cartesian(double x, double y, double z);
    double coord[3];
};
#endif
