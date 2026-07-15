#include "tree/BSTree.h"
#include <stdlib.h>

static void pre_rec(BSNode *node) {
  if (node == NULL)
    return;
  // todo
  pre_rec(node->left);
  pre_rec(node->right);
}

static void in_rec(BSNode *node) {
  if (node == NULL)
    return;
  in_rec(node->left);
  // todo;
  in_rec(node->right);
}

static void post_rec(BSNode *node) {
  if (node == NULL)
    return;
  post_rec(node->left);
  post_rec(node->right);
  // todo;
}

static void destroy_rec(BSNode *cur_node, void (*free_cb)(BSNode *, void *),
                        void *args) {
  if (cur_node == NULL)
    return;
  destroy_rec(cur_node->left, free_cb, args);
  destroy_rec(cur_node->right, free_cb, args);
  if (free_cb)
    free_cb(cur_node, args);
}

static BSNode *insert_rec(BSNode *root, BSNode *new_node,
                          int (*cmp)(BSNode *, BSNode *)) {
  if (root == NULL)
    return new_node;
  int tmp = cmp(new_node, root);
  if (tmp < 0)
    root->left = insert_rec(root->left, new_node, cmp);
  else if (tmp > 0)
    root->right = insert_rec(root->right, new_node, cmp);

  return root;
}

static BSNode *find_rec(BSNode *root, BSNode *node,
                        int (*cmp)(BSNode *, BSNode *)) {
  if (root == NULL)
    return NULL;

  int tmp = cmp(root, node);
  if (tmp == 0)
    return root;
  if (tmp > 0)
    return find_rec(root->left, node, cmp);

  return find_rec(root->right, node, cmp);
}

static BSNode *find_min(BSNode *node) {
  if (node == NULL)
    return NULL;

  while (node->left) {
    node = node->left;
  }

  return node;
}

static BSNode *find_max(BSNode *node) {
  if (node == NULL)
    return NULL;
  while (node->right) {
    node = node->right;
  }
  return node;
}

static BSNode *erase_rec(BSNode *root, BSNode *node,
                         int (*cmp)(BSNode *, BSNode *), BSNode **out_deleted) {
  if (root == NULL)
    return NULL;

  int tmp = cmp(root, node);

  if (tmp > 0) {
    root->left = erase_rec(root->left, node, cmp, out_deleted);
  } else if (tmp < 0) {
    root->right = erase_rec(root->right, node, cmp, out_deleted);
  } else {
    *out_deleted = root;

    if (!root->left) {
      BSNode *r = root->right;
      root->right = NULL;
      return r;
    }
    if (!root->right) {
      BSNode *l = root->left;
      root->left = NULL;
      return l;
    }

    BSNode *parent = root;
    BSNode *min = root->right;
    while (min->left) {
      parent = min;
      min = min->left;
    }

    if (parent == root) {
      root->right = min->right;
    } else {
      parent->left = min->right;
    }

    min->left = root->left;
    min->right = root->right;
    root->left = root->right = NULL;
    return min;
  }

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

void insert_BS(BSTree *tree, BSNode *new_node,
               int (*cmp)(BSNode *new_node, BSNode *tree_node)) {
  if (tree == NULL || new_node == NULL)
    return;

  new_node->left = NULL;
  new_node->right = NULL;
  tree->root = insert_rec(tree->root, new_node, cmp);
}

BSNode *find_BS(BSTree *tree, BSNode *node,
                int (*cmp)(BSNode *tree_node, BSNode *node)) {
  if (tree == NULL || node == NULL)
    return NULL;
  return find_rec(tree->root, node, cmp);
}

BSNode *erase_BS(BSTree *tree, BSNode *node,
                 int (*cmp)(BSNode *tree_node, BSNode *node)) {
  if (tree == NULL || node == NULL)
    return NULL;

  BSNode *deleted = NULL;
  tree->root = erase_rec(tree->root, node, cmp, &deleted);
  return deleted;
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
