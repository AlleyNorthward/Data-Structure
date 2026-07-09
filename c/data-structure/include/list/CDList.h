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

int size_CDL(CDList *list);
CDNode *front_CDL(CDList *list);
CDNode *back_CDL(CDList *list);
void print_CDL(CDList *list, void (*print_cb)(CDNode *node));

void push_back_CDL(CDList *list, CDNode *node);
void push_front_CDL(CDList *list, CDNode *node);
CDNode *pop_back_CDL(CDList *list);
CDNode *pop_front_CDL(CDList *list);

CDNode* find_CDL(CDList* list, int pos);
void reverse_CDL(CDList *list);
void insert_CDL(CDList* list, CDNode* pre, CDNode* node);
void erase_CDL(CDList* list, CDNode* node, void(*free_cb)(CDNode* node));

void insert_by_pos_CDL(CDList *list, int pos, CDNode *node);
void erase_by_pos_CDL(CDList *list, int pos, void (*free_cb)(CDNode *node));

#endif
