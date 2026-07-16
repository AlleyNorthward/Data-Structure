#include "tree/BSTree.h"
#include <stdlib.h>
static BSNode *cut_node(BSNode *root) {
  if (root == NULL)
    return NULL;

  if (root->left == NULL) {
    BSNode *new_root = root->right;
    root->right = NULL;
    return new_root;
  }

  if (root->right == NULL) {
    BSNode *new_root = root->left;
    root->left = NULL;
    return new_root;
  }

  BSNode *new_root = root->right;
  BSNode *parent = root;

  while (new_root->left) {
    parent = new_root;
    new_root = new_root->left;
  }

  if (parent == root) {
    parent->right = new_root->right;
  } else {
    parent->left = new_root->right;
  }

  new_root->left = root->left;
  new_root->right = root->right;
  root->left = NULL;
  root->right = NULL;

  return new_root;
}

static BSNode *erase_rec(BSNode *root, BSNode *node,
                         int (*cmp)(BSNode *, BSNode *), BSNode **deled_node) {
  if (root == NULL)
    return NULL;

  int tmp = cmp(root, node);
  if (tmp > 0)
    root->left = erase_rec(root->left, node, cmp, deled_node);
  else if (tmp < 0)
    root->right = erase_rec(root->right, node, cmp, deled_node);
  else {
    *deled_node = root;
    return cut_node(root);
  }
  return root;
}

static BSNode *find_rec(BSNode *root, BSNode *node,
                        int (*cmp)(BSNode *, BSNode *)) {
  if (root == NULL)
    return NULL;

  int tmp = cmp(root, node);
  if (tmp == 0)
    return root;
  else if (tmp > 0)
    return find_rec(root->left, node, cmp);
  return find_rec(root->right, node, cmp);
}

static void destroy_rec(BSNode *root, void (*free_cb)(BSNode *, void *),
                        void *args) {
  if (root == NULL)
    return;
  destroy_rec(root->left, free_cb, args);
  destroy_rec(root->right, free_cb, args);
  if (free_cb)
    free_cb(root, args);
}

static BSNode *insert_rec(BSNode *root, BSNode *node,
                          int (*cmp)(BSNode *, BSNode *)) {
  if (root == NULL)
    return node;
  int tmp = cmp(root, node);
  if (tmp > 0)
    root->left = insert_rec(root->left, node, cmp);
  else if (tmp < 0)
    root->right = insert_rec(root->right, node, cmp);

  return root;
}

BSTree *newBSTree() {
  BSTree *tree = malloc(sizeof(BSTree));
  if (tree == NULL)
    return NULL;
  tree->root = NULL;
  return tree;
}

void destroy_BS(BSTree *tree, void (*free_cb)(BSNode *, void *), void *args) {
  if (tree == NULL)
    return;
  destroy_rec(tree->root, free_cb, args);
  free(tree);
}

void insert_BS(BSTree *tree, BSNode *node, int (*cmp)(BSNode *, BSNode *)) {
  if (tree == NULL || node == NULL || cmp == NULL)
    return;
  node->left = NULL;
  node->right = NULL;
  tree->root = insert_rec(tree->root, node, cmp);
}

BSNode *find_BS(BSTree *tree, BSNode *node, int (*cmp)(BSNode *, BSNode *)) {
  if (tree == NULL || node == NULL || cmp == NULL)
    return NULL;
  return find_rec(tree->root, node, cmp);
}

BSNode *erase_BS(BSTree *tree, BSNode *node, int (*cmp)(BSNode *, BSNode *)) {
  if (tree == NULL || node == NULL || cmp == NULL)
    return NULL;

  BSNode *del = NULL;
  tree->root = erase_rec(tree->root, node, cmp, &del);
  return del;
}
BSNode *_find_BS(BSTree *tree, BSNode *node, int (*cmp)(BSNode *, BSNode *)) {
  if (!tree || !node)
    return NULL;
  BSNode *cur = tree->root;
  while (cur) {
    int tmp = cmp(cur, node);
    if (tmp == 0)
      return cur;
    cur = (tmp > 0) ? cur->left : cur->right;
  }
  return NULL;
}
