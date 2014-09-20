#include "easyunit/testharness.h"
using namespace easyunit;

int main(int argc, char** argv) {
  const TestResult* result = TestRegistry::runAndPrint();
  return result->getTotalErrors() + result->getTotalFailures();
}
