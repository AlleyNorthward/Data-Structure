#ifndef DLIST_H
#define DLIST_H

#include <stdbool.h>

typedef struct DNode {
  struct DNode *next;
  struct DNode *previous;
} DNode;

typedef struct DList {
  DNode *head;
  DNode *tail;
  int len;
} DList;

DList *newDList();
bool empty_DL(DList *list);
void clear_DL(DList *list, void (*free_cb)(DNode *node));
void destroy_DL(DList *list, void (*free_cb)(DNode *node));

int size_DL(DList *list);
DNode *front_DL(DList *list);
DNode *back_DL(DList *list);
void print_DL(DList *list, void (*print_cb)(DNode *node));

void push_back_DL(DList *list, DNode *node);
void push_front_DL(DList *list, DNode *node);
DNode *pop_back_DL(DList *list);
DNode *pop_front_DL(DList *list);

void insert_DL(DList* list, int pos, DNode* node);
void erase_DL(DList* list, int pos, void(*free_cb)(DNode* node));
void reverse_DL(DList* list);

#endif
