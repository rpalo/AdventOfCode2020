#include "unity.h"
#include "Day10.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  TEST_ASSERT_EQUAL_INT(35, part1("data/day10_test.txt"));
}

void test_part1_bigger() {
  TEST_ASSERT_EQUAL_INT(220, part1("data/day10_test2.txt"));
}

void test_part2() {
  TEST_ASSERT_EQUAL_INT(8, part2("data/day10_test.txt"));
}

void test_part2_bigger() {
  TEST_ASSERT_EQUAL_INT(19208, part2("data/day10_test2.txt"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part1_bigger);
  RUN_TEST(test_part2);
  RUN_TEST(test_part2_bigger);
  return UNITY_END();
}
