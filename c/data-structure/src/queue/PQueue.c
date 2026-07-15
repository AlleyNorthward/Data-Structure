#include "queue/PQueue.h"
#include "tree/heap.h"
#include <stddef.h>
#include <stdlib.h>

PQueue *newPQueue(int capacity, int elem_size, cmp_t cmp,
                  void (*free_cb)(void *elem, void *args), void *args) {
  if (elem_size <= 0 || cmp == NULL)
    return NULL;

  PQueue *pq = malloc(sizeof(PQueue));
  if (pq == NULL)
    return NULL;

  pq->arr = newDArray(capacity, elem_size);
  if (pq->arr == NULL) {
    free(pq);
    return NULL;
  }

  pq->cmp = cmp;
  pq->free_cb = free_cb;
  pq->args = args;

  return pq;
}

void destroy_PQ(PQueue *pq) {
  if (pq == NULL)
    return;
  destroy_DA(pq->arr, pq->free_cb, pq->args);
  free(pq);
}

void clear_PQ(PQueue *pq) {
  if (pq == NULL)
    return;

  clear_DA(pq->arr, pq->free_cb, pq->args);
}

void push_PQ(PQueue *pq, void *elem) {
  if (pq == NULL || elem == NULL)
    return;
  int old_len = pq->arr->len;
  push_back_DA(pq->arr, elem);
  if (pq->arr->len > old_len) {
    push_heap(pq->arr->data, pq->arr->len, pq->arr->elem_size, pq->cmp);
  }
}

void pop_PQ(PQueue *pq) {
  if (pq == NULL || empty_DA(pq->arr))
    return;

  pop_heap(pq->arr->data, pq->arr->len, pq->arr->elem_size, pq->cmp);
  --pq->arr->len;
}

void *top_PQ(PQueue *pq) {
  if (pq == NULL || empty_DA(pq->arr))
    return NULL;

  return pq->arr->data;
}

int size_PQ(PQueue *pq) {
  if (pq == NULL)
    return 0;
  return pq->arr->len;
}

bool empty_PQ(PQueue *pq) {
  if (pq == NULL)
    return true;
  return empty_DA(pq->arr);
}
