#include "list/DList.h"
#include <stdlib.h>

static void clear(DList *list, void(free_cb)(DNode *node, void *args),
                  void *args) {
  if (list == NULL || list->head == NULL)
    return;

  DNode *cur = list->head;
  while (cur) {
    DNode *del = cur;
    cur = cur->next;
    if (free_cb)
      free_cb(del, args);
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

static DNode *find(DList *list, int pos) {
  if (list == NULL || pos < 0 || pos >= list->len)
    return NULL;

  int mid = list->len / 2;
  if (pos > mid) {
    DNode *cur = list->tail;
    pos = list->len - pos - 1;
    for (int i = 0; i < pos; ++i) {
      cur = cur->previous;
    }
    return cur;
  }

  DNode *cur = list->head;
  for (int i = 0; i < pos; ++i) {
    cur = cur->next;
  }
  return cur;
}

static void add_node(DList *list, DNode *pre, DNode *node) {
  if (list == NULL || node == NULL)
    return;

  if (list->head == NULL) {
    node->next = NULL;
    node->previous = NULL;

    list->head = node;
    list->tail = node;

    ++list->len;
    return;
  }

  if (pre == NULL) {
    node->next = list->head;
    node->previous = NULL;

    list->head->previous = node;
    list->head = node;

    ++list->len;
    return;
  }

  if (pre == list->tail) {
    node->previous = pre;
    node->next = NULL;

    pre->next = node;
    list->tail = node;

    ++list->len;
    return;
  }

  DNode *next = pre->next;
  node->next = next;
  node->previous = pre;
  pre->next = node;
  next->previous = node;

  ++list->len;
}

static DNode *cut_node(DList *list, DNode *del) {
  if (list == NULL || del == NULL || list->head == NULL)
    return NULL;

  if (list->head == list->tail) {
    del->next = NULL;
    del->previous = NULL;

    list->head = NULL;
    list->tail = NULL;
    --list->len;

    return del;
  }

  if (del == list->head) {
    list->head = del->next;
    list->head->previous = NULL;
    del->next = NULL;
    del->previous = NULL;
    --list->len;

    return del;
  }

  if (del == list->tail) {
    list->tail = del->previous;
    list->tail->next = NULL;
    del->previous = NULL;
    del->next = NULL;
    --list->len;

    return del;
  }

  DNode *pre = del->previous;
  DNode *next = del->next;

  pre->next = next;
  next->previous = pre;

  del->previous = NULL;
  del->next = NULL;

  --list->len;

  return del;
}

DList *newDList() {
  DList *list = malloc(sizeof(DList));
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

void clear_DL(DList *list, void (*free_cb)(DNode *node, void *args),
              void *args) {
  if (list == NULL)
    return;
  clear(list, free_cb, args);
}

void destroy_DL(DList *list, void (*free_cb)(DNode *node, void *args),
                void *args) {
  if (list == NULL)
    return;
  clear(list, free_cb, args);
  free(list);
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

void push_back_DL(DList *list, DNode *node) {
  if (list == NULL || node == NULL)
    return;
  add_node(list, list->tail, node);
}

void push_front_DL(DList *list, DNode *node) {
  if (list == NULL || node == NULL)
    return;
  add_node(list, NULL, node);
}

DNode *pop_back_DL(DList *list) {
  if (list == NULL || list->head == NULL)
    return NULL;
  return cut_node(list, list->tail);
}

DNode *pop_front_DL(DList *list) {
  if (list == NULL || list->head == NULL)
    return NULL;
  return cut_node(list, list->head);
}

DNode *find_DL(DList *list, int pos) {
  if (list == NULL || pos < 0 || pos >= list->len)
    return NULL;
  return find(list, pos);
}

void insert_DL(DList *list, DNode *pre, DNode *node) {
  if (list == NULL || node == NULL)
    return;
  add_node(list, pre, node);
}

DNode *erase_DL(DList *list, DNode *del) {
  if (list == NULL || del == NULL || list->head == NULL)
    return NULL;
  return cut_node(list, del);
}

void insert_by_pos_DL(DList *list, int pos, DNode *node) {
  if (list == NULL || pos < 0 || pos > list->len || node == NULL)
    return;
  DNode *pre = find(list, pos - 1);
  add_node(list, pre, node);
}

DNode *erase_by_pos_DL(DList *list, int pos) {
  if (list == NULL || pos < 0 || pos >= list->len)
    return NULL;
  DNode *del = find(list, pos);
  return cut_node(list, del);
}

void reverse_DL(DList *list) {
  if (list == NULL || list->len == 0 || list->len == 1)
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

  list->tail = list->head;
  list->head = pre;
}
