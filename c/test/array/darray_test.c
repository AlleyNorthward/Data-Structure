#include "DArray.h"
#include "acutest.h"
#include "config.h"

void test_create_destroy(void) {
  DArray *array = newDArray(10, sizeof(int));
  TEST_CHECK(empty_DA(array));
  TEST_CHECK(size_DA(array) == 0);
  TEST_CHECK(capacity_DA(array) == 10);
  destroy_DA(array, NULL);

  array = newDArray(0, sizeof(int));
  TEST_CHECK(capacity_DA(array) == 4);

  destroy_DA(array, NULL);
}

void test_at(void) {
  DArray *array = newDArray(10, sizeof(int));
  for (int i = 0; i < 10; ++i) {
    GET_INT_DA(array, i) = i;
  }
  TEST_CHECK(size_DA(array) == 10);
  TEST_CHECK(empty_DA(array));
  int a = GET_INT_DA(array, 0);
  TEST_CHECK(a == 0);
  a = GET_INT_DA(array, 5);
  TEST_CHECK(a == 5);
  a = GET_INT_DA(array, 9);
  TEST_CHECK(a == 9);
  a = *(int *)at_DA(array, 7);
  TEST_CHECK(a == 7);
  a = *(int *)at_DA(array, 0);
  TEST_CHECK(a == 0);
  a = *(int *)at_DA(array, 9);
  TEST_CHECK(a == 9);
  clear_DA(array, NULL);
  TEST_CHECK(size_DA(array) == 0);
  TEST_CHECK(empty_DA(array));
  TEST_CHECK(capacity_DA(array) == 10);

  destroy_DA(array, NULL);
}

void test_push_back(void) {
  DArray *array = newDArray(10, sizeof(int));
  for (int i = 0; i < 10; ++i) {
    PUSH_BACK_INT_DA(array, i);
  }
  TEST_CHECK(size_DA(array) == 10);
  TEST_CHECK(empty_DA(array) == false);

  int a = GET_INT_DA(array, 2);

  TEST_CHECK(a == 2);
  a = GET_INT_DA(array, 5);
  TEST_CHECK(a == 5);
  a = GET_INT_DA(array, 9);
  TEST_CHECK(a == 9);
  a = GET_INT_DA(array, 0);
  TEST_CHECK(a == 0);
  clear_DA(array, NULL);
  TEST_CHECK(size_DA(array) == 0);

  for (int i = 0; i < 10; ++i) {
    push_back_DA(array, &i);
  }
  TEST_CHECK(size_DA(array) == 10);

  a = GET_INT_DA(array, 3);

  TEST_CHECK(a == 3);
  a = GET_INT_DA(array, 7);
  TEST_CHECK(a == 7);
  a = GET_INT_DA(array, 9);
  TEST_CHECK(a == 9);
  a = GET_INT_DA(array, 0);
  TEST_CHECK(a == 0);
  clear_DA(array, NULL);
  TEST_CHECK(size_DA(array) == 0);
  TEST_CHECK(capacity_DA(array) == 10);

  destroy_DA(array, NULL);
}

void test_expand(void) {
  DArray *array = newDArray(0, sizeof(int));
  for (int i = 0; i < 10; ++i) {
    PUSH_BACK_INT_DA(array, i);
  }
  TEST_CHECK(size_DA(array) == 10);
  TEST_CHECK(capacity_DA(array) == 16);

  for (int i = 0; i < 200; ++i) {
    PUSH_BACK_INT_DA(array, i);
  }
  TEST_CHECK(size_DA(array) == (10 + 200));
  TEST_CHECK(!empty_DA(array));

  destroy_DA(array, NULL);
}

void test_pop_back(void) {
  DArray *array = newDArray(0, sizeof(int));
  TEST_CHECK(capacity_DA(array) == 4);
  for (int i = 0; i < 10; ++i) {
    PUSH_BACK_INT_DA(array, i);
  }

  int a = POP_BACK_INT_DA(array);
  TEST_CHECK(a == 9);
  TEST_CHECK(size_DA(array) == 9);

  a = POP_BACK_INT_DA(array);
  TEST_CHECK(a == 8);
  TEST_CHECK(size_DA(array) == 8);

  clear_DA(array, NULL);
  TEST_CHECK(size_DA(array) == 0);

  PUSH_BACK_INT_DA(array, 8);
  TEST_CHECK(size_DA(array) == 1);
  a = POP_BACK_INT_DA(array);
  TEST_CHECK(a == 8);
  TEST_CHECK(size_DA(array) == 0);

  destroy_DA(array, NULL);
}

void test_insert(void) {
  DArray *array = newDArray(0, sizeof(int));

  for (int i = 0; i < 10; ++i) {
    PUSH_BACK_INT_DA(array, i);
  }

  TEST_CHECK(size_DA(array) == 10);
  INSERT_INT_DA(array, 5, 101);
  TEST_CHECK(GET_INT_DA(array, 5) == 101);
  TEST_CHECK(GET_INT_DA(array, 0) == 0);
  insert_DA(array, 3, &(int){104});
  TEST_CHECK(GET_INT_DA(array, 3) == 104);
  TEST_CHECK(size_DA(array) == 12);
  clear_DA(array, NULL);
  TEST_CHECK(size_DA(array) == 0);

  destroy_DA(array, NULL);
}

void test_erase(void) {
  DArray *array = newDArray(0, sizeof(int));

  for (int i = 0; i < 10; ++i) {
    PUSH_BACK_INT_DA(array, i);
  }
  TEST_CHECK(size_DA(array) == 10);
  erase_DA(array, 5, NULL);
  TEST_CHECK(size_DA(array) == 9);
  TEST_CHECK(GET_INT_DA(array, 5) == 6);
  erase_DA(array, 0, NULL);
  TEST_CHECK(size_DA(array) == 8);
  TEST_CHECK(GET_INT_DA(array, 0) == 1);
  erase_DA(array, size_DA(array) - 1, NULL);
  TEST_CHECK(GET_INT_DA(array, size_DA(array) - 1) == 8);
  TEST_CHECK(size_DA(array) == 7);

  destroy_DA(array, NULL);
}

TEST_LIST = {{"create_destroy", test_create_destroy},
             {"at", test_at},
             {"push_back", test_push_back},
             {"expand", test_expand},
             {"pop_back", test_pop_back},
             {"insert", test_insert},
             {"erase", test_erase},
             {NULL, NULL}};
