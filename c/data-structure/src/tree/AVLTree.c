#include "tree/AVLTree.h"
#include <stdlib.h>

static inline int height(AVLNode *node) { return node ? node->height : 0; }

static inline int max(int a, int b) { return (a > b) ? a : b; }

static inline void update_height(AVLNode *node) {
  if (node)
    node->height = 1 + max(height(node->left), height(node->right));
}

static inline int balance_factor(AVLNode *node) {
  return node ? height(node->left) - height(node->right) : 0;
}

static AVLNode *rotate_right(AVLNode *y) {
  AVLNode *x = y->left;
  AVLNode *T2 = x->right;

  x->right = y;
  y->left = T2;

  update_height(y);
  update_height(x);

  return x;
}

static AVLNode *rotate_left(AVLNode *x) {
  AVLNode *y = x->right;
  AVLNode *T2 = y->left;

  y->left = x;
  x->right = T2;

  update_height(x);
  update_height(y);
  return y;
}

static AVLNode *balance_node(AVLNode *node) {
  if (node == NULL)
    return NULL;

  update_height(node);
  int bf = balance_factor(node);

  if (bf > 1) {
    if (balance_factor(node->left) < 0)
      node->left = rotate_left(node->left);
    return rotate_right(node);
  }

  if (bf < -1) {
    if (balance_factor(node->right) > 0) {
      node->right = rotate_right(node->right);
    }
    return rotate_left(node);
  }
  return node;
}

static AVLNode *insert_rec(AVLNode *root, AVLNode *node,
                           int (*cmp)(AVLNode *, AVLNode *)) {
  if (root == NULL) {
    return node;
  }
  int tmp = cmp(root, node);
  if (tmp > 0) {
    root->left = insert_rec(root->left, node, cmp);
  } else if (tmp < 0) {
    root->right = insert_rec(root->right, node, cmp);
  } else {
    return root;
  }
  return balance_node(root);
}

static AVLNode *delete_min(AVLNode *root, AVLNode **min_out) {
  if (root->left == NULL) {
    *min_out = root;
    return root->right;
  }
  root->left = delete_min(root->left, min_out);
  return balance_node(root);
}

static AVLNode *erase_rec(AVLNode *root, AVLNode *node,
                          int (*cmp)(AVLNode *, AVLNode *),
                          AVLNode **deleted_out) {
  if (root == NULL)
    return NULL;

  int tmp = cmp(root, node);
  if (tmp > 0) {
    root->left = erase_rec(root->left, node, cmp, deleted_out);
    return balance_node(root);
  }
  if (tmp < 0) {
    root->right = erase_rec(root->right, node, cmp, deleted_out);
    return balance_node(root);
  }

  *deleted_out = root;

  if (root->left == NULL) {
    AVLNode *right = root->right;
    root->left = root->right = NULL;
    return right;
  }
  if (root->right == NULL) {
    AVLNode *left = root->left;
    root->left = root->right = NULL;
    return left;
  }

  AVLNode *min = NULL;
  AVLNode *new_right = delete_min(root->right, &min);

  min->left = root->left;
  min->right = new_right;
  root->left = NULL;
  root->right = NULL;

  return balance_node(min);
}

static AVLNode *find_rec(AVLNode *root, AVLNode *node,
                         int (*cmp)(AVLNode *, AVLNode *)) {
  if (root == NULL)
    return NULL;

  int tmp = cmp(root, node);
  if (tmp == 0)
    return root;
  else if (tmp > 0)
    return find_rec(root->left, node, cmp);
  else
    return find_rec(root->right, node, cmp);
}

static void destroy_rec(AVLNode *root, void (*free_cb)(AVLNode *, void *),
                        void *args) {
  if (root == NULL)
    return;

  destroy_rec(root->left, free_cb, args);
  destroy_rec(root->right, free_cb, args);
  if (free_cb)
    free_cb(root, args);
}

AVLTree *newAVLTree() {
  AVLTree *tree = malloc(sizeof(AVLTree));
  if (tree == NULL)
    return NULL;
  tree->root = NULL;
  return tree;
}

void destroy_AVL(AVLTree *tree, void (*free_cb)(AVLNode *, void *),
                 void *args) {
  if (tree == NULL)
    return;
  destroy_rec(tree->root, free_cb, args);
  free(tree);
}

void insert_AVL(AVLTree *tree, AVLNode *node,
                int (*cmp)(AVLNode *, AVLNode *)) {
  if (tree == NULL || node == NULL || cmp == NULL)
    return;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;

  tree->root = insert_rec(tree->root, node, cmp);
}

AVLNode *find_AVL(AVLTree *tree, AVLNode *node,
                  int (*cmp)(AVLNode *, AVLNode *)) {
  if (tree == NULL || node == NULL || cmp == NULL)
    return NULL;
  return find_rec(tree->root, node, cmp);
}

AVLNode *erase_AVL(AVLTree *tree, AVLNode *node,
                   int (*cmp)(AVLNode *, AVLNode *)) {
  if (tree == NULL || node == NULL || cmp == NULL)
    return NULL;

  AVLNode *del = NULL;
  tree->root = erase_rec(tree->root, node, cmp, &del);
  return del;
}

AVLNode *find_min_AVL(AVLTree *tree) {
  if (tree == NULL || tree->root == NULL)
    return NULL;
  AVLNode *cur = tree->root;
  while (cur->left) {
    cur = cur->left;
  }
  return cur;
}

AVLNode *find_max_AVL(AVLTree *tree) {
  if (tree == NULL || tree->root == NULL)
    return NULL;
  AVLNode *cur = tree->root;
  while (cur->right)
    cur = cur->right;
  return cur;
}

int height_AVL(AVLTree *tree) {
  if (tree == NULL)
    return 0;
  return height(tree->root);
}

static int avl_is_balanced_rec(AVLNode *root, int *height) {
  if (root == NULL) {
    *height = 0;
    return 1;
  }

  int left_h, right_h;
  if (!avl_is_balanced_rec(root->left, &left_h))
    return 0;
  if (!avl_is_balanced_rec(root->right, &right_h))
    return 0;

  int bf = left_h - right_h;
  if (bf < -1 || bf > 1)
    return 0;

  *height = 1 + (left_h > right_h ? left_h : right_h);
  return 1;
}

int is_balanced_AVL(AVLTree *tree) {
  if (tree == NULL)
    return 1;
  int height;
  return avl_is_balanced_rec(tree->root, &height);
}
