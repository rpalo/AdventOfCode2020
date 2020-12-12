#include "unity.h"
#include "Day12.h"

void setUp(){};
void tearDown(){};

void test_part1() {
  TEST_ASSERT_EQUAL_INT(25, part1("data/day12_test.txt"));
}

void test_part2() {
  TEST_ASSERT_EQUAL_INT(286, part2("data/day12_test.txt"));
}

void test_north() {
  ShipState current = {10, 10, 10, 10};
  TEST_ASSERT_EQUAL_INT(35, move(current, AC_NORTH, 25).wy);
}

void test_south() {
  ShipState current = {10, 10, 10, 10};
  TEST_ASSERT_EQUAL_INT(0, move(current, AC_SOUTH, 10).wy);
}

void test_west() {
  ShipState current = {10, 10, 10, 10};
  TEST_ASSERT_EQUAL_INT(0, move(current, AC_WEST, 10).wx);
}

void test_east() {
  ShipState current = {10, 10, 10, 10};
  TEST_ASSERT_EQUAL_INT(35, move(current, AC_EAST, 25).wx);
}

void test_left_q1() {
  ShipState current = {10, 10, 10, 10};
  ShipState new = move(current, AC_LEFT, 90);
  TEST_ASSERT_EQUAL_INT(-10, new.wx);
  TEST_ASSERT_EQUAL_INT(10, new.wy);
}

void test_left_q1_180() {
  ShipState current = {10, 10, 10, 10};
  ShipState new = move(current, AC_LEFT, 180);
  TEST_ASSERT_EQUAL_INT(-10, new.wx);
  TEST_ASSERT_EQUAL_INT(-10, new.wy);
}

void test_left_q3() {
  ShipState current = {10, 10, -10, -10};
  ShipState new = move(current, AC_LEFT, 90);
  TEST_ASSERT_EQUAL_INT(10, new.wx);
  TEST_ASSERT_EQUAL_INT(-10, new.wy);
}

void test_left_q3_180() {
  ShipState current = {10, 10, -10, -10};
  ShipState new = move(current, AC_LEFT, 180);
  TEST_ASSERT_EQUAL_INT(10, new.wx);
  TEST_ASSERT_EQUAL_INT(10, new.wy);
}

void test_left_y_pos() {
  ShipState current = {10, 10, 0, 10};
  ShipState new = move(current, AC_LEFT, 90);
  TEST_ASSERT_EQUAL_INT(-10, new.wx);
  TEST_ASSERT_EQUAL_INT(0, new.wy);
}

void test_left_x_neg_180() {
  ShipState current = {10, 10, -10, 0};
  ShipState new = move(current, AC_LEFT, 180);
  TEST_ASSERT_EQUAL_INT(10, new.wx);
  TEST_ASSERT_EQUAL_INT(0, new.wy);
}

void test_right_q1() {
  ShipState current = {10, 10, 10, 10};
  ShipState new = move(current, AC_RIGHT, 90);
  TEST_ASSERT_EQUAL_INT(10, new.wx);
  TEST_ASSERT_EQUAL_INT(-10, new.wy);
}

void test_right_q1_180() {
  ShipState current = {10, 10, 10, 10};
  ShipState new = move(current, AC_RIGHT, 180);
  TEST_ASSERT_EQUAL_INT(-10, new.wx);
  TEST_ASSERT_EQUAL_INT(-10, new.wy);
}

void test_right_q3() {
  ShipState current = {10, 10, -10, -10};
  ShipState new = move(current, AC_RIGHT, 90);
  TEST_ASSERT_EQUAL_INT(-10, new.wx);
  TEST_ASSERT_EQUAL_INT(10, new.wy);
}

void test_right_q3_180() {
  ShipState current = {10, 10, -10, -10};
  ShipState new = move(current, AC_RIGHT, 180);
  TEST_ASSERT_EQUAL_INT(10, new.wx);
  TEST_ASSERT_EQUAL_INT(10, new.wy);
}

void test_right_y_pos() {
  ShipState current = {10, 10, 0, 10};
  ShipState new = move(current, AC_RIGHT, 90);
  TEST_ASSERT_EQUAL_INT(10, new.wx);
  TEST_ASSERT_EQUAL_INT(0, new.wy);
}

void test_right_x_neg_180() {
  ShipState current = {10, 10, -10, 0};
  ShipState new = move(current, AC_RIGHT, 180);
  TEST_ASSERT_EQUAL_INT(10, new.wx);
  TEST_ASSERT_EQUAL_INT(0, new.wy);
}

void test_right_flips_x_y() {
  ShipState current = {10, 10, 5, 10};
  ShipState new = move(current, AC_RIGHT, 90);
  TEST_ASSERT_EQUAL_INT(10, new.wx);
  TEST_ASSERT_EQUAL_INT(-5, new.wy);
}

void test_forward() {
  ShipState current = {10, 10, 10, 5};
  ShipState new = move(current, AC_FORWARD, 5);
  TEST_ASSERT_EQUAL_INT(60, new.x);
  TEST_ASSERT_EQUAL_INT(35, new.y);
  TEST_ASSERT_EQUAL_INT(10, new.wx);
  TEST_ASSERT_EQUAL_INT(5, new.wy);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_part1);
  RUN_TEST(test_part2);
  RUN_TEST(test_east);
  RUN_TEST(test_north);
  RUN_TEST(test_west);
  RUN_TEST(test_south);
  RUN_TEST(test_left_q1);
  RUN_TEST(test_left_q1_180);
  RUN_TEST(test_left_q3);
  RUN_TEST(test_left_q3_180);
  RUN_TEST(test_left_x_neg_180);
  RUN_TEST(test_left_y_pos);
  RUN_TEST(test_right_q1);
  RUN_TEST(test_right_q1_180);
  RUN_TEST(test_right_q3);
  RUN_TEST(test_right_q3_180);
  RUN_TEST(test_right_x_neg_180);
  RUN_TEST(test_right_y_pos);
  RUN_TEST(test_forward);
  RUN_TEST(test_right_flips_x_y);
  return UNITY_END();
}
