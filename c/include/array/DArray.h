#ifndef DARRAY_H
#define DARRAY_H

#include <stdbool.h>

typedef struct DArray {
  void *data;
  int len;
  int capacity;
  int elem_size;
} DArray;

DArray *newDArray(int cap, int elem_size);
bool empty_DA(DArray *arr);
void *at_DA(DArray *arr, int pos);
void clear_DA(DArray *arr, void (*free_cb)(void *elem));
void destroy_DA(DArray *arr, void (*free_cb)(void *elem));

int size_DA(DArray *arr);
int capacity_DA(DArray *arr);
void print_DA(DArray *arr, void (*print_cb)(void *elem));

void expand_DA(DArray *arr);
void push_back_DA(DArray *arr, void *elem);
void* pop_back_DA(DArray *arr);

void insert_DA(DArray *arr, int pos, void *elem);
void erase_DA(DArray *arr, int pos, void (*free_cb)(void *elem));

#endif
