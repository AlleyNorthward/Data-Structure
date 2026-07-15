// #ifndef RBTREE_H
// #define RBTREE_H
//
// typedef struct RBNode {
//   struct RBNode *parent;
//   struct RBNode *right;
//   struct RBNode *left;
//   int color;
// } RBNode;
//
// typedef struct RBTree {
//   struct RBNode *root;
// } RBTree;
//
// RBTree *newRBTree();
// void clear_RB();
// void destroy_RB();
// void empty_RB();
//
// void insert_unique_RB();
// void insert_equal_RB();
//
// // linux
// void rb_rotate_left(struct rb_node *node, struct rb_root *root);
// void rb_rotate_right(struct rb_node *node, struct rb_root *root);
//
// void rb_link_node(struct rb_node *node, struct rb_node *parent,
//                   struct rb_node **link);
// void rb_insert_color(struct rb_node *node, struct rb_root *root);
// void rb_erase(struct rb_node *node, struct rb_root *root);
//
// void rb_replace_node(struct rb_node *victim, struct rb_node *new_node,
//                      struct rb_root *root);
//
// #endif
