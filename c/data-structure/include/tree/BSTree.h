#ifndef BSTREE_H
#define BSTREE_H

typedef struct BSNode {
  struct BSNode *left;
  struct BSNode *right;
} BSNode;

typedef struct BSTree {
  BSNode *root;
} BSTree;

BSTree *newBSTree();
void destroy_BS(BSTree *tree, void (*free_cb)(BSNode *, void *), void *args);

void insert_BS(BSTree *tree, BSNode *node,
               int (*cmp)(BSNode *root, BSNode *new));
BSNode *find_BS(BSTree *tree, BSNode *node,
                int (*cmp)(BSNode *root, BSNode *node));

BSNode *erase_BS(BSTree *tree, BSNode *node,
                 int (*cmp)(BSNode *root, BSNode *node));

#endif
