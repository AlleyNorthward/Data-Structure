#ifndef HEAP_H
#define HEAP_H

#include "dsdef.h"

void make_heap(void *base, int n, int elem_size, cmp_t);
void push_heap(void *base, int n, int elem_size, cmp_t);
void pop_heap(void *base, int n, int elem_size, cmp_t);
void sort_heap(void *base, int n, int elem_size, cmp_t);

#endif
