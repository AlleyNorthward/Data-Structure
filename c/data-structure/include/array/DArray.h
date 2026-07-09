#ifndef DARRAY_H
#define DARRAY_H

#include <stdbool.h>

#define AT_PTR_DA(arr, pos, type)                                              \
  ((type *)((char *)(arr)->data + (pos) * (arr)->elem_size))

#define GET_DA(arr, pos, type) (*AT_PTR_DA(arr, pos, type))
#define GET_INT_DA(arr, pos) GET_DA(arr, pos, int)
#define PUSH_BACK_INT_DA(arr, val) push_back_DA((arr), &(int){val})
#define POP_BACK_INT_DA(arr) (*(int*)pop_back_DA(arr))
#define INSERT_INT_BY_POS_DA(arr, pos, val) insert_by_pos_DA((arr), pos, &(int){val})

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

void insert_by_pos_DA(DArray *arr, int pos, void *elem);
void erase_by_pos_DA(DArray *arr, int pos, void (*free_cb)(void *elem));

#endif
