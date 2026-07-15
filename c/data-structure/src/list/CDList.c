#include "list/CDList.h"
#include <stdlib.h>

static void clear(CDList *list, void (*free_cb)(CDNode *node, void *args),
                  void *args) {
  if (list == NULL || list->head == NULL)
    return;

  CDNode *cur = list->head;
  list->tail->next = NULL;
  list->head->previous = NULL;

  while (cur) {
    CDNode *del = cur;
    cur = cur->next;
    if (free_cb)
      free_cb(del, args);
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

static CDNode *find(CDList *list, int pos) {
  if (list == NULL || list->head == NULL || pos < 0 || pos >= list->len)
    return NULL;

  int mid = list->len / 2;

  if (pos > mid) {
    pos = list->len - 1 - pos;
    CDNode *cur = list->tail;
    for (int i = 0; i < pos; ++i) {
      cur = cur->previous;
    }
    return cur;
  }

  CDNode *cur = list->head;
  for (int i = 0; i < pos; ++i) {
    cur = cur->next;
  }
  return cur;
}

static CDNode *cut_node(CDList *list, CDNode *del) {
  if (list == NULL || del == NULL || list->head == NULL)
    return NULL;

  if (list->head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
    list->len = 0;

    del->next = NULL;
    del->previous = NULL;

    return del;
  }

  CDNode *pre = del->previous;
  CDNode *next = del->next;
  pre->next = next;
  next->previous = pre;

  if (del == list->head) {
    list->head = next;
  }

  if (del == list->tail) {
    list->tail = pre;
  }

  --list->len;
  del->next = NULL;
  del->previous = NULL;
  return del;
}

static void add_node(CDList *list, CDNode *pre, CDNode *node) {
  if (list == NULL || node == NULL)
    return;

  if (list->head == NULL) {
    list->head = node;
    list->tail = node;
    node->previous = node;
    node->next = node;
    ++list->len;
    return;
  }

  if (pre == NULL) {
    node->next = list->head;
    node->previous = list->tail;
    list->head->previous = node;
    list->tail->next = node;
    list->head = node;
    ++list->len;
    return;
  }

  if (pre == list->tail) {
    node->next = list->head;
    node->previous = pre;

    list->head->previous = node;
    pre->next = node;

    list->tail = node;
    ++list->len;
    return;
  }

  CDNode *next = pre->next;
  node->next = next;
  node->previous = pre;

  pre->next = node;
  next->previous = node;

  ++list->len;
}

CDList *newCDList() {
  CDList *list = malloc(sizeof(CDList));
  if (list == NULL)
    return NULL;
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

void clear_CDL(CDList *list, void (*free_cb)(CDNode *node, void *args),
               void *args) {
  if (list == NULL || list->head == NULL)
    return;
  clear(list, free_cb, args);
}

void destroy_CDL(CDList *list, void (*free_cb)(CDNode *node, void *args),
                 void *args) {
  if (list == NULL)
    return;
  clear(list, free_cb, args);
  free(list);
}

int size_CDL(CDList *list) {
  if (list == NULL)
    return 0;
  return list->len;
}

CDNode *front_CDL(CDList *list) {
  if (list == NULL)
    return NULL;
  return list->head;
}

CDNode *back_CDL(CDList *list) {
  if (list == NULL)
    return NULL;
  return list->tail;
}

void push_back_CDL(CDList *list, CDNode *node) {
  if (list == NULL || node == NULL)
    return;
  add_node(list, list->tail, node);
}

void push_front_CDL(CDList *list, CDNode *node) {
  if (list == NULL || node == NULL)
    return;
  add_node(list, NULL, node);
}

CDNode *pop_back_CDL(CDList *list) {
  if (list == NULL || list->head == NULL)
    return NULL;

  return cut_node(list, list->tail);
}

CDNode *pop_front_CDL(CDList *list) {
  if (list == NULL || list->head == NULL)
    return NULL;

  return cut_node(list, list->head);
}

CDNode *find_CDL(CDList *list, int pos) {
  if (list == NULL || pos < 0 || pos >= list->len)
    return NULL;
  return find(list, pos);
}

void insert_CDL(CDList *list, CDNode *pre, CDNode *node) {
  if (list == NULL || node == NULL)
    return;

  add_node(list, pre, node);
}

CDNode *erase_CDL(CDList *list, CDNode *del) {
  if (list == NULL || list->head == NULL || del == NULL)
    return NULL;

  return cut_node(list, del);
}

void insert_by_pos_CDL(CDList *list, int pos, CDNode *node) {
  if (list == NULL || pos < 0 || pos > list->len || node == NULL)
    return;
  CDNode *pre = find(list, pos - 1);
  add_node(list, pre, node);
}

CDNode *erase_by_pos_CDL(CDList *list, int pos) {
  if (list == NULL || pos < 0 || pos >= list->len || list->head == NULL)
    return NULL;

  CDNode *del = find(list, pos);
  return cut_node(list, del);
}

void reverse_CDL(CDList *list) {
  if (list == NULL || list->head == NULL || list->len == 1)
    return;

  CDNode *cur = list->head;
  CDNode *pre = list->tail;
  CDNode *next = NULL;

  do {
    next = cur->next;
    cur->next = pre;
    cur->previous = next;
    pre = cur;
    cur = next;
  } while (cur != list->head);

  list->tail = list->head;
  list->head = pre;
}
