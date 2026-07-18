#include "acutest.h"
#include "dsdef.h"
#include "tree/rbtree.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct TestNode {
  int key;
  char data[32];
  RBNode node;
} TestNode;

static int cmp_cb(RBNode *tree_node, RBNode *target_node) {
  TestNode *tree = container_of(tree_node, TestNode, node);
  TestNode *target = container_of(target_node, TestNode, node);
  return tree->key - target->key;
}

static TestNode *make_node(int key, const char *data) {
  TestNode *n = malloc(sizeof(TestNode));
  n->key = key;
  strncpy(n->data, data, sizeof(n->data) - 1);
  n->data[sizeof(n->data) - 1] = '\0';
  n->node.parent = NULL;
  n->node.left = NULL;
  n->node.right = NULL;
  n->node.color = RB_RED;
  return n;
}

static void free_cb(RBNode *node, void *args) {
  (void)args;
  TestNode *tn = container_of(node, TestNode, node);
  free(tn);
}

static int rb_check_rec(RBNode *node, int *black_height) {
  if (node == NULL) {
    *black_height = 1;
    return 1;
  }
  if (node->color == RB_RED) {
    if ((node->left && node->left->color == RB_RED) ||
        (node->right && node->right->color == RB_RED)) {
      return 0;
    }
  }
  int left_bh, right_bh;
  if (!rb_check_rec(node->left, &left_bh))
    return 0;
  if (!rb_check_rec(node->right, &right_bh))
    return 0;
  if (left_bh != right_bh)
    return 0;
  *black_height = left_bh + (node->color == RB_BLACK ? 1 : 0);
  return 1;
}

static int is_valid_RB(RBTree *tree) {
  if (tree == NULL || tree->root == NULL)
    return 1;
  if (tree->root->color == RB_RED)
    return 0;
  int bh;
  return rb_check_rec(tree->root, &bh);
}

static void test_create_destroy(void) {
  RBTree *tree = newRBTree();
  TEST_CHECK(tree != NULL);
  TEST_CHECK(tree->root == NULL);

  TestNode *n1 = make_node(5, "five");
  TestNode *n2 = make_node(3, "three");
  TestNode *n3 = make_node(7, "seven");
  insert_RB(tree, &n1->node, cmp_cb);
  insert_RB(tree, &n2->node, cmp_cb);
  insert_RB(tree, &n3->node, cmp_cb);
  TEST_CHECK(tree->root != NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  destroy_RB(tree, free_cb, NULL);
}

static void test_insert_find(void) {
  RBTree *tree = newRBTree();
  TestNode *n1 = make_node(5, "five");
  TestNode *n2 = make_node(3, "three");
  TestNode *n3 = make_node(7, "seven");
  TestNode *n4 = make_node(1, "one");
  TestNode *n5 = make_node(4, "four");

  insert_RB(tree, &n1->node, cmp_cb);
  insert_RB(tree, &n2->node, cmp_cb);
  insert_RB(tree, &n3->node, cmp_cb);
  insert_RB(tree, &n4->node, cmp_cb);
  insert_RB(tree, &n5->node, cmp_cb);
  TEST_CHECK(is_valid_RB(tree) == 1);

  TestNode ref;
  ref.key = 5;
  RBNode *found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  TestNode *found_node = container_of(found, TestNode, node);
  TEST_CHECK(found_node->key == 5);
  TEST_CHECK(strcmp(found_node->data, "five") == 0);

  ref.key = 1;
  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  found_node = container_of(found, TestNode, node);
  TEST_CHECK(found_node->key == 1);

  ref.key = 10;
  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);

  RBTree *empty = newRBTree();
  ref.key = 5;
  found = find_RB(empty, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);
  destroy_RB(empty, free_cb, NULL);

  destroy_RB(tree, free_cb, NULL);
}

static void test_insert_duplicate(void) {
  RBTree *tree = newRBTree();
  TestNode *n1 = make_node(5, "first");
  TestNode *n2 = make_node(5, "second");

  insert_RB(tree, &n1->node, cmp_cb);
  insert_RB(tree, &n2->node, cmp_cb);
  TEST_CHECK(is_valid_RB(tree) == 1);

  TestNode ref;
  ref.key = 5;
  RBNode *found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  TestNode *fn = container_of(found, TestNode, node);
  TEST_CHECK(fn->key == 5);

  RBNode *deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted != NULL);
  free_cb(deleted, NULL);

  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);

  deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted != NULL);
  free_cb(deleted, NULL);

  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  destroy_RB(tree, free_cb, NULL);
}

static void test_delete_leaf(void) {
  RBTree *tree = newRBTree();
  TestNode *n1 = make_node(5, "root");
  TestNode *n2 = make_node(3, "left");
  TestNode *n3 = make_node(7, "right");
  insert_RB(tree, &n1->node, cmp_cb);
  insert_RB(tree, &n2->node, cmp_cb);
  insert_RB(tree, &n3->node, cmp_cb);
  TEST_CHECK(is_valid_RB(tree) == 1);

  TestNode ref;
  ref.key = 3;
  RBNode *deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n2->node);
  free_cb(deleted, NULL);

  RBNode *found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  ref.key = 7;
  deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n3->node);
  free_cb(deleted, NULL);

  ref.key = 5;
  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  destroy_RB(tree, free_cb, NULL);
}

