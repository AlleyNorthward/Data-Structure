#ifndef CLIST_H
#define CLIST_H

#include <stdbool.h>

typedef struct CNode {
  struct CNode *next;
} CNode;

typedef struct CList {
  CNode *head;
  CNode *tail;
  int len;
} CList;

CList *newCList();
bool empty_CL(CList *list);
void clear_CL(CList *list, void (*free_cb)(CNode *node));
void destroy_CL(CList *list, void (*free_cb)(CNode *node));

int size_CL(CList *list);
CNode *front_CL(CList *list);
CNode *back_CL(CList *list);
void print_CL(CList *list, void (*print_cb)(CNode *node));

void push_back_CL(CList *list, CNode *node);
void push_front_CL(CList *list, CNode *node);
CNode *pop_back_CL(CList *list);
CNode *pop_front_CL(CList *list);

void insert_CL(CList* list, int pos, CNode* node);
void erase_CL(CList* list, int pos, void (* free_cb)(CNode* node));
void reverse_CL(CList* list);
#endif
