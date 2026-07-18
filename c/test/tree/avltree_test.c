#include "acutest.h"
#include "dsdef.h"
#include "tree/AVLTree.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct TestNode {
  int key;
  char data[32];
  AVLNode node;
} TestNode;

static int cmp_cb(AVLNode *tree_node, AVLNode *target_node) {
  TestNode *tree = container_of(tree_node, TestNode, node);
  TestNode *target = container_of(target_node, TestNode, node);
  return tree->key - target->key;
}

static TestNode *make_node(int key, const char *data) {
  TestNode *n = malloc(sizeof(TestNode));
  n->key = key;
  strncpy(n->data, data, sizeof(n->data) - 1);
  n->data[sizeof(n->data) - 1] = '\0';
  n->node.left = NULL;
  n->node.right = NULL;
  n->node.height = 1;
  return n;
}

static void free_cb(AVLNode *node, void *args) {
  (void)args;
  TestNode *tn = container_of(node, TestNode, node);
  free(tn);
}

static void test_create_destroy(void) {
  AVLTree *tree = newAVLTree();
  TEST_CHECK(tree != NULL);
  TEST_CHECK(tree->root == NULL);

  TestNode *n1 = make_node(5, "five");
  TestNode *n2 = make_node(3, "three");
  TestNode *n3 = make_node(7, "seven");
  insert_AVL(tree, &n1->node, cmp_cb);
  insert_AVL(tree, &n2->node, cmp_cb);
  insert_AVL(tree, &n3->node, cmp_cb);
  TEST_CHECK(tree->root != NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  destroy_AVL(tree, free_cb, NULL);
}

static void test_insert_find(void) {
  AVLTree *tree = newAVLTree();
  TestNode *n1 = make_node(5, "five");
  TestNode *n2 = make_node(3, "three");
  TestNode *n3 = make_node(7, "seven");
  TestNode *n4 = make_node(1, "one");
  TestNode *n5 = make_node(4, "four");

  insert_AVL(tree, &n1->node, cmp_cb);
  insert_AVL(tree, &n2->node, cmp_cb);
  insert_AVL(tree, &n3->node, cmp_cb);
  insert_AVL(tree, &n4->node, cmp_cb);
  insert_AVL(tree, &n5->node, cmp_cb);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  TestNode ref;
  ref.key = 5;
  AVLNode *found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  TestNode *found_node = container_of(found, TestNode, node);
  TEST_CHECK(found_node->key == 5);
  TEST_CHECK(strcmp(found_node->data, "five") == 0);

  ref.key = 1;
  found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  found_node = container_of(found, TestNode, node);
  TEST_CHECK(found_node->key == 1);

  ref.key = 10;
  found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);

  AVLTree *empty = newAVLTree();
  ref.key = 5;
  found = find_AVL(empty, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);
  destroy_AVL(empty, free_cb, NULL);

  destroy_AVL(tree, free_cb, NULL);
}

static void test_insert_duplicate(void) {
  AVLTree *tree = newAVLTree();
  TestNode *n1 = make_node(5, "first");
  TestNode *n2 = make_node(5, "second");

  insert_AVL(tree, &n1->node, cmp_cb);
  insert_AVL(tree, &n2->node, cmp_cb);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  TestNode ref;
  ref.key = 5;
  AVLNode *found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  TestNode *fn = container_of(found, TestNode, node);
  TEST_CHECK(fn->key == 5);
  TEST_CHECK(strcmp(fn->data, "first") == 0);

  AVLNode *deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);

  found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  destroy_AVL(tree, free_cb, NULL);
  free(n2);
}

static void test_delete_leaf(void) {
  AVLTree *tree = newAVLTree();
  TestNode *n1 = make_node(5, "root");
  TestNode *n2 = make_node(3, "left");
  TestNode *n3 = make_node(7, "right");
  insert_AVL(tree, &n1->node, cmp_cb);
  insert_AVL(tree, &n2->node, cmp_cb);
  insert_AVL(tree, &n3->node, cmp_cb);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  TestNode ref;
  ref.key = 3;
  AVLNode *deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n2->node);
  free_cb(deleted, NULL);

  AVLNode *found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  ref.key = 7;
  deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n3->node);
  free_cb(deleted, NULL);

  ref.key = 5;
  found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  destroy_AVL(tree, free_cb, NULL);
}

