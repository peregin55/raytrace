#ifndef MATRIX4_H
#define MATRIX4_H

#include <iostream>
#include "Cartesian.h"
using namespace std;

class Point;
class Vector;

/** Matrix4.
 * 4x4 matrix for transforming homogeneous coordinates.
 * Provides convenience methods for operating on
 * Points and Vectors.
 */
class Matrix4 {
  public:
    Matrix4() { }
    Matrix4(const double m[4][4]);
    Matrix4(double xx, double xy, double xz, double xw,
            double yx, double yy, double yz, double yw,
            double zx, double zy, double zz, double zw,
            double wx, double wy, double wz, double ww);
    double operator()(int row, int col) const;
    Matrix4 operator*(const Matrix4& other) const;
    Point operator*(const Point& other) const;
    Vector operator*(const Vector& other) const;
    Matrix4 transpose() const;
    Matrix4 inverse() const;
    bool operator==(const Matrix4& other) const;
    bool operator!=(const Matrix4& other) const;
    bool eq(const Matrix4& other, const double err) const;
    bool neq(const Matrix4& other, const double err) const;
  private:
    static const double EPSILON;
    double elem[4][4];
    void decomposeLUP(double lu[4][4], int perm[4]) const;
    void solveLUP(double x[4], const double lu[4][4], const int perm[4], const double b[4]) const;
  friend ostream& operator<<(ostream& cout, const Matrix4& m);
};
Matrix4 identity();
Matrix4 rotate(Coordinate axis, double radians);
Matrix4 translate(double x, double y, double z);
Matrix4 scale(double x, double y, double z);

#endif
