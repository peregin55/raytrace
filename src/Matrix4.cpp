#include <cmath>
#include "Matrix4.h"
#include "Point.h"
#include "RenderException.h"

Matrix4::Matrix4(double xx, double xy, double xz, double xw,
                 double yx, double yy, double yz, double yw,
                 double zx, double zy, double zz, double zw,
                 double wx, double wy, double wz, double ww) {
  elem[0][0] = xx;
  elem[0][1] = xy;
  elem[0][2] = xz;
  elem[0][3] = xw;
  elem[1][0] = yx;
  elem[1][1] = yy;
  elem[1][2] = yz;
  elem[1][3] = yw;
  elem[2][0] = zx;
  elem[2][1] = zy;
  elem[2][2] = zz;
  elem[2][3] = zw;
  elem[3][0] = wx;
  elem[3][1] = wy;
  elem[3][2] = wz;
  elem[3][3] = ww;
}

Matrix4::Matrix4(double m[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      elem[i][j] = m[i][j];
    }
  }
}

double Matrix4::operator()(int row, int col) const {
  return elem[row][col];
}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
  double m[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      double val = 0.0;
      for (int k = 0; k < 4; k++) {
         val += elem[i][k]*other.elem[k][j];
      }
      m[i][j] = val;
    }
  }
  return Matrix4(m);
}

Point Matrix4::operator*(const Point& other) const {
  double o[] = {other[X], other[Y], other[Z], 1.0};
  double m[4];
  for (int i = 0; i < 4; i++) {
    double val = 0.0;
    for (int k = 0; k < 4; k++) {
       val += elem[i][k]*o[k];
    }
    m[i] = val;
  }
  if (m[3] != 1.0) {
    throw RenderException("Expected normalized w = 1");
  }
  return Point(m[0], m[1], m[2]);
}


Matrix4 Matrix4::transpose(void) const {
  double m[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m[j][i] = this->elem[i][j];
    }
  }
  return Matrix4(m);
}

Matrix4 identity(void) {
  return Matrix4(1.0, 0.0, 0.0, 0.0,
                 0.0, 1.0, 0.0, 0.0,
                 0.0, 0.0, 1.0, 0.0,
                 0.0, 0.0, 0.0, 1.0);
}

Matrix4 rotateZ(double radians) {
  return Matrix4(cos(radians), -sin(radians), 0.0, 0.0,
                 sin(radians),  cos(radians), 0.0, 0.0,
                 0.0,           0.0,          0.0, 0.0,
                 0.0,           0.0,          0.0, 1.0);
}

Matrix4 translate(double x, double y, double z) {
  return Matrix4(1.0, 0.0, 0.0, x,
                 0.0, 1.0, 0.0, y,
                 0.0, 0.0, 1.0, z,
                 0.0, 0.0, 0.0, 1.0);
}

Matrix4 scale(double x, double y, double z) {
  return Matrix4(x, 0.0, 0.0, 0.0,
                 0.0, y, 0.0, 0.0,
                 0.0, 0.0, z, 0.0,
                 0.0, 0.0, 0.0, 1.0);
}
