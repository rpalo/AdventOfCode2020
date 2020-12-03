#include "unity.h"
#include "Day3.h"

void setUp(){
};

void tearDown() {
};

void test_part1() {
  TreeGrid* trees = parse("data/day3_test.txt");
  TEST_ASSERT_EQUAL_INT(7, part1(trees));
  freeTreeGrid(trees);
}

void test_part2() {
  TreeGrid* trees = parse("data/day3_test.txt");
  TEST_ASSERT_EQUAL_INT(336, part2(trees));
  freeTreeGrid(trees);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  return UNITY_END();
}
