#include "array/DArray.h"
#include <dscommon.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void *at(DArray *arr, int pos) {
  return (char *)arr->data + pos * arr->elem_size;
}

static void clear(DArray *arr, void (*free_cb)(void *elem, void *args),
                  void *args) {
  if (arr == NULL || arr->len == 0)
    return;

  if (free_cb) {
    for (int i = 0; i < arr->len; ++i) {
      void *elem = (char *)arr->data + i * arr->elem_size;
      free_cb(elem, args);
    }
  }

  arr->len = 0;
}

static bool expand(DArray *arr) {
  if (arr == NULL)
    return false;
  int new_capacity = arr->capacity == 0 ? 4 : arr->capacity * 2;
  void *new_data = realloc(arr->data, arr->elem_size * new_capacity);
  if (new_data == NULL)
    return false;

  arr->capacity = new_capacity;
  arr->data = new_data;

  return true;
}

static void set(DArray *arr, int pos, void *elem) {
  char *dest = (char *)arr->data + pos * arr->elem_size;
  memcpy(dest, elem, arr->elem_size);
}

DArray *newDArray(int capacity, int elem_size) {
  if (elem_size == 0)
    return NULL;
  if (capacity == 0)
    capacity = 4;

  DArray *array = malloc(sizeof(DArray));
  if (array == NULL)
    return NULL;

  array->data = calloc(capacity, elem_size);
  if (!array->data) {
    free(array);
    return NULL;
  }

  array->elem_size = elem_size;
  array->capacity = capacity;
  array->len = 0;

  return array;
}

bool empty_DA(DArray *arr) {
  if (arr == NULL)
    return true;

  return arr->len == 0;
}

void clear_DA(DArray *arr, void (*free_cb)(void *elem, void *args),
              void *args) {
  if (arr == NULL || arr->len == 0)
    return;
  clear(arr, free_cb, args);
}

void destroy_DA(DArray *arr, void (*free_cb)(void *elem, void *args),
                void *args) {
  if (arr == NULL)
    return;

  clear(arr, free_cb, args);
  free(arr->data);
  free(arr);
}

int size_DA(DArray *arr) {
  if (arr == NULL)
    return 0;
  return arr->len;
}

int capacity_DA(DArray *arr) {
  if (arr == NULL)
    return 0;
  return arr->capacity;
}

void print_DA(DArray *arr, void (*print_cb)(void *elem)) {
  if (arr == NULL || print_cb == NULL) {
    putchar('\n');
    return;
  }

  int size = size_DA(arr);
  for (int i = 0; i < size; ++i) {
    void *elem = at(arr, i);
    print_cb(elem);
  }
  putchar('\n');
}

void push_back_DA(DArray *arr, void *elem) {
  if (arr == NULL || elem == NULL)
    return;

  if (arr->len == arr->capacity)
    if (!expand(arr))
      return;

  void *dest = at(arr, arr->len);
  memcpy(dest, elem, arr->elem_size);
  ++arr->len;
}

void *pop_back_DA(DArray *arr) {
  if (arr == NULL || arr->len == 0)
    return NULL;
  --arr->len;
  return at(arr, arr->len);
}

void move_right_DA(DArray *arr, int pos, int n) {
  if (arr == NULL || arr->data == NULL || pos < 0 || pos >= arr->len ||
      n <= 0 || n > arr->capacity - arr->len)
    return;

  char *base = (char *)arr->data;
  memmove(base + (pos + n) * arr->elem_size, base + pos * arr->elem_size,
          (arr->len - pos) * arr->elem_size);
  arr->len += n;
}

void move_left_DA(DArray *arr, int pos, int n) {
  if (arr == NULL || arr->data == NULL || pos < 0 || pos >= arr->len ||
      n <= 0 || n > arr->len - pos)
    return;

  char *base = (char *)arr->data;
  memmove(base + arr->elem_size * pos, base + arr->elem_size * (pos + n),
          (arr->len - pos - n) * arr->elem_size);
  arr->len -= n;
}

void reverse_DA(DArray *arr, int left_pos, int right_pos) {
  if (arr == NULL || arr->len == 0 || arr->data == NULL || left_pos < 0 ||
      left_pos >= arr->len || right_pos < 0 || right_pos >= arr->len ||
      left_pos >= right_pos)
    return;

  char *base = (char *)arr->data;
  int size = arr->elem_size;

  while (left_pos < right_pos) {
    swap_bytes(base + left_pos * size, base + right_pos * size, size);
    ++left_pos;
    --right_pos;
  }
}

void set_DA(DArray *arr, int pos, void *elem) {
  if (arr == NULL || elem == NULL || pos < 0 || pos >= arr->capacity) {
    return;
  }
  if (pos < arr->len) {
    set(arr, pos, elem);
    return;
  }

  void *dest = (char *)arr->data + arr->len * arr->elem_size;
  int size =
      (pos - arr->len) * arr->elem_size; // 确实不用+1, 虽然是[arr->len, pos],
                                         // 但是pos就接着被设置了, 没必要再+1
  memset(dest, 0, size);
  set(arr, pos, elem);
  arr->len = pos + 1;
}

void *at_DA(DArray *arr, int pos) {
  if (arr == NULL || pos < 0 || pos >= arr->len)
    return NULL;
  return at(arr, pos);
}

void resize_DA(DArray *arr, int new_len) {
  if (arr == NULL || new_len < 0)
    return;

  if (new_len > arr->capacity)
    while (new_len > arr->capacity)
      if (!expand(arr))
        return;

  if (new_len > arr->len) {
    void *dest = (char *)arr->data + arr->len * arr->elem_size;
    int size = arr->elem_size * (new_len - arr->len);
    memset(dest, 0, size);
  }

  arr->len = new_len;
}
