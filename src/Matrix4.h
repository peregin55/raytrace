#ifndef MATRIX4_H
#define MATRIX4_H

#include <iostream>
using namespace std;

class Point;

class Matrix4 {
  public:
    Matrix4(double m[4][4]);
    Matrix4(double xx, double xy, double xz, double xw,
            double yx, double yy, double yz, double yw,
            double zx, double zy, double zz, double zw,
            double wx, double wy, double wz, double ww);
    double operator()(int row, int col) const;
    Matrix4 operator*(const Matrix4& other) const;
    Point operator*(const Point& other) const;
    Matrix4 transpose(void) const;
    bool operator==(const Matrix4& other) const;
    bool operator!=(const Matrix4& other) const;
  private:
    double elem[4][4];
};
ostream& operator<<(ostream& cout, const Matrix4& m);
Matrix4 identity(void);
Matrix4 rotateZ(double radians);
Matrix4 translate(double x, double y, double z);
Matrix4 scale(double x, double y, double z);

#endif
