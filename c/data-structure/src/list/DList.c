#include "DList.h"
#include <stdio.h>
#include <stdlib.h>

DList *newDList() {
  DList *list = (DList *)malloc(sizeof(DList));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;

  return list;
}

bool empty_DL(DList *list) {
  if (list == NULL)
    return true;
  return list->head == NULL;
}

void clear_DL(DList *list, void (*free_cb)(DNode *node)) {
  if (list == NULL || empty_DL(list))
    return;

  DNode *cur = list->head;
  while (cur) {
    DNode *del = cur;
    cur = cur->next;
    if (free_cb != NULL)
      free_cb(del);
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

void destroy_DL(DList *list, void (*free_cb)(DNode *node)) {
  if (list == NULL)
    return;

  clear_DL(list, free_cb);
  free(list);
}

void push_back_DL(DList *list, DNode *node) {
  if (list == NULL || node == NULL)
    return;

  node->previous = list->tail;
  node->next = NULL;

  if (empty_DL(list)) {
    list->head = node;
  } else {
    list->tail->next = node;
  }
  list->tail = node;
  ++list->len;
}

void push_front_DL(DList *list, DNode *node) {
  if (list == NULL || node == NULL)
    return;

  node->next = list->head;
  node->previous = NULL;

  if (empty_DL(list)) {
    list->tail = node;
  } else {
    list->head->previous = node;
  }
  list->head = node;
  ++list->len;
}

DNode *pop_back_DL(DList *list) {
  if (list == NULL || empty_DL(list))
    return NULL;

  DNode *del = list->tail;
  if (list->head == list->tail) {
    del->previous = NULL;
    del->next = NULL;

    list->head = NULL;
    list->tail = NULL;
    --list->len;
    return del;
  }

  DNode *pre = del->previous;
  pre->next = NULL;
  del->next = NULL;
  del->previous = NULL;
  list->tail = pre;
  --list->len;

  return del;
}

DNode *pop_front_DL(DList *list) {
  if (list == NULL || empty_DL(list))
    return NULL;

  DNode *del = list->head;
  if (list->head == list->tail) {
    del->previous = NULL;
    del->next = NULL;

    list->head = NULL;
    list->tail = NULL;
    --list->len;
    return del;
  }

  DNode *next = del->next;
  next->previous = NULL;
  del->next = NULL;
  del->previous = NULL;
  list->head = next;
  --list->len;

  return del;
}

int size_DL(DList *list) {
  if (list == NULL)
    return 0;
  return list->len;
}

DNode *front_DL(DList *list) {
  if (list == NULL)
    return NULL;
  return list->head;
}

DNode *back_DL(DList *list) {
  if (list == NULL)
    return NULL;
  return list->tail;
}

void print_DL(DList *list, void (*print_cb)(DNode *node)) {
  if (list == NULL || print_cb == NULL || empty_DL(list)) {
    putchar('\n');
    return;
  }

  DNode *cur = list->head;
  while (cur) {
    print_cb(cur);
    cur = cur->next;
  }

  putchar('\n');
}

void insert_DL(DList *list, int pos, DNode *node) {
  if (list == NULL || node == NULL || pos < 0 || pos > size_DL(list))
    return;

  if (pos == 0) {
    push_front_DL(list, node);
    return;
  }

  if (pos == size_DL(list)) {
    push_back_DL(list, node);
    return;
  }

  DNode *cur = list->head;
  for (int i = 0; i < pos - 1; ++i) {
    cur = cur->next;
  }

  node->next = cur->next;
  node->previous = cur;

  cur->next = node;
  cur->next->previous = node;

  ++list->len;
}

void erase_DL(DList *list, int pos, void (*free_cb)(DNode *node)) {
  if (list == NULL || pos < 0 || pos > size_DL(list) - 1 || empty_DL(list))
    return;

  if (pos == 0) {
    pop_front_DL(list);
    return;
  }

  if (pos == size_DL(list) - 1) {
    pop_back_DL(list);
    return;
  }

  DNode *cur = list->head;
  for (int i = 0; i < pos - 1; ++i) {
    cur = cur->next;
  }

  DNode *del = cur->next;
  cur->next = del->next;
  del->next->previous = cur;

  del->next = NULL;
  del->previous = NULL;
  --list->len;
  free_cb(del);
}

void reverse_DL(DList *list) {
  if (list == NULL || empty_DL(list))
    return;

  if (size_DL(list) == 1)
    return;

  DNode *cur = list->head;
  DNode *pre = NULL;
  DNode *next = NULL;

  while (cur) {
    next = cur->next;
    cur->next = pre;
    cur->previous = next;

    pre = cur;
    cur = next;
  }

  list->head = list->tail;
  list->tail = pre;
}

DNode* find_DL(DList* list, int pos){
  if(list == NULL || pos < 0 || pos > size_DL(list))
    return NULL;

  if(pos == size_DL(list))
    pos = pos - 1;

  DNode* cur = front_DL(list);
  for(int i = 0; i < pos; ++i){
    cur = cur->next;
  }

  return cur;
}
