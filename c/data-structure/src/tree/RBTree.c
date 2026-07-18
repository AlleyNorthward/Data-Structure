#include "tree/RBTree.h"
#include <stdlib.h>

static inline int is_red(RBNode *node) {
  return node != NULL && node->color == RB_RED;
}

static inline int is_black(RBNode *node) {
  return node == NULL || node->color == RB_BLACK;
}

static void rotate_left(RBTree *tree, RBNode *x) {
  RBNode *y = x->right;
  x->right = y->left;
  if (y->left != NULL)
    y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == NULL)
    tree->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

static void rotate_right(RBTree *tree, RBNode *y) {
  RBNode *x = y->left;
  y->left = x->right;
  if (x->right != NULL)
    x->right->parent = y;
  x->parent = y->parent;
  if (y->parent == NULL)
    tree->root = x;
  else if (y == y->parent->left)
    y->parent->left = x;
  else
    y->parent->right = x;
  x->right = y;
  y->parent = x;
}

static void rb_insert_fixup(RBTree *tree, RBNode *node) {
  while (node->parent != NULL && node->parent->color == RB_RED) {
    RBNode *grandpa = node->parent->parent;
    if (node->parent == grandpa->left) {
      RBNode *uncle = grandpa->right;
      if (is_red(uncle)) {
        node->parent->color = RB_BLACK;
        uncle->color = RB_BLACK;
        grandpa->color = RB_RED;
        node = grandpa;
      } else {
        if (node == node->parent->right) {
          node = node->parent;
          rotate_left(tree, node);
        }
        node->parent->color = RB_BLACK;
        grandpa->color = RB_RED;
        rotate_right(tree, grandpa);
      }
    } else {
      RBNode *uncle = grandpa->left;
      if (is_red(uncle)) {
        node->parent->color = RB_BLACK;
        uncle->color = RB_BLACK;
        grandpa->color = RB_RED;
        node = grandpa;
      } else {
        if (node == node->parent->left) {
          node = node->parent;
          rotate_right(tree, node);
        }
        node->parent->color = RB_BLACK;
        grandpa->color = RB_RED;
        rotate_left(tree, grandpa);
      }
    }
  }
  tree->root->color = RB_BLACK;
}

void insert_RB(RBTree *tree, RBNode *node, int (*cmp)(RBNode *, RBNode *)) {
  if (!tree || !node || !cmp)
    return;

  RBNode *y = NULL;
  RBNode *x = tree->root;
  while (x != NULL) {
    y = x;
    if (cmp(x, node) > 0)
      x = x->left;
    else
      x = x->right;
  }
  node->parent = y;
  if (y == NULL)
    tree->root = node;
  else if (cmp(y, node) > 0)
    y->left = node;
  else
    y->right = node;

  node->left = NULL;
  node->right = NULL;
  node->color = RB_RED;

  rb_insert_fixup(tree, node);
}

static void rb_transplant(RBTree *tree, RBNode *u, RBNode *v) {
  if (u->parent == NULL)
    tree->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  if (v != NULL)
    v->parent = u->parent;
}

static void rb_erase_fixup(RBTree *tree, RBNode *x, RBNode *x_parent) {
  while (x != tree->root && is_black(x)) {
    if (x == x_parent->left) {
      RBNode *w = x_parent->right;
      if (is_red(w)) {
        w->color = RB_BLACK;
        x_parent->color = RB_RED;
        rotate_left(tree, x_parent);
        w = x_parent->right;
      }
      if (is_black(w->left) && is_black(w->right)) {
        w->color = RB_RED;
        x = x_parent;
        x_parent = x->parent;
      } else {
        if (is_black(w->right)) {
          if (w->left)
            w->left->color = RB_BLACK;
          w->color = RB_RED;
          rotate_right(tree, w);
          w = x_parent->right;
        }
        w->color = x_parent->color;
        x_parent->color = RB_BLACK;
        if (w->right)
          w->right->color = RB_BLACK;
        rotate_left(tree, x_parent);
        x = tree->root;
      }
    } else {
      RBNode *w = x_parent->left;
      if (is_red(w)) {
        w->color = RB_BLACK;
        x_parent->color = RB_RED;
        rotate_right(tree, x_parent);
        w = x_parent->left;
      }
      if (is_black(w->right) && is_black(w->left)) {
        w->color = RB_RED;
        x = x_parent;
        x_parent = x->parent;
      } else {
        if (is_black(w->left)) {
          if (w->right)
            w->right->color = RB_BLACK;
          w->color = RB_RED;
          rotate_left(tree, w);
          w = x_parent->left;
        }
        w->color = x_parent->color;
        x_parent->color = RB_BLACK;
        if (w->left)
          w->left->color = RB_BLACK;
        rotate_right(tree, x_parent);
        x = tree->root;
      }
    }
  }
  if (x != NULL)
    x->color = RB_BLACK;
}

