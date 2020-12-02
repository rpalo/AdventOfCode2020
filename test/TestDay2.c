#include "unity.h"
#include "Day2.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  PoliciedPassword p1 = {.a = 1, .b = 3, .letter = 'a', .password = "abcde"};
  PoliciedPassword p2 = {.a = 1, .b = 3, .letter = 'b', .password = "cdefg"};
  PoliciedPassword p3 = {.a = 2, .b = 9, .letter = 'c', .password = "ccccccccc"};
  PoliciedPassword* passwords[] = {&p1, &p2, &p3};
  int result = part1(passwords, 3);
  TEST_ASSERT_EQUAL_INT(2, result);
}

void test_part2() {
  PoliciedPassword p1 = {.a = 1, .b = 3, .letter = 'a', .password = "abcde"};
  PoliciedPassword p2 = {.a = 1, .b = 3, .letter = 'b', .password = "cdefg"};
  PoliciedPassword p3 = {.a = 2, .b = 9, .letter = 'c', .password = "ccccccccc"};
  PoliciedPassword* passwords[] = {&p1, &p2, &p3};
  int result = part2(passwords, 3);
  TEST_ASSERT_EQUAL_INT(1, result);
}

void test_part2_1_based_indexing() {
  PoliciedPassword p1 = {.a = 2, .b = 4, .letter = 'a', .password = "abade"};
  PoliciedPassword p2 = {.a = 2, .b = 3, .letter = 'a', .password = "abade"};
  PoliciedPassword* passwords[] = {&p1, &p2};
  int result = part2(passwords, 2);
  TEST_ASSERT_EQUAL_INT(1, result);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  RUN_TEST(test_part2_1_based_indexing);
  return UNITY_END();
}
