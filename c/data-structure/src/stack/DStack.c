#include "DStack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

DStack *newDStack(int elem_size, void (*free_cb_)(void *)) {
  if (elem_size <= 0)
    return NULL;

  DStack *stack = malloc(sizeof(DStack));
  if (!stack)
    return NULL;

  stack->free_cb = free_cb_;

  stack->arr = newDArray(8, elem_size);
  if (!stack->arr) {
    free(stack);
    return NULL;
  }

  return stack;
}

void clear_DS(DStack *stack) {
  if (!stack)
    return;
  clear_DA(stack->arr, stack->free_cb);
}

void destroy_DS(DStack *stack) {
  if (!stack)
    return;
  clear_DS(stack);
  free(stack->arr);
  free(stack);
}

int size_DS(DStack *stack) {
  if (!stack)
    return 0;
  return size_DA(stack->arr);
}

bool empty_DS(DStack *stack) {
  if (!stack)
    return true;
  return empty_DA(stack->arr);
}

void push_DS(DStack *stack, void *elem) {
  if (!stack)
    return;
  push_back_DA(stack->arr, elem);
}

void pop_DS(DStack *stack) {
  if (stack->arr->len == 0)
    return;

  if (stack->free_cb) {
    void *elem_ptr = (char *)stack->arr->data +
                     (stack->arr->len - 1) * stack->arr->elem_size;
    stack->free_cb(elem_ptr);
  }
  --stack->arr->len;
}

void top_DS(DStack *stack, void *out) {
  if (stack->arr->len == 0)
    return;
  memcpy(out,
         (char *)stack->arr->data +
             (stack->arr->len - 1) * stack->arr->elem_size,
         stack->arr->elem_size);
}
