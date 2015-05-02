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
#include "RenderException.h"
#include "Color.h"
#include "Texture.h"

TEST_CASE("Texture") {
  const Color WHITE(1,1,1);
  const Color RED(1,0,0);
  const Color GREEN(0,1,0);
  const Color BLUE(0,0,1);

  SECTION("texture within bounds") {
    shared_ptr<Texture> t(Texture::fromFile("textures/test.png"));
    CHECK(t->fileColorAt(0,0) == WHITE);
    CHECK(t->fileColorAt(1,0) == RED);
    CHECK(t->fileColorAt(2,0) == GREEN);
    CHECK(t->fileColorAt(3,0) == BLUE);
    CHECK(t->fileColorAt(4,0) == WHITE);
    CHECK(t->fileColorAt(5,0) == WHITE);
    CHECK(t->fileColorAt(6,0) == WHITE);
    CHECK(t->fileColorAt(7,0) == WHITE);
    CHECK(t->fileColorAt(8,0) == RED);
    CHECK(t->fileColorAt(9,0) == GREEN);

    CHECK(t->fileColorAt(0,1) == WHITE);
    CHECK(t->fileColorAt(1,1) == WHITE);
    CHECK(t->fileColorAt(2,1) == BLUE);
    CHECK(t->fileColorAt(3,1) == GREEN);
    CHECK(t->fileColorAt(4,1) == RED);
    CHECK(t->fileColorAt(5,1) == WHITE);
    CHECK(t->fileColorAt(6,1) == WHITE);
    CHECK(t->fileColorAt(7,1) == WHITE);
    CHECK(t->fileColorAt(8,1) == WHITE);
    CHECK(t->fileColorAt(9,1) == BLUE);

    CHECK(t->fileColorAt(0,9) == RED);
    CHECK(t->fileColorAt(9,9) == BLUE);
  }

  SECTION("texture unbounded") {
    shared_ptr<Texture> t(Texture::fromFile("textures/test.png"));
    REQUIRE_THROWS_AS(t->fileColorAt(100, -20), RenderException);
    REQUIRE_THROWS_AS(t->fileColorAt(-1,-1), RenderException);
    REQUIRE_THROWS_AS(t->fileColorAt(-21, 19), RenderException);
    REQUIRE_THROWS_AS(t->fileColorAt(0, 19), RenderException);
  }

  SECTION("texture uv within bounds") {
    shared_ptr<Texture> t(Texture::fromFile("textures/test.png"));
    CHECK(t->colorAt(0.0, 0.0) == RED);
    CHECK(t->colorAt(1.0, 0.0) == BLUE);
    CHECK(t->colorAt(0.0, 1.0) == WHITE);
    CHECK(t->colorAt(1.0, 1.0) == GREEN);
  }
  SECTION("texture uv unbounded") {
    shared_ptr<Texture> t(Texture::fromFile("textures/test.png"));
    REQUIRE_THROWS_AS(t->colorAt(-1.0, 0.0), RenderException);
    REQUIRE_THROWS_AS(t->colorAt(0.0, -1.0), RenderException);
    REQUIRE_THROWS_AS(t->colorAt(-1.0, -1.0), RenderException);
  }
}
