#include "acutest.h"
#include "dsdef.h"
#include "tree/BSTree.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct TestNode {
  int key;
  char data[32];
  BSNode node;
} TestNode;

static int cmp_insert(BSNode *new_node, BSNode *tree_node) {
  TestNode *new = container_of(new_node, TestNode, node);
  TestNode *old = container_of(tree_node, TestNode, node);
  return new->key - old->key;
}

static int cmp_find(BSNode *tree_node, BSNode *ref_node) {
  TestNode *tree = container_of(tree_node, TestNode, node);
  TestNode *ref = container_of(ref_node, TestNode, node);
  return tree->key - ref->key;
}

static TestNode *make_node(int key, const char *data) {
  TestNode *n = malloc(sizeof(TestNode));
  n->key = key;
  strncpy(n->data, data, sizeof(n->data) - 1);
  n->data[sizeof(n->data) - 1] = '\0';
  n->node.left = NULL;
  n->node.right = NULL;
  return n;
}

static void free_cb(BSNode *node, void *args) {
  (void)args;
  TestNode *tn = container_of(node, TestNode, node);
  free(tn);
}

static void test_create_destroy(void) {
  BSTree *tree = newBSTree();
  TEST_CHECK(tree != NULL);
  TEST_CHECK(tree->root == NULL);

  TestNode *n1 = make_node(5, "five");
  TestNode *n2 = make_node(3, "three");
  TestNode *n3 = make_node(7, "seven");
  insert_BS(tree, &n1->node, cmp_insert);
  insert_BS(tree, &n2->node, cmp_insert);
  insert_BS(tree, &n3->node, cmp_insert);
  TEST_CHECK(tree->root != NULL);

  destroy_BS(tree, free_cb, NULL);
}

static void test_insert_find(void) {
  BSTree *tree = newBSTree();
  TestNode *n1 = make_node(5, "five");
  TestNode *n2 = make_node(3, "three");
  TestNode *n3 = make_node(7, "seven");
  TestNode *n4 = make_node(1, "one");
  TestNode *n5 = make_node(4, "four");

  insert_BS(tree, &n1->node, cmp_insert);
  insert_BS(tree, &n2->node, cmp_insert);
  insert_BS(tree, &n3->node, cmp_insert);
  insert_BS(tree, &n4->node, cmp_insert);
  insert_BS(tree, &n5->node, cmp_insert);

  TestNode ref;
  ref.key = 5;
  BSNode *found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found != NULL);
  TestNode *found_node = container_of(found, TestNode, node);
  TEST_CHECK(found_node->key == 5);
  TEST_CHECK(strcmp(found_node->data, "five") == 0);

  ref.key = 1;
  found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found != NULL);
  found_node = container_of(found, TestNode, node);
  TEST_CHECK(found_node->key == 1);

  ref.key = 10;
  found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found == NULL);

  BSTree *empty = newBSTree();
  ref.key = 5;
  found = find_BS(empty, &ref.node, cmp_find);
  TEST_CHECK(found == NULL);
  destroy_BS(empty, free_cb, NULL);

  destroy_BS(tree, free_cb, NULL);
}

static void test_insert_duplicate(void) {
  BSTree *tree = newBSTree();
  TestNode *n1 = make_node(5, "first");
  TestNode *n2 = make_node(5, "second");

  insert_BS(tree, &n1->node, cmp_insert);
  insert_BS(tree, &n2->node, cmp_insert);

  TestNode ref;
  ref.key = 5;
  BSNode *found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found != NULL);
  TestNode *fn = container_of(found, TestNode, node);
  TEST_CHECK(fn->key == 5);
  TEST_CHECK(strcmp(fn->data, "first") == 0);

  BSNode *deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);

  found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found == NULL);

  destroy_BS(tree, free_cb, NULL);
  free(n2);
}

