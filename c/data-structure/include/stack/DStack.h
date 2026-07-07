#ifndef DSTACK_H
#define DSTACK_H

#include "DArray.h"

#define PUSH_INT_DS(stack, val) push_DS(stack, &(int){val})

typedef struct DStack {
  DArray *arr;
  void (*free_cb)(void*);
} DStack;

DStack *newDStack(int elem_size, void (*free_cb_)(void*));
void destroy_DS(DStack *stack);
void clear_DS(DStack* stack);
int size_DS(DStack *stack);
bool empty_DS(DStack *stack);

void push_DS(DStack *stack, void *elem);
void pop_DS(DStack *stack);
void top_DS(DStack *stack, void *out);

#endif
