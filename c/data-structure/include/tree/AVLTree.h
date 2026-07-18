#ifndef AVLTREE_H
#define AVLTREE_H

typedef struct AVLNode {
  struct AVLNode *left;
  struct AVLNode *right;
  int height;
} AVLNode;

typedef struct AVLTree {
  AVLNode *root;
} AVLTree;

AVLTree *newAVLTree();
void destroy_AVL(AVLTree *tree, void (*free_cb)(AVLNode *, void *), void *args);
void insert_AVL(AVLTree *tree, AVLNode *node,
                int (*cmp)(AVLNode *root, AVLNode *node));
AVLNode *find_AVL(AVLTree *tree, AVLNode *node,
                  int (*cmp)(AVLNode *root, AVLNode *node));
AVLNode *erase_AVL(AVLTree *tree, AVLNode *node,
                   int (*cmp)(AVLNode *root, AVLNode *node));

AVLNode *find_min_AVL(AVLTree *tree);
AVLNode *find_max_AVL(AVLTree *tree);
int height_AVL(AVLTree *tree);
int is_balanced_AVL(AVLTree *tree);

#endif
