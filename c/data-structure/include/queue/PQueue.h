#ifndef PQUEUE_H
#define PQUEUE_H

#include "array/DArray.h"
#include "dsdef.h"

typedef struct PQueue {
  DArray *arr;
  cmp_t cmp;
  void (*free_cb)(void *elem, void *args);
  void* args;
} PQueue;

PQueue *newPQueue(int capacity, int elem_size, cmp_t cmp,
                  void (*free_cb)(void *elem, void *args), void *args);
void destroy_PQ(PQueue *pq);
void clear_PQ(PQueue* pq);

void push_PQ(PQueue *pq, void *elem);
void pop_PQ(PQueue *pq);
void *top_PQ(PQueue *pq);
void reserve_PQ(PQueue* pq, int new_capacity);

bool empty_PQ(PQueue* pq);
int size_PQ(PQueue* pq);

#endif
