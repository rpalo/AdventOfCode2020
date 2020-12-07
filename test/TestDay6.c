#include "unity.h"
#include "Day6.h"

#include <stdio.h>

void setUp(){};
void tearDown(){};

void test_part1() {
  FILE* fp;
  fp = fopen("data/day6_test.txt", "r");

  TEST_ASSERT_EQUAL_INT(3, tally_anyone_yeses(fp));
  TEST_ASSERT_EQUAL_INT(3, tally_anyone_yeses(fp));
  TEST_ASSERT_EQUAL_INT(3, tally_anyone_yeses(fp));
  TEST_ASSERT_EQUAL_INT(1, tally_anyone_yeses(fp));
  TEST_ASSERT_EQUAL_INT(1, tally_anyone_yeses(fp));
  fclose(fp);
}

void test_part2() {
  FILE* fp;
  fp = fopen("data/day6_test.txt", "r");

  TEST_ASSERT_EQUAL_INT(3, tally_everyone_yeses(fp));
  TEST_ASSERT_EQUAL_INT(0, tally_everyone_yeses(fp));
  TEST_ASSERT_EQUAL_INT(1, tally_everyone_yeses(fp));
  TEST_ASSERT_EQUAL_INT(1, tally_everyone_yeses(fp));
  TEST_ASSERT_EQUAL_INT(1, tally_everyone_yeses(fp));
  fclose(fp);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  return UNITY_END();
}
