#include "unity.h"
#include "Day18.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  TEST_ASSERT_EQUAL_INT(26406 ,part1("data/day18_test.txt"));
}

void test_part2() {
  TEST_ASSERT_EQUAL_INT(694122 ,part2("data/day18_test.txt"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  return UNITY_END();
}
