#include "unity.h"
#include "Day4.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  int count;
  SimplePassport* passes = parse_simple_passports("data/day4_test.txt", &count);
  TEST_ASSERT_EQUAL_INT(2, part1(passes, count));
  free(passes);
}

void test_part2() {
  int count;
  FancyPassport* passes = parse_fancy_passports("data/day4_part2_test.txt", &count);
  TEST_ASSERT_EQUAL_INT(4, part2(passes, count));
  free(passes);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  return UNITY_END();
}
