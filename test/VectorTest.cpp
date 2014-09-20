#include <cmath>
#include "easyunit/test.h" 
#include "Vector.h"
using namespace easyunit; 

TEST(Vector,constructors_test) {
  Vector v(1,2,3);
  ASSERT_EQUALS(1, v[X]);
  ASSERT_EQUALS(2, v[Y]);
  ASSERT_EQUALS(3, v[Z]);
}

TEST(Vector,equality) {
  Vector v1(1,2,3);
  Vector v2(1,2,3);
  Vector v3(1,2,1);
  ASSERT_EQUALS(v1, v2);
  ASSERT_EQUALS(v2, v1);
  ASSERT_TRUE(v1 != v3);
  ASSERT_TRUE(v3 != v1);
}

TEST(Vector,vector_arithmetic) {
  Vector v1(1,2,3);
  Vector v2(4,5,6);
  ASSERT_EQUALS(Vector(5,7,9), v1+v2);
  ASSERT_EQUALS(Vector(-3,-3,-3), v1-v2);
  ASSERT_EQUALS(Vector(-1,-2,-3), -v1);
  ASSERT_EQUALS(32, v1.dot(v2));
  ASSERT_EQUALS(Vector(-3,6,-3), v1.cross(v2));
}

TEST(Vector,scalar_arithmetc) {
  Vector v1(1,2,3);
  ASSERT_EQUALS(Vector(2,4,6), v1*2);
  ASSERT_EQUALS(v1, v1/1.0);
  ASSERT_EQUALS(Vector(1,2,3), Vector(3,6,9)/3.0);
}

TEST(Vector,vector_operations) {
  EQUALS_DELTA(sqrt(14), Vector(1,2,3).length(), 1e-6);
  ASSERT_EQUALS(Vector(3,4,0).normalized(), Vector(3.0/5,4.0/5,0.0));
}

TEST(Vector,from_points) {
  ASSERT_EQUALS(Vector(5,4,3), Point(5,4,3) - Point(0,0,0));
  ASSERT_EQUALS(Vector(4,2,0), Point(5,4,3) - Point(1,2,3));
  ASSERT_EQUALS(Point(1,2,3), Point(1,2,3) + Vector(0,0,0));
  ASSERT_EQUALS(Point(0,0,0), Point(1,2,3) + Vector(-1,-2,-3));
}
