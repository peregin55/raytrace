#include "Matrix4.h"
#include "Point.h"
#include "catch/catch.hpp"
#include <cmath>

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
      REQUIRE(expected == m(i,j));
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
      REQUIRE(expected == m(i,j));
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
  CHECK(m1 != m1.transpose());
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
      REQUIRE(isValid);
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
