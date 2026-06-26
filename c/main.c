#include "List.h"
#include "stdio.h"
#include <stdlib.h>

typedef struct Item {
  Node node;
  int val;
} Item;

Item *newItems(int val) {
  Item *it = (Item *)malloc(sizeof(Item));
  it->val = val;
  it->node.next = NULL;
  return it;
}

void print_item(Node *node) {
  Item *it = container_of(node, Item, node);
  printf("%d ", it->val);
}

void free_item(Node *node) {
  Item *it = container_of(node, Item, node);
  free(it);
}

int main() {
  List *list = newList();

  Item *i1 = newItems(1);
  Item *i2 = newItems(2);
  Item *i3 = newItems(3);
  Item *i4 = newItems(4);

  push_back_L(list, &i1->node);
  push_back_L(list, &i2->node);
  push_back_L(list, &i3->node);
  push_back_L(list, &i4->node);

  print_L(list, print_item);
  printf("\n");

  Node *front_node = front_L(list);
  Item *it = container_of(front_node, Item, node);
  printf("头部: %d\n", it->val);

  destory_L(list, free_item);
}
