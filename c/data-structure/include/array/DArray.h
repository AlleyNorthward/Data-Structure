#ifndef DARRAY_H
#define DARRAY_H

#include <stdbool.h>

#define SET_DA(arr, pos, val, type) set_DA(arr, pos, &(type){val})
#define GET_DA(arr, pos, type) (*(type *)at_DA(arr, pos))

typedef struct DArray {
  void *data;
  int len;
  int capacity;
  int elem_size;
} DArray;

DArray *newDArray(int capacity, int elem_size);
bool empty_DA(DArray *arr);
void clear_DA(DArray *arr, void (*free_cb)(void *elem, void *args), void *args);
void destroy_DA(DArray *arr, void (*free_cb)(void *elem, void *args),
                void *args);

int size_DA(DArray *arr);
int capacity_DA(DArray *arr);
void print_DA(DArray *arr, void (*print_cb)(void *elem));
void resize_DA(DArray *arr, int new_len);

void push_back_DA(DArray *arr, void *elem);
void *pop_back_DA(DArray *arr);
void set_DA(DArray *arr, int pos, void *elem);
void *at_DA(DArray *arr, int pos);

void move_right_DA(DArray *arr, int pos, int n);
void move_left_DA(DArray *arr, int pos, int n);
void reverse_DA(DArray *arr, int left_pos, int right_pos);

#endif