static void test_delete_one_child(void) {
  AVLTree *tree = newAVLTree();
  TestNode *n1 = make_node(5, "root");
  TestNode *n2 = make_node(3, "left");
  TestNode *n3 = make_node(1, "left-left");
  insert_AVL(tree, &n1->node, cmp_cb);
  insert_AVL(tree, &n2->node, cmp_cb);
  insert_AVL(tree, &n3->node, cmp_cb);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  TestNode ref;
  ref.key = 3;
  AVLNode *deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n2->node);
  free_cb(deleted, NULL);

  ref.key = 1;
  AVLNode *found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == &n3->node);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  ref.key = 5;
  deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);

  ref.key = 1;
  found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  destroy_AVL(tree, free_cb, NULL);
}

static void test_delete_two_children(void) {
  AVLTree *tree = newAVLTree();
  TestNode *n5 = make_node(5, "5");
  TestNode *n3 = make_node(3, "3");
  TestNode *n7 = make_node(7, "7");
  TestNode *n1 = make_node(1, "1");
  TestNode *n4 = make_node(4, "4");
  TestNode *n6 = make_node(6, "6");
  TestNode *n8 = make_node(8, "8");
  insert_AVL(tree, &n5->node, cmp_cb);
  insert_AVL(tree, &n3->node, cmp_cb);
  insert_AVL(tree, &n7->node, cmp_cb);
  insert_AVL(tree, &n1->node, cmp_cb);
  insert_AVL(tree, &n4->node, cmp_cb);
  insert_AVL(tree, &n6->node, cmp_cb);
  insert_AVL(tree, &n8->node, cmp_cb);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  TestNode ref;
  ref.key = 5;
  AVLNode *deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n5->node);
  free_cb(deleted, NULL);

  ref.key = 6;
  AVLNode *found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == &n6->node);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  ref.key = 7;
  deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n7->node);
  free_cb(deleted, NULL);

  ref.key = 8;
  found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == &n8->node);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  destroy_AVL(tree, free_cb, NULL);
}

static void test_delete_root(void) {
  AVLTree *tree = newAVLTree();
  TestNode *n1 = make_node(5, "root");
  insert_AVL(tree, &n1->node, cmp_cb);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  TestNode ref;
  ref.key = 5;
  AVLNode *deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  TestNode *n2 = make_node(3, "right");
  insert_AVL(tree, &n2->node, cmp_cb);
  ref.key = 3;
  deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n2->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  TestNode *n3 = make_node(7, "left");
  insert_AVL(tree, &n3->node, cmp_cb);
  ref.key = 7;
  deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n3->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  destroy_AVL(tree, free_cb, NULL);
}

static void test_delete_not_found(void) {
  AVLTree *tree = newAVLTree();
  TestNode *n1 = make_node(5, "five");
  insert_AVL(tree, &n1->node, cmp_cb);

  TestNode ref;
  ref.key = 10;
  AVLNode *deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == NULL);

  ref.key = 5;
  AVLNode *found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  destroy_AVL(tree, free_cb, NULL);
}

static void test_erase_return_free(void) {
  AVLTree *tree = newAVLTree();
  TestNode *n1 = make_node(5, "five");
  insert_AVL(tree, &n1->node, cmp_cb);

  TestNode ref;
  ref.key = 5;
  AVLNode *deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  destroy_AVL(tree, free_cb, NULL);
}

static void test_errors(void) {
  AVLTree *tree = newAVLTree();
  TestNode ref;
  ref.key = 1;

  insert_AVL(NULL, NULL, NULL);
  find_AVL(NULL, NULL, NULL);
  erase_AVL(NULL, NULL, NULL);
  destroy_AVL(NULL, NULL, NULL);

  insert_AVL(tree, NULL, cmp_cb);
  TEST_CHECK(tree->root == NULL);

  AVLNode *found = find_AVL(tree, NULL, cmp_cb);
  TEST_CHECK(found == NULL);

  AVLNode *deleted = erase_AVL(tree, NULL, cmp_cb);
  TEST_CHECK(deleted == NULL);

  found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);

  destroy_AVL(tree, free_cb, NULL);
}

