#ifndef LSTACK_H
#define LSTACK_H

#include "List.h"

typedef struct LStack {
  List *list;
} LStack;

LStack *newLStack();
void destroy_LS(LStack *stack, void (*free_cb)(Node *node));
void clear_LS(LStack *stack, void (*free_cb)(Node *node));
int size_LS(LStack *stack);
bool empty_LS(LStack *stack);

void push_LS(LStack *stack, Node *node);
Node *pop_LS(LStack *stack);
Node *top_LS(LStack *stack);

#endif