static void test_delete_one_child(void) {
  RBTree *tree = newRBTree();
  TestNode *n1 = make_node(5, "root");
  TestNode *n2 = make_node(3, "left");
  TestNode *n3 = make_node(1, "left-left");
  insert_RB(tree, &n1->node, cmp_cb);
  insert_RB(tree, &n2->node, cmp_cb);
  insert_RB(tree, &n3->node, cmp_cb);
  TEST_CHECK(is_valid_RB(tree) == 1);

  TestNode ref;
  ref.key = 3;
  RBNode *deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n2->node);
  free_cb(deleted, NULL);

  ref.key = 1;
  RBNode *found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == &n3->node);
  TEST_CHECK(is_valid_RB(tree) == 1);

  ref.key = 5;
  deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);

  ref.key = 1;
  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  destroy_RB(tree, free_cb, NULL);
}

static void test_delete_two_children(void) {
  RBTree *tree = newRBTree();
  TestNode *n5 = make_node(5, "5");
  TestNode *n3 = make_node(3, "3");
  TestNode *n7 = make_node(7, "7");
  TestNode *n1 = make_node(1, "1");
  TestNode *n4 = make_node(4, "4");
  TestNode *n6 = make_node(6, "6");
  TestNode *n8 = make_node(8, "8");
  insert_RB(tree, &n5->node, cmp_cb);
  insert_RB(tree, &n3->node, cmp_cb);
  insert_RB(tree, &n7->node, cmp_cb);
  insert_RB(tree, &n1->node, cmp_cb);
  insert_RB(tree, &n4->node, cmp_cb);
  insert_RB(tree, &n6->node, cmp_cb);
  insert_RB(tree, &n8->node, cmp_cb);
  TEST_CHECK(is_valid_RB(tree) == 1);

  TestNode ref;
  ref.key = 5;
  RBNode *deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n5->node);
  free_cb(deleted, NULL);

  ref.key = 6;
  RBNode *found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == &n6->node);
  TEST_CHECK(is_valid_RB(tree) == 1);

  ref.key = 7;
  deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n7->node);
  free_cb(deleted, NULL);

  ref.key = 8;
  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == &n8->node);
  TEST_CHECK(is_valid_RB(tree) == 1);

  destroy_RB(tree, free_cb, NULL);
}

static void test_delete_root(void) {
  RBTree *tree = newRBTree();
  TestNode *n1 = make_node(5, "root");
  insert_RB(tree, &n1->node, cmp_cb);
  TEST_CHECK(is_valid_RB(tree) == 1);

  TestNode ref;
  ref.key = 5;
  RBNode *deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  TestNode *n2 = make_node(3, "right");
  insert_RB(tree, &n2->node, cmp_cb);
  ref.key = 3;
  deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n2->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  TestNode *n3 = make_node(7, "left");
  insert_RB(tree, &n3->node, cmp_cb);
  ref.key = 7;
  deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n3->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  destroy_RB(tree, free_cb, NULL);
}

static void test_delete_not_found(void) {
  RBTree *tree = newRBTree();
  TestNode *n1 = make_node(5, "five");
  insert_RB(tree, &n1->node, cmp_cb);

  TestNode ref;
  ref.key = 10;
  RBNode *deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == NULL);

  ref.key = 5;
  RBNode *found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  destroy_RB(tree, free_cb, NULL);
}

static void test_erase_return_free(void) {
  RBTree *tree = newRBTree();
  TestNode *n1 = make_node(5, "five");
  insert_RB(tree, &n1->node, cmp_cb);

  TestNode ref;
  ref.key = 5;
  RBNode *deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  destroy_RB(tree, free_cb, NULL);
}

static void test_iterators(void) {
  RBTree *tree = newRBTree();
  TestNode *nodes[10];
  for (int i = 0; i < 10; i++) {
    nodes[i] = make_node(i * 10, "iter");
    insert_RB(tree, &nodes[i]->node, cmp_cb);
  }

  RBNode *cur = find_min_RB(tree);
  TEST_CHECK(cur != NULL);
  int expected = 0;
  while (cur != NULL) {
    TestNode *tn = container_of(cur, TestNode, node);
    TEST_CHECK(tn->key == expected);
    expected += 10;
    cur = next_RB(cur);
  }
  TEST_CHECK(expected == 100);

  cur = find_max_RB(tree);
  TEST_CHECK(cur != NULL);
  expected = 90;
  while (cur != NULL) {
    TestNode *tn = container_of(cur, TestNode, node);
    TEST_CHECK(tn->key == expected);
    expected -= 10;
    cur = prev_RB(cur);
  }
  TEST_CHECK(expected == -10);

  destroy_RB(tree, free_cb, NULL);
}

