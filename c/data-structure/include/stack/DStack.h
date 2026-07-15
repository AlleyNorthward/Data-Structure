#ifndef DSTACK_H
#define DSTACK_H

#include "array/DArray.h"

typedef struct DStack {
  DArray *arr;
  void (*free_cb)(void *elem, void *args);
  void *args;
} DStack;

DStack *newDStack(int elem_size, void (*free_cb_)(void *elem, void *args),
                  void *args);
void destroy_DS(DStack *stack);
void clear_DS(DStack *stack);
int size_DS(DStack *stack);
bool empty_DS(DStack *stack);

void push_DS(DStack *stack, void *elem);
void pop_DS(DStack *stack);
void top_DS(DStack *stack, void *out);

#endif
