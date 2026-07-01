#ifndef DLIST_H
#define DLIST_H

typedef struct DNode{
  struct Node* next;
  struct Node* previous;
}DNode;

typedef struct DList{
  DNode* head;
  DNode* tail;
}DList;

DList* newList();
void push_back_L(DList* list);

#endif
