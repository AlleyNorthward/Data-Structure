#ifndef LIST_H
#define LIST_H

#include "config.h"
#include "stdbool.h"

typedef struct Node {
  struct Node *next;
} Node;

typedef struct List {
  Node *head;
  Node *tail;
  int len;
} List;

List *newList();
void push_back_L(List *list, Node *node);
Node *pop_back_L(List *list);
void push_front_L(List *list, Node *node);
Node *pop_front_L(List *list);
Node *front_L(List *list);
Node *back_L(List *list);
bool empty_L(List *list);
int size_L(List *list);
void insert_L(List *list, int pos, Node *node);

void print_L(List *list, void (*print_cb)(Node *node));
void destory_L(List *list, void (*free_cb)(Node *node));
void clear(List *list, void (*free_cb)(Node *node));
#endif
