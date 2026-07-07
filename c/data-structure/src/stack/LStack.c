#include "LStack.h"
#include <stdlib.h>

LStack *newLStack() {
  LStack *stack = malloc(sizeof(LStack));
  if (!stack)
    return NULL;
  stack->list = newList();

  if (!stack->list) {
    free(stack);
    return NULL;
  }

  return stack;
}

void clear_LS(LStack *stack, void (*free_cb)(Node *node)) {
  if (!stack)
    return;
  clear_L(stack->list, free_cb);
}

void destroy_LS(LStack *stack, void (*free_cb)(Node *node)) {
  if (!stack)
    return;
  destroy_L(stack->list, free_cb);
  free(stack);
}

int size_LS(LStack *stack) {
  if (stack == 0)
    return 0;
  return size_L(stack->list);
}

bool empty_LS(LStack *stack) {
  if (stack == NULL)
    return true;
  return empty_L(stack->list);
}

void push_LS(LStack *stack, Node *node) {
  if (stack == NULL)
    return;
  push_front_L(stack->list, node);
}

Node *pop_LS(LStack *stack) {
  if (stack == NULL || size_L(stack->list) == 0)
    return NULL;
  return pop_front_L(stack->list);
}

Node *top_LS(LStack *stack) {
  if (stack == NULL)
    return NULL;
  return front_L(stack->list);
}
