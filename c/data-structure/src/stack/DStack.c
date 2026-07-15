#include "stack/DStack.h"
#include <stdlib.h>
#include <string.h>

DStack *newDStack(int elem_size, void (*free_cb_)(void *elem, void *args),
                  void *args) {
  if (elem_size <= 0)
    return NULL;
  DStack *stack = malloc(sizeof(DStack));
  if (!stack)
    return NULL;
  stack->free_cb = free_cb_;
  stack->args = args;
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
  clear_DA(stack->arr, stack->free_cb, stack->args);
}

void destroy_DS(DStack *stack) {
  if (!stack)
    return;
  destroy_DA(stack->arr, stack->free_cb, stack->args);
  free(stack);
}

int size_DS(DStack *stack) { return stack ? size_DA(stack->arr) : 0; }

bool empty_DS(DStack *stack) { return stack ? empty_DA(stack->arr) : true; }

void push_DS(DStack *stack, void *elem) {
  if (!stack || !elem)
    return;
  push_back_DA(stack->arr, elem);
}

void pop_DS(DStack *stack) {
  if (!stack || stack->arr->len == 0)
    return;
  if (stack->free_cb) {
    void *elem_ptr = at_DA(stack->arr, stack->arr->len - 1);
    if (elem_ptr)
      stack->free_cb(elem_ptr, stack->args);
  }
  --stack->arr->len;
}

void top_DS(DStack *stack, void *out) {
  if (!stack || stack->arr->len == 0 || !out)
    return;
  void *src = at_DA(stack->arr, stack->arr->len - 1);
  if (src) {
    memcpy(out, src, stack->arr->elem_size);
  }
}