static void test_edge_cases(void) {
  AVLTree *tree = newAVLTree();

  TestNode *n1 = make_node(0, "zero");
  TestNode *n2 = make_node(-100, "neg");
  TestNode *n3 = make_node(100, "pos");
  insert_AVL(tree, &n1->node, cmp_cb);
  insert_AVL(tree, &n2->node, cmp_cb);
  insert_AVL(tree, &n3->node, cmp_cb);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  TestNode ref;
  ref.key = -100;
  AVLNode *found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);

  ref.key = 0;
  found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);

  ref.key = 100;
  found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found != NULL);

  AVLNode *deleted = erase_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(deleted == &n3->node);
  free_cb(deleted, NULL);

  ref.key = 100;
  found = find_AVL(tree, &ref.node, cmp_cb);
  TEST_CHECK(found == NULL);
  TEST_CHECK(is_balanced_AVL(tree) == 1);

  destroy_AVL(tree, free_cb, NULL);
}

static void test_random_operations(void) {
  srand(42);
  AVLTree *tree = newAVLTree();
  int inserted[200] = {0};

  for (int i = 0; i < 500; i++) {
    int op = rand() % 3;
    int key = rand() % 200;
    TestNode *n = make_node(key, "rand");
    if (op == 0) {
      insert_AVL(tree, &n->node, cmp_cb);
      if (!inserted[key]) {
        inserted[key] = 1;
      } else {
        free(n);
      }
    } else if (op == 1) {
      TestNode ref;
      ref.key = key;
      AVLNode *found = find_AVL(tree, &ref.node, cmp_cb);
      if (inserted[key]) {
        TEST_CHECK(found != NULL);
      } else {
        TEST_CHECK(found == NULL);
      }
      free(n);
    } else {
      TestNode ref;
      ref.key = key;
      AVLNode *deleted = erase_AVL(tree, &ref.node, cmp_cb);
      if (inserted[key]) {
        TEST_CHECK(deleted != NULL);
        free_cb(deleted, NULL);
        inserted[key] = 0;
      } else {
        TEST_CHECK(deleted == NULL);
      }
      free(n);
    }
    TEST_CHECK(is_balanced_AVL(tree) == 1);
  }

  destroy_AVL(tree, free_cb, NULL);
}

static void test_stress(void) {
  srand((unsigned)time(NULL));
  const int N = 10000;
  AVLTree *tree = newAVLTree();
  TestNode **nodes = malloc(N * sizeof(TestNode *));
  int *keys = malloc(N * sizeof(int));

  for (int i = 0; i < N; i++) {
    int key = rand() % (N * 10);
    keys[i] = key;
    TestNode *n = make_node(key, "stress");
    nodes[i] = n;
    insert_AVL(tree, &n->node, cmp_cb);
    if (i % 100 == 0)
      TEST_CHECK(is_balanced_AVL(tree) == 1);
  }

  for (int i = 0; i < N / 2; i++) {
    int idx = rand() % N;
    TestNode ref;
    ref.key = keys[idx];
    AVLNode *found = find_AVL(tree, &ref.node, cmp_cb);
    TEST_CHECK(found != NULL);
  }

  for (int i = 0; i < N / 2; i++) {
    int idx = rand() % N;
    if (nodes[idx] == NULL)
      continue;
    TestNode ref;
    ref.key = nodes[idx]->key;
    AVLNode *deleted = erase_AVL(tree, &ref.node, cmp_cb);
    if (deleted) {
      free_cb(deleted, NULL);
      nodes[idx] = NULL;
    }
    if (i % 100 == 0)
      TEST_CHECK(is_balanced_AVL(tree) == 1);
  }

  TEST_CHECK(is_balanced_AVL(tree) == 1);
  destroy_AVL(tree, free_cb, NULL);
  free(nodes);
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
             {"errors", test_errors},
             {"edge_cases", test_edge_cases},
             {"random_operations", test_random_operations},
             {"stress", test_stress},
             {NULL, NULL}};