static void test_delete_leaf(void) {
  BSTree *tree = newBSTree();
  TestNode *n1 = make_node(5, "root");
  TestNode *n2 = make_node(3, "left");
  TestNode *n3 = make_node(7, "right");
  insert_BS(tree, &n1->node, cmp_insert);
  insert_BS(tree, &n2->node, cmp_insert);
  insert_BS(tree, &n3->node, cmp_insert);

  TestNode ref;
  ref.key = 3;
  BSNode *deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n2->node);
  free_cb(deleted, NULL);

  BSNode *found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found == NULL);

  ref.key = 7;
  deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n3->node);
  free_cb(deleted, NULL);

  ref.key = 5;
  found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found != NULL);

  destroy_BS(tree, free_cb, NULL);
}

static void test_delete_one_child(void) {
  BSTree *tree = newBSTree();
  TestNode *n1 = make_node(5, "root");
  TestNode *n2 = make_node(3, "left");
  TestNode *n3 = make_node(1, "left-left");
  insert_BS(tree, &n1->node, cmp_insert);
  insert_BS(tree, &n2->node, cmp_insert);
  insert_BS(tree, &n3->node, cmp_insert);

  TestNode ref;
  ref.key = 3;
  BSNode *deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n2->node);
  free_cb(deleted, NULL);

  ref.key = 1;
  BSNode *found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found == &n3->node);

  ref.key = 5;
  deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);

  ref.key = 1;
  found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found != NULL);

  destroy_BS(tree, free_cb, NULL);
}

static void test_delete_two_children(void) {
  BSTree *tree = newBSTree();
  TestNode *n5 = make_node(5, "5");
  TestNode *n3 = make_node(3, "3");
  TestNode *n7 = make_node(7, "7");
  TestNode *n1 = make_node(1, "1");
  TestNode *n4 = make_node(4, "4");
  TestNode *n6 = make_node(6, "6");
  TestNode *n8 = make_node(8, "8");
  insert_BS(tree, &n5->node, cmp_insert);
  insert_BS(tree, &n3->node, cmp_insert);
  insert_BS(tree, &n7->node, cmp_insert);
  insert_BS(tree, &n1->node, cmp_insert);
  insert_BS(tree, &n4->node, cmp_insert);
  insert_BS(tree, &n6->node, cmp_insert);
  insert_BS(tree, &n8->node, cmp_insert);

  TestNode ref;
  ref.key = 5;
  BSNode *deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n5->node);
  free_cb(deleted, NULL);

  ref.key = 6;
  BSNode *found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found == &n6->node);

  ref.key = 7;
  deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n7->node);
  free_cb(deleted, NULL);

  ref.key = 8;
  found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found == &n8->node);

  destroy_BS(tree, free_cb, NULL);
}

static void test_delete_root(void) {
  BSTree *tree = newBSTree();
  TestNode *n1 = make_node(5, "root");
  insert_BS(tree, &n1->node, cmp_insert);

  TestNode ref;
  ref.key = 5;
  BSNode *deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);

  TestNode *n2 = make_node(3, "right");
  insert_BS(tree, &n2->node, cmp_insert);
  ref.key = 3;
  deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n2->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);

  TestNode *n3 = make_node(7, "left");
  insert_BS(tree, &n3->node, cmp_insert);
  ref.key = 7;
  deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n3->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);

  destroy_BS(tree, free_cb, NULL);
}

static void test_delete_not_found(void) {
  BSTree *tree = newBSTree();
  TestNode *n1 = make_node(5, "five");
  insert_BS(tree, &n1->node, cmp_insert);

  TestNode ref;
  ref.key = 10;
  BSNode *deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == NULL);

  ref.key = 5;
  BSNode *found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found != NULL);

  destroy_BS(tree, free_cb, NULL);
}

static void test_erase_return_free(void) {
  BSTree *tree = newBSTree();
  TestNode *n1 = make_node(5, "five");
  insert_BS(tree, &n1->node, cmp_insert);

  TestNode ref;
  ref.key = 5;
  BSNode *deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n1->node);
  free_cb(deleted, NULL);
  TEST_CHECK(tree->root == NULL);

  destroy_BS(tree, free_cb, NULL);
}

