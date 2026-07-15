#include "list/CList.h"
#include <stdlib.h>

static void clear(CList *list, void (*free_cb)(CNode *node, void *args),
                  void *args) {
  if (list == NULL || list->head == NULL)
    return;
  CNode *cur = list->head;

  list->tail->next = NULL;
  while (cur) {
    CNode *del = cur;
    cur = cur->next;
    if (free_cb)
      free_cb(del, args);
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

CNode *find(CList *list, int pos) {
  if (list == NULL || pos < 0 || pos >= list->len || list->head == NULL)
    return NULL;

  CNode *cur = list->head;
  for (int i = 0; i < pos; ++i) {
    cur = cur->next;
  }
  return cur;
}

static void add_node(CList *list, CNode *pre, CNode *node) {
  if (list == NULL || node == NULL)
    return;

  if (list->head == NULL) {
    node->next = node;
    list->head = node;
    list->tail = node;
    ++list->len;
    return;
  }

  if (pre == NULL) {
    node->next = list->head;
    list->tail->next = node;
    list->head = node;
    ++list->len;
    return;
  }

  if (pre == list->tail) {
    node->next = list->head;
    list->tail->next = node;
    list->tail = node;
    ++list->len;
    return;
  }

  CNode *next = pre->next;
  node->next = next;
  pre->next = node;

  ++list->len;
}

static CNode *cut_node(CList *list, CNode *pre) {
  if (list == NULL || list->head == NULL || pre == list->tail)
    return NULL;

  CNode *del = NULL;
  if (list->head == list->tail) {
    del = list->head;
    del->next = NULL;

    list->head = NULL;
    list->tail = NULL;
    --list->len;
    return del;
  }

  if (pre == NULL) {
    del = list->head;
    list->head = del->next;
    list->tail->next = list->head;
    del->next = NULL;
    --list->len;

    return del;
  }

  if (pre->next == list->tail) {
    del = list->tail;
    list->tail = pre;
    pre->next = list->head;
    del->next = NULL;

    --list->len;
    return del;
  }

  del = pre->next;
  pre->next = del->next;
  del->next = NULL;

  --list->len;
  return del;
}

CList *newCList() {
  CList *list = malloc(sizeof(CList));
  if (list == NULL)
    return NULL;

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
  return list;
}

bool empty_CL(CList *list) {
  if (list == NULL)
    return true;

  return list->head == NULL;
}

void clear_CL(CList *list, void (*free_cb)(CNode *node, void *args),
              void *args) {
  if (list == NULL)
    return;
  clear(list, free_cb, args);
}

void destroy_CL(CList *list, void (*free_cb)(CNode *node, void *args),
                void *args) {
  if (list == NULL)
    return;
  clear(list, free_cb, args);
  free(list);
}

int size_CL(CList *list) {
  if (list == NULL)
    return 0;
  return list->len;
}

CNode *front_CL(CList *list) {
  if (list == NULL)
    return NULL;
  return list->head;
}

CNode *back_CL(CList *list) {
  if (list == NULL)
    return NULL;
  return list->tail;
}

void push_back_CL(CList *list, CNode *node) {
  if (list == NULL || node == NULL)
    return;
  add_node(list, list->tail, node);
}

void push_front_CL(CList *list, CNode *node) {
  if (list == NULL || node == NULL)
    return;
  add_node(list, NULL, node);
}

CNode *pop_back_CL(CList *list) {
  if (list == NULL || list->head == NULL)
    return NULL;
  CNode *pre = find(list, list->len - 2);
  return cut_node(list, pre);
}

CNode *pop_front_CL(CList *list) {
  if (list == NULL || list->head == NULL)
    return NULL;
  return cut_node(list, NULL);
}

CNode *find_CL(CList *list, int pos) {
  if (list == NULL || pos < 0 || pos >= list->len)
    return NULL;
  return find(list, pos);
}

void insert_CL(CList *list, CNode *pre, CNode *node) {
  if (list == NULL || node == NULL)
    return;
  add_node(list, pre, node);
}

CNode *erase_CL(CList *list, CNode *pre) {
  if (list == NULL || list->head == NULL)
    return NULL;

  return cut_node(list, pre);
}

void insert_by_pos_CL(CList *list, int pos, CNode *node) {
  if (list == NULL || pos < 0 || pos > list->len || node == NULL)
    return;

  CNode *pre = find(list, pos - 1);
  add_node(list, pre, node);
}

CNode *erase_by_pos_CL(CList *list, int pos) {
  if (list == NULL || pos < 0 || pos >= list->len || list->head == NULL)
    return NULL;
  CNode *pre = find(list, pos - 1);
  CNode *del = cut_node(list, pre);
  return del;
}

void reverse_CL(CList *list) {
  if (list == NULL || list->head == NULL || list->len == 1)
    return;

  CNode *cur = list->head;
  CNode *pre = list->tail;
  CNode *next = NULL;

  do {
    next = cur->next;
    cur->next = pre;
    pre = cur;
    cur = next;
  } while (cur != list->head);

  list->tail = list->head;
  list->head = pre;
}
