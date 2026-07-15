#ifndef LSTACK_H
#define LSTACK_H

#include "list/List.h"

typedef struct LStack {
  List *list;
} LStack;

LStack *newLStack();
void destroy_LS(LStack *stack, void (*free_cb)(Node *node, void *args),
                void *args);
void clear_LS(LStack *stack, void (*free_cb)(Node *node, void *args),
              void *args);
int size_LS(LStack *stack);
bool empty_LS(LStack *stack);

void push_LS(LStack *stack, Node *node);
Node *pop_LS(LStack *stack);
Node *top_LS(LStack *stack);

#endif
