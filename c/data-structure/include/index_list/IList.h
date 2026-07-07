#ifndef ILIST_H
#define ILIST_H

#include "DArray.h"
#include "List.h"

typedef struct IList {
  List *list;
  DArray *array;
  int max_key;
} IList;

IList *newIList(int max_key_);
void clear_IL(IList *list, void (*free_cb)(Node *node));
bool empty_IL(IList *list);
void destroy_IL(IList *list, void (*free_cb)(Node *node));

int size_IL(IList *list);
Node *front_IL(IList *list);
Node *back_IL(IList *list);
void print_IL(IList *list, void (*print_cb)(Node *node));

void push_back_IL(IList *list, Node *node, int (*key_cb)(Node *node));
Node *pop_back_IL(IList *list, int (*key_cb)(Node *node));
void push_front_IL(IList *list, Node *node, int (*key_cb)(Node *node));
Node *pop_front_IL(IList *list, int (*key_cb)(Node *node));

// no overload haha
void insert_IL(IList *list, Node *node, int pos, int (*key_cb)(Node *node));
void erase_IL(IList *list, int pos, void (*free_cb)(Node *node),
              int (*key_cb)(Node *node));

#endif
