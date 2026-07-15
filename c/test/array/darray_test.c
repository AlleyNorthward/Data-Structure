#include "array/DArray.h"
#include "acutest.h"
#include <stdlib.h>
#include <string.h>

static int free_counter = 0;

static void mock_free_cb(void *elem, void *args) {
  (void)args;
  char *ptr = *(char **)elem;
  free(ptr);
  free_counter++;
}

void test_create_destroy(void) {
  DArray *arr = newDArray(20, sizeof(int));
  TEST_CHECK(arr != NULL);
  TEST_CHECK(capacity_DA(arr) == 20);
  TEST_CHECK(arr->elem_size == sizeof(int));
  TEST_CHECK(size_DA(arr) == 0);
  destroy_DA(arr, NULL, NULL);

  arr = newDArray(0, sizeof(int));
  TEST_CHECK(capacity_DA(arr) == 4);
  destroy_DA(arr, NULL, NULL);

  arr = newDArray(10, 0);
  TEST_CHECK(arr == NULL);
}

void test_set_get(void) {
  DArray *arr = newDArray(10, sizeof(int));
  TEST_CHECK(arr != NULL);

  for (int i = 0; i < 10; i++) {
    SET_DA(arr, i, i, int);
    TEST_CHECK(GET_DA(arr, i, int) == i);
  }
  TEST_CHECK(size_DA(arr) == 10);

  resize_DA(arr, 16);
  SET_DA(arr, 15, 999, int);
  TEST_CHECK(size_DA(arr) == 16);
  TEST_CHECK(GET_DA(arr, 15, int) == 999);
  for (int i = 10; i < 15; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == 0);
  }

  SET_DA(arr, 5, 555, int);
  TEST_CHECK(size_DA(arr) == 16);
  TEST_CHECK(GET_DA(arr, 5, int) == 555);

  SET_DA(arr, 20, 888, int);
  TEST_CHECK(size_DA(arr) == 16);
  int *p = (int *)at_DA(arr, 20);
  TEST_CHECK(p == NULL);

  destroy_DA(arr, NULL, NULL);
}

void test_resize(void) {
  DArray *arr = newDArray(0, sizeof(int));
  TEST_CHECK(capacity_DA(arr) == 4);

  resize_DA(arr, 10);
  TEST_CHECK(size_DA(arr) == 10);
  TEST_CHECK(capacity_DA(arr) >= 10);

  for (int i = 0; i < 10; i++) {
    SET_DA(arr, i, i, int);
  }

  resize_DA(arr, 15);
  TEST_CHECK(size_DA(arr) == 15);
  TEST_CHECK(capacity_DA(arr) >= 15);
  for (int i = 0; i < 10; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == i);
  }
  for (int i = 10; i < 15; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == 0);
  }

  resize_DA(arr, 8);
  TEST_CHECK(size_DA(arr) == 8);
  for (int i = 0; i < 8; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == i);
  }

  resize_DA(arr, 12);
  TEST_CHECK(size_DA(arr) == 12);
  for (int i = 8; i < 12; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == 0);
  }

  resize_DA(arr, 0);
  TEST_CHECK(size_DA(arr) == 0);
  TEST_CHECK(capacity_DA(arr) >= 4);

  resize_DA(arr, -5);
  TEST_CHECK(size_DA(arr) == 0);

  destroy_DA(arr, NULL, NULL);
}

void test_push_pop(void) {
  DArray *arr = newDArray(2, sizeof(int));
  TEST_CHECK(arr != NULL);

  for (int i = 0; i < 5; i++) {
    push_back_DA(arr, &i);
  }
  TEST_CHECK(size_DA(arr) == 5);
  TEST_CHECK(capacity_DA(arr) >= 5);
  for (int i = 0; i < 5; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == i);
  }

  int val = *(int *)pop_back_DA(arr);
  TEST_CHECK(val == 4);
  val = *(int *)pop_back_DA(arr);
  TEST_CHECK(val == 3);
  TEST_CHECK(size_DA(arr) == 3);

  for (int i = 0; i < 3; i++) {
    void *p = pop_back_DA(arr);
    TEST_CHECK(p != NULL);
  }
  TEST_CHECK(size_DA(arr) == 0);

  void *p = pop_back_DA(arr);
  TEST_CHECK(p == NULL);

  for (int i = 0; i < 100; i++) {
    push_back_DA(arr, &i);
  }
  TEST_CHECK(size_DA(arr) == 100);
  TEST_CHECK(capacity_DA(arr) >= 100);
  for (int i = 0; i < 100; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == i);
  }

  destroy_DA(arr, NULL, NULL);
}

