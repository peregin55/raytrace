#include "Matrix4.h"
#include "Point.h"
#include "RenderException.h"
#include "catch/catch.hpp"
#include <cmath>
#include <iostream>
using namespace std;

TEST_CASE("Matrix4") {
SECTION("constructor test1") {
  Matrix4 m(1,2,3,4,
           2,4,6,8,
           3,6,9,12,
           4,8,12,16);
  double expected;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      expected = (i+1)*(j+1);
      CHECK(expected == m(i,j));
    }
  }
}

SECTION("constructor test2") {
  double elems[4][4] = {{1,2,3,4},
                        {2,4,6,8},
                        {3,6,9,12},
                        {4,8,12,16}};
  Matrix4 m(elems);
  double expected;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      expected = (i+1)*(j+1);
      CHECK(expected == m(i,j));
    }
  }
}

SECTION("matrix mult") {
  Matrix4 m(1,2,3,4,
            5,6,7,8,
            9,8,7,6,
            5,4,3,2);
  CHECK(m == m*identity());
  CHECK(m ==identity()*m);
}

SECTION("point_mult") {
  Point p(1,2,3);
  CHECK(p == identity()*p);
}

SECTION("transpose") {
  Matrix4 m(1,2,3,4,
            5,6,7,8,
            9,8,7,6,
            5,4,3,2);
  Matrix4 m_transposed(1,5,9,5,
                       2,6,8,4,
                       3,7,7,3,
                       4,8,6,2);
  CHECK(m_transposed == m.transpose());
  CHECK(identity() == identity().transpose());
}

SECTION("inverse") {
  Matrix4 m(1,2,3,4,
            2,-4,6,9,
            3,-9,-3,0,
            0,3,-6,8);
  Matrix4 i1 = m * m.inverse();
  Matrix4 i2 = m.inverse() * m;
  CHECK(i1.eq(identity(), 1e-8));
  CHECK(i2.eq(identity(), 1e-8));
  Matrix4 expected(0.7448979591836735, -0.2857142857142857, 0.2755102040816326, -0.05102040816326531,
                   0.24081632653061225, -0.11428571428571428, -0.004081632653061225, 0.00816326530612245,
                   0.02244897959183674, 0.05714285714285715, -0.045578231292517, -0.07551020408163266,
                   -0.07346938775510205, 0.08571428571428572, -0.0326530612244898, 0.0653061224489796);
  CHECK(m.inverse().eq(expected, 1e-8));
}

SECTION("singular_inverse") {
  Matrix4 m(1,2,4,4,
            2,-4,0,9,
            3,-9,-3,0,
            0,3,3,8);
  REQUIRE_THROWS_AS(m.inverse(), RenderException);
}


SECTION("equalality") {
  Matrix4 m1(1,2,3,4,
             5,6,7,8,
             9,8,7,6,
             5,4,3,2);
  Matrix4 m2(1,2,3,4,
             5,6,7,8,
             9,8,7,6,
             5,4,3,2);
  CHECK(m1 == m2);
  CHECK(m1.eq(m2, 1e-8));

  CHECK(m1 != m1.transpose());
  CHECK(m1.neq(m1.transpose(), 1e-8));
}

SECTION("identity") {
  Matrix4 i(1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1);
  CHECK(i == identity());
}

SECTION("rotateZ") {
  Matrix4 expected(0,1,0,0,
                  -1,0,0,0,
                   0,0,0,0,
                   0,0,0,1);
  Matrix4 actual = rotateZ(-M_PI/2.0);
  bool isValid;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      isValid = actual(i,j) > expected(i,j) - 1e-10 &&
                actual(i,j) < expected(i,j) + 1e-10;
      CHECK(isValid);
    }
  }
}

SECTION("translate") {
  Matrix4 expected(1,0,0,10,
                   0,1,0,20,
                   0,0,1,30,
                   0,0,0,1);
  CHECK(expected == translate(10,20,30));
}

SECTION("scale") {
  Matrix4 expected(2,0,0,0,
                   0,4,0,0,
                   0,0,6,0,
                   0,0,0,1);
  CHECK(expected == scale(2,4,6));
}
}