RBNode *erase_RB(RBTree *tree, RBNode *node, int (*cmp)(RBNode *, RBNode *)) {
  RBNode *z = find_RB(tree, node, cmp);
  if (z == NULL)
    return NULL;

  RBNode *x;
  RBNode *x_parent;
  RBNode *y = z;
  RBColor y_original_color = y->color;

  if (z->left == NULL) {
    x = z->right;
    x_parent = z->parent;
    rb_transplant(tree, z, z->right);
  } else if (z->right == NULL) {
    x = z->left;
    x_parent = z->parent;
    rb_transplant(tree, z, z->left);
  } else {
    y = z->right;
    while (y->left != NULL)
      y = y->left;
    y_original_color = y->color;

    x = y->right;
    if (y->parent == z) {
      x_parent = y;
    } else {
      x_parent = y->parent;
      rb_transplant(tree, y, y->right);
      y->right = z->right;
      if (y->right)
        y->right->parent = y;
    }
    rb_transplant(tree, z, y);
    y->left = z->left;
    if (y->left)
      y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RB_BLACK && tree->root != NULL) {
    rb_erase_fixup(tree, x, x_parent);
  }

  z->parent = z->left = z->right = NULL;
  return z;
}

RBNode *find_RB(RBTree *tree, RBNode *node, int (*cmp)(RBNode *, RBNode *)) {
  RBNode *cur = tree ? tree->root : NULL;
  while (cur != NULL) {
    int tmp = cmp(cur, node);
    if (tmp == 0)
      return cur;
    else if (tmp > 0)
      cur = cur->left;
    else
      cur = cur->right;
  }
  return NULL;
}

RBNode *find_min_RB(RBTree *tree) {
  if (!tree || !tree->root)
    return NULL;
  RBNode *cur = tree->root;
  while (cur->left)
    cur = cur->left;
  return cur;
}

RBNode *find_max_RB(RBTree *tree) {
  if (!tree || !tree->root)
    return NULL;
  RBNode *cur = tree->root;
  while (cur->right)
    cur = cur->right;
  return cur;
}

RBNode *next_RB(RBNode *node) {
  if (node == NULL)
    return NULL;
  if (node->right != NULL) {
    node = node->right;
    while (node->left)
      node = node->left;
    return node;
  }
  RBNode *p = node->parent;
  while (p != NULL && node == p->right) {
    node = p;
    p = p->parent;
  }
  return p;
}

RBNode *prev_RB(RBNode *node) {
  if (node == NULL)
    return NULL;
  if (node->left != NULL) {
    node = node->left;
    while (node->right)
      node = node->right;
    return node;
  }
  RBNode *p = node->parent;
  while (p != NULL && node == p->left) {
    node = p;
    p = p->parent;
  }
  return p;
}

RBTree *newRBTree() {
  RBTree *tree = malloc(sizeof(RBTree));
  if (tree)
    tree->root = NULL;
  return tree;
}

static void destroy_rec(RBNode *root, void (*free_cb)(RBNode *, void *),
                        void *args) {
  if (root == NULL)
    return;
  destroy_rec(root->left, free_cb, args);
  destroy_rec(root->right, free_cb, args);
  if (free_cb)
    free_cb(root, args);
}

void destroy_RB(RBTree *tree, void (*free_cb)(RBNode *, void *), void *args) {
  if (tree == NULL)
    return;
  destroy_rec(tree->root, free_cb, args);
  free(tree);
}
