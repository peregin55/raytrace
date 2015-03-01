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
    Texture t = Texture::fromFile("textures/test.png");
    CHECK(t.colorAt(0,0) == WHITE);
    CHECK(t.colorAt(1,0) == RED);
    CHECK(t.colorAt(2,0) == GREEN);
    CHECK(t.colorAt(3,0) == BLUE);
    CHECK(t.colorAt(4,0) == WHITE);
    CHECK(t.colorAt(5,0) == WHITE);
    CHECK(t.colorAt(6,0) == WHITE);
    CHECK(t.colorAt(7,0) == WHITE);
    CHECK(t.colorAt(8,0) == RED);
    CHECK(t.colorAt(9,0) == GREEN);

    CHECK(t.colorAt(0,1) == WHITE);
    CHECK(t.colorAt(1,1) == WHITE);
    CHECK(t.colorAt(2,1) == BLUE);
    CHECK(t.colorAt(3,1) == GREEN);
    CHECK(t.colorAt(4,1) == RED);
    CHECK(t.colorAt(5,1) == WHITE);
    CHECK(t.colorAt(6,1) == WHITE);
    CHECK(t.colorAt(7,1) == WHITE);
    CHECK(t.colorAt(8,1) == WHITE);
    CHECK(t.colorAt(9,1) == BLUE);

    CHECK(t.colorAt(0,9) == RED);
    CHECK(t.colorAt(9,9) == BLUE);
  }

  SECTION("texture unbounded") {
    Texture t = Texture::fromFile("textures/test.png");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 10; j++) {
        CHECK(t.colorAt(i*10+j,0) == t.colorAt(j,0));
        CHECK(t.colorAt(0,i*10+j) == t.colorAt(0,j));
      }
    }
    for (int i = 0; i < 3; i++) {
      for (int j = 1; j < 10; j++) {
        CHECK(t.colorAt(-i*10-j,0) == t.colorAt(10-j,0));
        CHECK(t.colorAt(0,-i*10-j) == t.colorAt(0,10-j));
      }
    }
    CHECK(t.colorAt(100, -20) == t.colorAt(0,0));
    CHECK(t.colorAt(-1,-1) == t.colorAt(9,9));
    CHECK(t.colorAt(-21, 19) == t.colorAt(9,9));
    CHECK(t.colorAt(0, 19) == t.colorAt(0,9));
  }
}
