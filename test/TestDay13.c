#include "unity.h"
#include "Day13.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  TEST_ASSERT_EQUAL_INT(295, part1("data/day13_test.txt"));
}

void test_part2() {
  TEST_ASSERT_EQUAL_INT(1068781, part2("data/day13_test.txt"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  return UNITY_END();
}