void test_move_right(void) {
  DArray *arr = newDArray(20, sizeof(int));
  for (int i = 0; i < 20; i++) {
    SET_DA(arr, i, i, int);
  }

  resize_DA(arr, 23);
  move_right_DA(arr, 5, 3);
  TEST_CHECK(size_DA(arr) == 26);

  SET_DA(arr, 5, 101, int);
  SET_DA(arr, 6, 102, int);
  SET_DA(arr, 7, 103, int);

  for (int i = 0; i < 26; i++) {
    int expected;
    if (i < 5) {
      expected = i;
    } else if (i == 5) {
      expected = 101;
    } else if (i == 6) {
      expected = 102;
    } else if (i == 7) {
      expected = 103;
    } else if (i < 23) {
      expected = i - 3;
    } else {
      expected = 0;
    }
    TEST_CHECK(GET_DA(arr, i, int) == expected);
  }

  int old_len = size_DA(arr);
  resize_DA(arr, old_len + 2);
  move_right_DA(arr, old_len, 2);
  TEST_CHECK(size_DA(arr) == old_len + 4);
  TEST_CHECK(GET_DA(arr, old_len, int) == 0);
  TEST_CHECK(GET_DA(arr, old_len + 1, int) == 0);
  TEST_CHECK(GET_DA(arr, old_len + 2, int) == 0);
  TEST_CHECK(GET_DA(arr, old_len + 3, int) == 0);

  int current_len = size_DA(arr);
  move_right_DA(arr, -1, 2);
  TEST_CHECK(size_DA(arr) == current_len);
  move_right_DA(arr, current_len, 2);
  TEST_CHECK(size_DA(arr) == current_len);
  move_right_DA(arr, 0, 0);
  TEST_CHECK(size_DA(arr) == current_len);
  int cap = capacity_DA(arr);
  move_right_DA(arr, 0, cap - current_len + 1);
  TEST_CHECK(size_DA(arr) == current_len);

  destroy_DA(arr, NULL, NULL);
}

void test_move_left(void) {
  DArray *arr = newDArray(20, sizeof(int));
  for (int i = 0; i < 20; i++) {
    SET_DA(arr, i, i, int);
  }

  move_left_DA(arr, 5, 2);
  TEST_CHECK(size_DA(arr) == 18);
  for (int i = 0; i < 18; i++) {
    int expected = (i < 5) ? i : i + 2;
    TEST_CHECK(GET_DA(arr, i, int) == expected);
  }

  clear_DA(arr, NULL, NULL);
  for (int i = 0; i < 20; i++) {
    SET_DA(arr, i, i, int);
  }
  move_left_DA(arr, 0, 3);
  TEST_CHECK(size_DA(arr) == 17);
  for (int i = 0; i < 17; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == i + 3);
  }

  clear_DA(arr, NULL, NULL);
  for (int i = 0; i < 20; i++) {
    SET_DA(arr, i, i, int);
  }
  move_left_DA(arr, 17, 3);
  TEST_CHECK(size_DA(arr) == 17);
  for (int i = 0; i < 17; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == i);
  }

  int old_len = size_DA(arr);
  move_left_DA(arr, 0, old_len + 1);
  TEST_CHECK(size_DA(arr) == old_len);
  move_left_DA(arr, -1, 2);
  TEST_CHECK(size_DA(arr) == old_len);
  move_left_DA(arr, old_len, 1);
  TEST_CHECK(size_DA(arr) == old_len);

  destroy_DA(arr, NULL, NULL);
}