static void test_errors(void) {
  BSTree *tree = newBSTree();
  TestNode ref;
  ref.key = 1;

  insert_BS(NULL, NULL, NULL);
  find_BS(NULL, NULL, NULL);
  erase_BS(NULL, NULL, NULL);
  destroy_BS(NULL, NULL, NULL);

  insert_BS(tree, NULL, cmp_insert);
  TEST_CHECK(tree->root == NULL);

  BSNode *found = find_BS(tree, NULL, cmp_find);
  TEST_CHECK(found == NULL);

  BSNode *deleted = erase_BS(tree, NULL, cmp_find);
  TEST_CHECK(deleted == NULL);

  found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found == NULL);

  destroy_BS(tree, free_cb, NULL);
}

static void test_edge_cases(void) {
  BSTree *tree = newBSTree();

  TestNode *n1 = make_node(0, "zero");
  TestNode *n2 = make_node(-100, "neg");
  TestNode *n3 = make_node(100, "pos");
  insert_BS(tree, &n1->node, cmp_insert);
  insert_BS(tree, &n2->node, cmp_insert);
  insert_BS(tree, &n3->node, cmp_insert);

  TestNode ref;
  ref.key = -100;
  BSNode *found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found != NULL);

  ref.key = 0;
  found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found != NULL);

  ref.key = 100;
  found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found != NULL);

  BSNode *deleted = erase_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(deleted == &n3->node);
  free_cb(deleted, NULL);

  ref.key = 100;
  found = find_BS(tree, &ref.node, cmp_find);
  TEST_CHECK(found == NULL);

  destroy_BS(tree, free_cb, NULL);
}

static void test_random_operations(void) {
  srand(42);
  BSTree *tree = newBSTree();
  int inserted[200] = {0};
  int count = 0;

  for (int i = 0; i < 500; i++) {
    int op = rand() % 3;
    int key = rand() % 200;
    TestNode *n = make_node(key, "rand");
    if (op == 0) {
      insert_BS(tree, &n->node, cmp_insert);
      if (!inserted[key]) {
        inserted[key] = 1;
        count++;
      } else {
        free(n);
      }
    } else if (op == 1) {
      TestNode ref;
      ref.key = key;
      BSNode *found = find_BS(tree, &ref.node, cmp_find);
      if (inserted[key]) {
        TEST_CHECK(found != NULL);
      } else {
        TEST_CHECK(found == NULL);
      }
      free(n);
    } else {
      TestNode ref;
      ref.key = key;
      BSNode *deleted = erase_BS(tree, &ref.node, cmp_find);
      if (inserted[key]) {
        TEST_CHECK(deleted != NULL);
        free_cb(deleted, NULL);
        inserted[key] = 0;
        count--;
      } else {
        TEST_CHECK(deleted == NULL);
      }
      free(n);
    }
  }

  destroy_BS(tree, free_cb, NULL);
}

static void test_stress(void) {
  srand((unsigned)time(NULL));
  const int N = 10000;
  BSTree *tree = newBSTree();
  TestNode **nodes = malloc(N * sizeof(TestNode *));
  int *keys = malloc(N * sizeof(int));

  for (int i = 0; i < N; i++) {
    int key = rand() % (N * 10);
    keys[i] = key;
    TestNode *n = make_node(key, "stress");
    nodes[i] = n;
    insert_BS(tree, &n->node, cmp_insert);
  }

  for (int i = 0; i < N / 2; i++) {
    int idx = rand() % N;
    TestNode ref;
    ref.key = keys[idx];
    BSNode *found = find_BS(tree, &ref.node, cmp_find);
    TEST_CHECK(found != NULL);
  }

  for (int i = 0; i < N / 2; i++) {
    int idx = rand() % N;
    if (nodes[idx] == NULL)
      continue;
    TestNode ref;
    ref.key = nodes[idx]->key;
    BSNode *deleted = erase_BS(tree, &ref.node, cmp_find);
    if (deleted) {
      free_cb(deleted, NULL);
      nodes[idx] = NULL;
    }
  }

  destroy_BS(tree, free_cb, NULL);
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
