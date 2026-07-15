#include "tree/heap.h"
#include "dscommon.h"
#include <stdlib.h>
#include <string.h>

static inline char *elem_at(void *base, int index, int elem_size) {
  return (char *)base + index * elem_size;
}

static void sift_up(void *base, int hole, int elem_size, cmp_t cmp) {
  char *arr = (char *)base;
  char *tmp = malloc(elem_size);
  if (!tmp)
    return;

  memcpy(tmp, elem_at(base, hole, elem_size), elem_size);
  while (hole > 0) {
    int parent = (hole - 1) / 2;
    char *parent_ptr = elem_at(base, parent, elem_size);
    if (cmp(parent_ptr, tmp)) {
      break;
    }
    memcpy(elem_at(base, hole, elem_size), parent_ptr, elem_size);
    hole = parent;
  }
  memcpy(elem_at(base, hole, elem_size), tmp, elem_size);
  free(tmp);
}

static void sift_down(void *base, int hole, int n, int elem_size, cmp_t cmp) {
  char *arr = (char *)base;
  char *tmp = malloc(elem_size);

  if (!tmp)
    return;

  memcpy(tmp, elem_at(base, hole, elem_size), elem_size);

  int child;

  while ((child = 2 * hole + 1) < n) {
    if (child + 1 < n && cmp(elem_at(base, child + 1, elem_size),
                             elem_at(base, child, elem_size))) {
      ++child;
    }
    if (cmp(tmp, elem_at(base, child, elem_size)))
      break;
    memcpy(elem_at(base, hole, elem_size), elem_at(base, child, elem_size),
           elem_size);
    hole = child;
  }
  memcpy(elem_at(base, hole, elem_size), tmp, elem_size);
  free(tmp);
}

void make_heap(void *base, int n, int elem_size, cmp_t cmp) {
  if (base == NULL || n <= 1 || cmp == NULL)
    return;
  int start = (n - 2) / 2;
  for (int i = start; i >= 0; --i) {
    sift_down(base, i, n, elem_size, cmp);
  }
}

void push_heap(void *base, int n, int elem_size, cmp_t cmp) {
  if (base == NULL || n <= 1 || cmp == NULL)
    return;
  sift_up(base, n - 1, elem_size, cmp);
}

void pop_heap(void *base, int n, int elem_size, cmp_t cmp) {
  if (base == NULL || n <= 1 || cmp == NULL)
    return;
  swap_bytes(elem_at(base, 0, elem_size), elem_at(base, n - 1, elem_size),
             elem_size);
  sift_down(base, 0, n - 1, elem_size, cmp);
}

void sort_heap(void *base, int n, int elem_size, cmp_t cmp) {
  if (base == NULL || n <= 1 || cmp == NULL)
    return;
  for (int i = n; i > 1; --i) {
    pop_heap(base, i, elem_size, cmp);
  }
}