void test_reverse(void) {
  DArray *arr = newDArray(20, sizeof(int));
  for (int i = 0; i < 20; i++) {
    SET_DA(arr, i, i, int);
  }

  reverse_DA(arr, 0, 19);
  for (int i = 0; i < 20; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == 19 - i);
  }

  clear_DA(arr, NULL, NULL);
  for (int i = 0; i < 20; i++) {
    SET_DA(arr, i, i, int);
  }
  reverse_DA(arr, 5, 14);
  for (int i = 0; i < 20; i++) {
    int expected;
    if (i < 5 || i > 14)
      expected = i;
    else
      expected = 5 + 14 - i;
    TEST_CHECK(GET_DA(arr, i, int) == expected);
  }

  reverse_DA(arr, 7, 7);
  TEST_CHECK(GET_DA(arr, 7, int) == 12);

  int old_len = size_DA(arr);
  reverse_DA(arr, -1, 5);
  TEST_CHECK(size_DA(arr) == old_len);
  reverse_DA(arr, 19, 20);
  TEST_CHECK(size_DA(arr) == old_len);
  reverse_DA(arr, 10, 5);
  TEST_CHECK(size_DA(arr) == old_len);

  destroy_DA(arr, NULL, NULL);
}

void test_clear_free(void) {
  DArray *arr = newDArray(10, sizeof(char *));
  char *s1 = strdup("hello");
  char *s2 = strdup("world");
  SET_DA(arr, 0, s1, char *);
  SET_DA(arr, 1, s2, char *);

  free_counter = 0;
  clear_DA(arr, mock_free_cb, NULL);
  TEST_CHECK(size_DA(arr) == 0);
  TEST_CHECK(free_counter == 2);

  clear_DA(arr, mock_free_cb, NULL);
  TEST_CHECK(free_counter == 2);

  s1 = strdup("one");
  s2 = strdup("two");
  SET_DA(arr, 0, s1, char *);
  SET_DA(arr, 1, s2, char *);
  free_counter = 0;
  destroy_DA(arr, mock_free_cb, NULL);
  TEST_CHECK(free_counter == 2);

  arr = newDArray(10, sizeof(int));
  for (int i = 0; i < 5; i++) {
    SET_DA(arr, i, i, int);
  }
  clear_DA(arr, NULL, NULL);
  TEST_CHECK(size_DA(arr) == 0);
  for (int i = 0; i < 5; i++) {
    push_back_DA(arr, &i);
  }
  for (int i = 0; i < 5; i++) {
    TEST_CHECK(GET_DA(arr, i, int) == i);
  }

  destroy_DA(arr, NULL, NULL);
}

void test_combined(void) {
  DArray *arr = newDArray(10, sizeof(int));
  for (int i = 0; i < 10; i++) {
    SET_DA(arr, i, i, int);
  }

  resize_DA(arr, 15);
  move_right_DA(arr, 5, 3);
  TEST_CHECK(size_DA(arr) == 18);

  SET_DA(arr, 5, 100, int);
  SET_DA(arr, 6, 200, int);
  SET_DA(arr, 7, 300, int);

  for (int i = 0; i < 18; i++) {
    int expected;
    if (i < 5) {
      expected = i;
    } else if (i == 5) {
      expected = 100;
    } else if (i == 6) {
      expected = 200;
    } else if (i == 7) {
      expected = 300;
    } else if (i < 13) {
      expected = i - 3;
    } else {
      expected = 0;
    }
    TEST_CHECK(GET_DA(arr, i, int) == expected);
  }

  move_left_DA(arr, 10, 2);
  TEST_CHECK(size_DA(arr) == 16);

  destroy_DA(arr, NULL, NULL);
}

void test_errors(void) {
  DArray *arr = newDArray(10, sizeof(int));

  clear_DA(NULL, NULL, NULL);
  destroy_DA(NULL, NULL, NULL);
  resize_DA(NULL, 10);
  move_right_DA(NULL, 0, 1);
  move_left_DA(NULL, 0, 1);
  reverse_DA(NULL, 0, 1);

  SET_DA(arr, -1, 1, int);
  TEST_CHECK(size_DA(arr) == 0);
  SET_DA(arr, 100, 1, int);
  TEST_CHECK(size_DA(arr) == 0);
  TEST_CHECK(at_DA(arr, 100) == NULL);

  push_back_DA(arr, NULL);
  TEST_CHECK(size_DA(arr) == 0);
  TEST_CHECK(pop_back_DA(arr) == NULL);

  destroy_DA(arr, NULL, NULL);
}

TEST_LIST = {{"create_destroy", test_create_destroy},
             {"set_get", test_set_get},
             {"resize", test_resize},
             {"push_pop", test_push_pop},
             {"move_right", test_move_right},
             {"move_left", test_move_left},
             {"reverse", test_reverse},
             {"clear_free", test_clear_free},
             {"combined", test_combined},
             {"errors", test_errors},
             {NULL, NULL}};
