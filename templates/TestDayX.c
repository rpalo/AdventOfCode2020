#include "unity.h"
#include "Day{X}.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  TEST_ASSERT_EQUAL_INT( ,part1("data/day{X}_test.txt"));
}

void test_part2() {
  TEST_ASSERT_EQUAL_INT( ,part2("data/day{X}_test.txt"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  return UNITY_END();
}
