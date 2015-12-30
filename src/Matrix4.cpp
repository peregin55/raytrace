/* Copyright 2015 Stephen H. Johnson

  This file is part of raytrace.

  raytrace is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  raytrace is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with raytrace.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <cmath>
#include <cstring>
#include <iomanip>
#include "Matrix4.h"
#include "Point.h"
#include "Vector.h"
#include "RenderException.h"

const double Matrix4::EPSILON = 1.0e-10;

Matrix4::Matrix4(const double xx, const double xy, const double xz, const double xw,
                 const double yx, const double yy, const double yz, const double yw,
                 const double zx, const double zy, const double zz, const double zw,
                 const double wx, const double wy, const double wz, const double ww) {
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

Matrix4::Matrix4(const double m[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      elem[i][j] = m[i][j];
    }
  }
}

double Matrix4::operator()(const int row, const int col) const {
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
  // homogenize w element 
  if (m[3] != 1.0) {
    for (int i = 0; i < 3; i++) {
      m[i] /= m[3];
    }
  }
  return Point(m[0], m[1], m[2]);
}

Vector Matrix4::operator*(const Vector& other) const {
  double o[] = {other[X], other[Y], other[Z], 0.0};
  double m[4];
  for (int i = 0; i < 4; i++) {
    double val = 0.0;
    for (int k = 0; k < 4; k++) {
       val += elem[i][k]*o[k];
    }
    m[i] = val;
  }
  // dropping w element - in most cases will be 0,
  // although it may not be such as when using
  // inverse-transpose matrix TransformSurface 
  return Vector(m[0], m[1], m[2]);
}

Matrix4 Matrix4::transpose() const {
  double m[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m[j][i] = this->elem[i][j];
    }
  }
  return Matrix4(m);
}

// based on LUP algorithm from Cormen et al "Introduction to Algorithms"
Matrix4 Matrix4::inverse() const {
  double lu[4][4];
  int perm[4];
  decomposeLUP(lu, perm);
  double result[4][4];
  for (int i = 0; i < 4; i++) {
    double x[4];
    double b[4] = {0};
    b[i] = 1.0;
    solveLUP(x, lu, perm, b);
    for (int j=0; j < 4; j++) {
      result[j][i] = x[j];
    }
  }
  return Matrix4(result);
}

/* Determines the PA = LU decomposition of this matrix (A).
 * Writes into lu, where L is lu[i][j] for i > j
 * and U is lu[i][j] for i <= j
 * Writes into perm, representing the row-index permutations
 * from the decomposistion (analogous to the P permutation matrix)
 */
void Matrix4::decomposeLUP(double lu[4][4], int perm[4]) const {
  // copy this matrix into lu
  memcpy(lu, elem, sizeof(double)*16);
  // initial identity permutation
  for (int i = 0; i < 4; i++) {
    perm[i] = i;
  }
  // move down/in the row/columns of the matrix A to calc PA = LU
  for (int k = 0; k < 4; k++) {
    // find largest pivot and its index
    double pivot = 0.0;
    int k_prime = -1;
    for (int i = k; i < 4; i++) {
      double next  = fabs(lu[i][k]);
      if (next > pivot) {
        pivot = next;
        k_prime = i;
      }
    }
    if (pivot < EPSILON || k_prime < 0) {
      throw RenderException("Unable to calculate inverse of singular matrix");
    }
    // adjust perm bookkeeping and matrix for new pivot-row
    swap(perm[k], perm[k_prime]);
    for (int i = 0; i < 4; i++) {
      swap(lu[k][i], lu[k_prime][i]);
    }
    // with pivot-row in place, do LU decomposition
    for (int i = k+1; i < 4; i++) {
      // U[k][i] = lu[k][i], so just need to calculate L[i][k]
      lu[i][k] = lu[i][k] / lu[k][k];
    }
    // and calculate Schur complement of submatrix
    for (int i = k+1; i < 4; i++) {
      for (int j = k+1; j < 4; j++) {
        lu[i][j] = lu[i][j] - lu[i][k] * lu[k][j];
      }
    }
  }
}

// Generic method to solve Ax = b for x, given LUP dcomposition of A and b
void Matrix4::solveLUP(double x[4], const double lu[4][4], const int perm[4], const double b[4]) const {
  double y[4];
  // forward-substitute over L to solve y
  for (int i = 0; i < 4; i++) {
    double res = 0.0;
    for (int j = 0; j < i; j++) {
      res += lu[i][j] * y[j];
    }
    y[i] = b[perm[i]] - res;
  }
  // back-subsitute over U to solve x
  for (int i = 3; i >=0; i--) {
    double res = 0.0;
    for (int j = i+1; j < 4; j++) {
      res += lu[i][j] * x[j];
    }
    x[i] = (y[i] - res) / lu[i][i];
  }
}

bool Matrix4::operator==(const Matrix4& other) const {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (elem[i][j] != other.elem[i][j]) {
        return false;
      }
    }
  }
  return true;
}

bool Matrix4::operator!=(const Matrix4& other) const {
  return !(*this == other);
}

bool Matrix4::equals(const Matrix4& other, const double err) const {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (!(elem[i][j] < other.elem[i][j] + err &&
            elem[i][j] > other.elem[i][j] - err)) {
        return false;
      }
    }
  }
  return true;
}

Matrix4 identity(void) {
  return Matrix4(1.0, 0.0, 0.0, 0.0,
                 0.0, 1.0, 0.0, 0.0,
                 0.0, 0.0, 1.0, 0.0,
                 0.0, 0.0, 0.0, 1.0);
}

Matrix4 rotate(Coordinate axis, double radians) {
  if (axis == X) {
    return Matrix4(1.0,   0.0,           0.0,           0.0,
                   0.0,   cos(radians), -sin(radians),  0.0,
                   0.0,   sin(radians),  cos(radians),  0.0,
                   0.0,   0.0,           0.0,           1.0);
  } else if (axis == Y) {
    return Matrix4(cos(radians),  0.0,   sin(radians),  0.0,
                   0.0,           1.0,   0.0,           0.0,
                  -sin(radians),  0.0,   cos(radians),  0.0,
                   0.0,           0.0,   0.0,           1.0);
  } else {
    return Matrix4(cos(radians), -sin(radians), 0.0, 0.0,
                   sin(radians),  cos(radians), 0.0, 0.0,
                   0.0,           0.0,          1.0, 0.0,
                   0.0,           0.0,          0.0, 1.0);
  }
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

ostream& operator<<(ostream& cout, const Matrix4& m) {
  cout << "[\n";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cout << setw(4) << m(i,j) << " ";
    }
    cout << "\n";
  }
  cout << "]\n";
  return cout;
}
