#include "catch/catch.hpp" 
#include "Point.h"

TEST_CASE("Point") {
  SECTION("constructors_test") {
    Point p(1,2,3);
    CHECK(1 == p[X]);
    CHECK(2 == p[Y]);
    CHECK(3 == p[Z]);
  }

  SECTION("equality") {
    Point p1(1,2,3);
    Point p2(1,2,3);
    Point p3(1,2,1);
    CHECK(p1 ==p2);
    CHECK(p2== p1);
    CHECK(p1 != p3);
    CHECK(p3 != p1);
  }
}
