#include <stdio.h>

#include "unity.h"
#include "Day1.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  int numbers[] = {
    1721,
    979,
    366,
    299,
    675,
    1456,
  };
  int result = part1(numbers, 6);
  TEST_ASSERT_EQUAL_INT(514579, result);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  return UNITY_END();
}
