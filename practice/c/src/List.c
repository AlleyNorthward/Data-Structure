#include "List.h"
#include <stdlib.h>

List *newList() {
  List *list = (List *)malloc(sizeof(List));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;

  return list;
}

void clear(List *list, void (*free_cb)(Node *node)) {
  if (list == NULL || free_cb == NULL)
    return;

  Node *cur = list->head;
  while (cur) {
    Node *del = cur;
    cur = cur->next;
    free_cb(del);
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

void destory_L(List *list, void (*free_cb)(Node *node)) {
  clear(list, free_cb);
  free(list);
}
