#ifndef _SQLIST_H
#define _SQLIST_H

#define TRUE_                        1
#define FALSE_                       0
#define OK_                          1
#define ERROR_                       0
#define INFEASIBLE_                 -1
#define OVERFLOW_                   -2
#define NULL_                        0

typedef int ElemType;
typedef int Status;
typedef unsigned char u8;
typedef Status (*Visit)(ElemType);

typedef struct{
    ElemType* base;
    int capacity;
    int size;
}SqList;

Status InitList_Sq(SqList* L, int capacity);
Status DestroyList_Sq(SqList* L);
Status ListClear_Sq(SqList* L);
Status ListInsert_Sq(SqList* L, int position, ElemType e, u8 flag);
Status ListBulkInsert_Sq(SqList* L, int position, int* arr, int arr_size, u8 flag);
Status ListErase_Sq(SqList* L, int position, ElemType* e, u8 flag);
int ListFind_Sq(SqList L, ElemType e, u8 flag);
Status PrintElem_(ElemType e);
Status ListTraverse_Sq(SqList L, Status(*visit)(ElemType));

#endif

