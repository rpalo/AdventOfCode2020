#include "unity.h"
#include "Day23.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  TEST_ASSERT_EQUAL_INT(67384529,part1("data/day23_test.txt"));
}

void test_part2() {
  TEST_ASSERT_EQUAL_INT64(149245887792, part2("data/day23_test.txt"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  return UNITY_END();
}
