#include "unity.h"
#include "Day9.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  TEST_ASSERT_EQUAL_INT(127, part1("data/day9_test.txt", 5));
}

void test_part2() {
  TEST_ASSERT_EQUAL_INT(62, part2("data/day9_test.txt", 5));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  return UNITY_END();
}
