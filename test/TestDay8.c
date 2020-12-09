#include "unity.h"
#include "Day8.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  TEST_ASSERT_EQUAL_INT(5, part1("data/day8_test.txt"));
}

void test_part2() {
  TEST_ASSERT_EQUAL_INT(8, part2("data/day8_test.txt"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  return UNITY_END();
}
