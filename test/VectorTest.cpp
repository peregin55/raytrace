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
#include "catch/catch.hpp"
#include "Vector.h"

TEST_CASE("Vector") {
  SECTION("constructors") {
    Vector v(1,2,3);
    CHECK(1 == v[X]);
    CHECK(2 == v[Y]);
    CHECK(3 == v[Z]);
  }

  SECTION("equality_==") {
    Vector v1(1,2,3);
    Vector v2(1,2,3);
    Vector v3(1,2,1);
    CHECK(v1 == v2);
    CHECK(v2 == v1);
    CHECK(v1 != v3);
    CHECK(v3 != v1);
  }

  SECTION("equality_equals") {
    Vector v1(1,2,3);
    Vector v2(1,2,3);
    Vector v3(1,2,1);
    CHECK(v1.equals(v2, 0.0));
    CHECK(v2.equals(v1, 0.0));
    CHECK(!v1.equals(v3, 0.0));
    CHECK(!v3.equals(v1, 0.0));
  }

  SECTION("vector arithmetic") {
    Vector v1(1,2,3);
    Vector v2(4,5,6);
    CHECK(Vector(5,7,9).equals(v1+v2, 0.0));
    CHECK(Vector(-3,-3,-3).equals(v1-v2, 0.0));
    CHECK(Vector(-1,-2,-3).equals(-v1, 0.0));
    CHECK(32 == v1.dot(v2));
    CHECK(Vector(-3,6,-3).equals(v1.cross(v2), 0.0));
  }

  SECTION("scalar arithmetc") {
    Vector v1(1,2,3);
    CHECK(Vector(2,4,6).equals(v1*2, 0.0));
    CHECK(v1.equals(v1/1.0, 0.0));
    CHECK(Vector(1,2,3).equals(Vector(3,6,9)/3.0, 0.0));
  }

  SECTION("vector operations") {
    bool lengthValid = (Vector(1,2,3).length() < sqrt(14) + 1e-6) &&
          (Vector(1,2,3).length() > sqrt(14) - 1e-6);
    CHECK(lengthValid);
    CHECK(Vector(3,4,0).normalize().equals(Vector(3.0/5,4.0/5,0.0), 0.0));
  }

  SECTION("from points") {
    CHECK(Vector(5,4,3).equals(Point(5,4,3) - Point(0,0,0), 0.0));
    CHECK(Vector(4,2,0).equals(Point(5,4,3) - Point(1,2,3), 0.0));
    CHECK(Point(1,2,3) == Point(1,2,3) + Vector(0,0,0));
    CHECK(Point(0,0,0) == Point(1,2,3) + Vector(-1,-2,-3));
  }
}
