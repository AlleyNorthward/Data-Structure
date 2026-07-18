#ifndef RBTREE_H
#define RBTREE_H

typedef enum { RB_RED = 0, RB_BLACK = 1 } RBColor;

typedef struct RBNode {
  struct RBNode *parent;
  struct RBNode *left;
  struct RBNode *right;
  RBColor color;
} RBNode;

typedef struct RBTree {
  RBNode *root;
} RBTree;

RBTree *newRBTree();
void destroy_RB(RBTree *tree, void (*free_cb)(RBNode *, void *), void *args);
void insert_RB(RBTree *tree, RBNode *node,
               int (*cmp)(RBNode *root, RBNode *node));
RBNode *find_RB(RBTree *tree, RBNode *node,
                int (*cmp)(RBNode *root, RBNode *node));
RBNode *erase_RB(RBTree *tree, RBNode *node,
                 int (*cmp)(RBNode *root, RBNode *node));

RBNode *next_RB(RBNode *node);
RBNode *prev_RB(RBNode *node);

RBNode *find_min_RB(RBTree *tree);
RBNode *find_max_RB(RBTree *tree);

#endif
