#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct Node {
  struct Node *next;
} Node;

typedef struct List {
  Node *head;
  Node *tail;
  int len;
} List;

List *newList();
bool empty_L(List *list);
void clear_L(List *list, void (*free_cb)(Node *node));
void destroy_L(List *list, void (*free_cb)(Node *node));

int size_L(List *list);
Node *front_L(List *list);
Node *back_L(List *list);
void print_L(List *list, void (*print_cb)(Node *node));

void push_back_L(List *list, Node *node);
void push_front_L(List *list, Node *node);
Node *pop_back_L(List *list);
Node *pop_front_L(List *list);

Node *find_L(List *list, int pos);
void insert_L(List *list, int pos, Node *node);
void erase_L(List *list, int pos, void (*free_cb)(Node *node));
void reverse_L(List *list);

#endif
