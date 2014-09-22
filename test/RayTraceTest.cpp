#define CATCH_CONFIG_RUNNER
#include "catch/catch.hpp"

bool shouldDebug = false;

int main(int argc, char** argv) {
  return Catch::Session().run(argc, argv);
}
