#include "CDList.h"
#include <stdlib.h>

CDList *newCDList() {
  CDList *list = (CDList *)malloc(sizeof(CDList));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;

  return list;
}

bool empty_CDL(CDList *list) {
  if (list == NULL)
    return true;

  return list->head == NULL;
}

void clear_CDL(CDList *list, void (*free_cb)(CDNode *node)) {
  if (list == NULL || empty_CDL(list))
    return;

  list->tail->next = NULL;
  CDNode *cur = list->head;
  while (cur != NULL) {
    CDNode *del = cur;
    cur = cur->next;
    if (free_cb != NULL)
      free_cb(cur);
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

void destroy_CDL(CDList *list, void (*free_cb)(CDNode *node)) {
  if (list == NULL)
    return;
  clear_CDL(list, free_cb);
  free(list);
}
