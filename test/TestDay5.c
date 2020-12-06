#include "unity.h"
#include "Day5.h"

void setUp(){};
void tearDown(){};

void test_44_5() {
  TEST_ASSERT_EQUAL_INT(357, seat_ID("FBFBBFFRLR"));
}

void test_70_7() {
  TEST_ASSERT_EQUAL_INT(567, seat_ID("BFFFBBFRRR"));
}

void test_14_7() {
  TEST_ASSERT_EQUAL_INT(119, seat_ID("FFFBBBFRRR"));
}

void test_102_4() {
  TEST_ASSERT_EQUAL_INT(820, seat_ID("BBFFBBFRLL"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_44_5);
  RUN_TEST(test_70_7);
  RUN_TEST(test_14_7);
  RUN_TEST(test_102_4);
  return UNITY_END();
}
