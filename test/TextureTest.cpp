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
    unique_ptr<Texture> t(Texture::fromFile("textures/test.png"));
    CHECK(t->pixelColor(0,0) == WHITE);
    CHECK(t->pixelColor(1,0) == RED);
    CHECK(t->pixelColor(2,0) == GREEN);
    CHECK(t->pixelColor(3,0) == BLUE);
    CHECK(t->pixelColor(4,0) == WHITE);
    CHECK(t->pixelColor(5,0) == WHITE);
    CHECK(t->pixelColor(6,0) == WHITE);
    CHECK(t->pixelColor(7,0) == WHITE);
    CHECK(t->pixelColor(8,0) == RED);
    CHECK(t->pixelColor(9,0) == GREEN);

    CHECK(t->pixelColor(0,1) == WHITE);
    CHECK(t->pixelColor(1,1) == WHITE);
    CHECK(t->pixelColor(2,1) == BLUE);
    CHECK(t->pixelColor(3,1) == GREEN);
    CHECK(t->pixelColor(4,1) == RED);
    CHECK(t->pixelColor(5,1) == WHITE);
    CHECK(t->pixelColor(6,1) == WHITE);
    CHECK(t->pixelColor(7,1) == WHITE);
    CHECK(t->pixelColor(8,1) == WHITE);
    CHECK(t->pixelColor(9,1) == BLUE);

    CHECK(t->pixelColor(0,9) == RED);
    CHECK(t->pixelColor(9,9) == BLUE);
  }

  SECTION("texture unbounded") {
    unique_ptr<Texture> t(Texture::fromFile("textures/test.png"));
    REQUIRE_THROWS_AS(t->pixelColor(100, -20), RenderException);
    REQUIRE_THROWS_AS(t->pixelColor(-1,-1), RenderException);
    REQUIRE_THROWS_AS(t->pixelColor(-21, 19), RenderException);
    REQUIRE_THROWS_AS(t->pixelColor(0, 19), RenderException);
  }
}
