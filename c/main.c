#include <stdlib.h>
typedef struct Node {
  int key;
  struct Node *left;
  struct Node *right;
} Node;

typedef struct {
  Node *root;
} BSTree;

int search(Node *node, int key) {
  if (node == NULL)
    return 0;
  if (key == node->key)
    return 1;

  if (key < node->key)
    return search(node, key);
  else
    return search(node->right, key);
}

int main() {}