static void test_errors(void) {
  RBTree *tree = newRBTree();
  TestNode ref;
  ref.key = 1;

  insert_RB(NULL, NULL, NULL);
  find_RB(NULL, NULL, NULL);
  erase_RB(NULL, NULL, NULL);
  destroy_RB(NULL, NULL, NULL);

  insert_RB(tree, NULL, cmp_cb);
  TEST_CHECK(tree->root == NULL);

  RBNode *found = find_RB(tree, NULL, cmp_cb);
  TEST_CHECK(found == NULL);

  RBNode *deleted = erase_RB(tree, NULL, cmp_cb);
  TEST_CHECK(deleted == NULL);

  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);

  destroy_RB(tree, free_cb, NULL);
}

static void test_edge_cases(void) {
  RBTree *tree = newRBTree();

  TestNode *n1 = make_node(0, "zero");
  TestNode *n2 = make_node(-100, "neg");
  TestNode *n3 = make_node(100, "pos");
  insert_RB(tree, &n1->node, cmp_cb);
  insert_RB(tree, &n2->node, cmp_cb);
  insert_RB(tree, &n3->node, cmp_cb);
  TEST_CHECK(is_valid_RB(tree) == 1);

  TestNode ref;
  ref.key = -100;
  RBNode *found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);

  ref.key = 0;
  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);

  ref.key = 100;
  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);

  RBNode *deleted = erase_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n3->node);
  free_cb(deleted, NULL);

  ref.key = 100;
  found = find_RB(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);
  TEST_CHECK(is_valid_RB(tree) == 1);

  destroy_RB(tree, free_cb, NULL);
}

static void test_random_operations(void) {
  srand(42);
  RBTree *tree = newRBTree();
  int inserted[200] = {0};

  for (int i = 0; i < 500; i++) {
    int op = rand() % 3;
    int key = rand() % 200;

    if (op == 0) {
      TestNode ref;
      ref.key = key;
      if (find_RB(tree, &ref.node, cmp_cb) == NULL) {
        TestNode *n = make_node(key, "rand");
        insert_RB(tree, &n->node, cmp_cb);
        inserted[key] = 1;
      }
    } else if (op == 1) {
      TestNode ref;
      ref.key = key;
      RBNode *found = find_RB(tree, &ref.node, cmp_cb);
      if (inserted[key])
        TEST_CHECK(found != NULL);
      else
        TEST_CHECK(found == NULL);
    } else {
      TestNode ref;
      ref.key = key;
      RBNode *deleted = erase_RB(tree, &ref.node, cmp_cb);
      if (inserted[key]) {
        TEST_CHECK(deleted != NULL);
        free_cb(deleted, NULL);
        inserted[key] = 0;
      } else {
        TEST_CHECK(deleted == NULL);
      }
    }
    TEST_CHECK(is_valid_RB(tree) == 1);
  }

  destroy_RB(tree, free_cb, NULL);
}

static void test_stress(void) {
  srand((unsigned)time(NULL));
  const int N = 10000;
  RBTree *tree = newRBTree();

  int *keys = malloc(N * sizeof(int));

  for (int i = 0; i < N; i++) {
    int key = rand() % (N * 10);
    keys[i] = key;
    TestNode *n = make_node(key, "stress");
    insert_RB(tree, &n->node, cmp_cb);
    if (i % 100 == 0)
      TEST_CHECK(is_valid_RB(tree) == 1);
  }

  for (int i = 0; i < N / 2; i++) {
    int idx = rand() % N;
    TestNode ref;
    ref.key = keys[idx];
    RBNode *found = find_RB(tree, &ref.node, cmp_cb);
    TEST_CHECK(found != NULL);
  }

  for (int i = 0; i < N / 2; i++) {
    int idx = rand() % N;
    TestNode ref;
    ref.key = keys[idx];
    RBNode *deleted = erase_RB(tree, &ref.node, cmp_cb);
    if (deleted) {
      free_cb(deleted, NULL);
    }
    if (i % 100 == 0)
      TEST_CHECK(is_valid_RB(tree) == 1);
  }

  TEST_CHECK(is_valid_RB(tree) == 1);
  destroy_RB(tree, free_cb, NULL);
  free(keys);
}

TEST_LIST = {{"create_destroy", test_create_destroy},
             {"insert_find", test_insert_find},
             {"insert_duplicate", test_insert_duplicate},
             {"delete_leaf", test_delete_leaf},
             {"delete_one_child", test_delete_one_child},
             {"delete_two_children", test_delete_two_children},
             {"delete_root", test_delete_root},
             {"delete_not_found", test_delete_not_found},
             {"erase_return_free", test_erase_return_free},
             {"iterators", test_iterators},
             {"errors", test_errors},
             {"edge_cases", test_edge_cases},
             {"random_operations", test_random_operations},
             {"stress", test_stress},
             {NULL, NULL}};
