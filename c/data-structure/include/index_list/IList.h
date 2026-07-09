#ifndef ILIST_H
#define ILIST_H

#include "CDList.h"
#include "DArray.h"

typedef struct IList {
  CDList *list;
  DArray *array;
  int max_key;
} IList;

IList *newIList(int max_key_);
void clear_IL(IList *list, void (*free_cb)(CDNode *node));
bool empty_IL(IList *list);
void destroy_IL(IList *list, void (*free_cb)(CDNode *node));

int size_IL(IList *list);
CDNode *front_IL(IList *list);
CDNode *back_IL(IList *list);
void print_IL(IList *list, void (*print_cb)(CDNode *node));

void push_back_IL(IList *list, CDNode *node, int (*get_val)(CDNode *node));
CDNode *pop_back_IL(IList *list, int (*get_val)(CDNode *node));
void push_front_IL(IList *list, CDNode *node, int (*get_val)(CDNode *node));
CDNode *pop_front_IL(IList *list, int (*get_val)(CDNode *node));

CDNode *find_by_pos_IL(IList *list, int pos);
void insert_IL(IList *list, CDNode *pre, CDNode *node,
               int (*get_val)(CDNode *node));
void erase_IL(IList *list, CDNode *node, int (*get_val)(CDNode *node),
              void(free_cb)(CDNode *node));

CDNode *find_by_val_IL(IList *list, int val);
void insert_by_val_IL(IList *list, CDNode *node, int val,
                      int (*get_val)(CDNode *node));
void erase_by_val_IL(IList* list, int val, int (*get_val)(CDNode*node), void(*free_cb)(CDNode* node));

#endif
