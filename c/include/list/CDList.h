#ifndef CDLIST_H
#define CDLIST_H

#include <stdbool.h>

typedef struct CDNode {
  struct CDNode *next;
  struct CDNode *previous;
} CDNode;

typedef struct CDList {
  CDNode *head;
  CDNode *tail;
  int len;
} CDList;

CDList *newCDList();
bool empty_CDL(CDList *list);
void clear_CDL(CDList *list, void (*free_cb)(CDNode *node));
void destroy_CDL(CDList *list, void (*free_cb)(CDNode *node));

#endif
