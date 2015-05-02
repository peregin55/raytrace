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
