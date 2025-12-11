#ifndef _LIST_H
#define _LIST_H

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

typedef struct LNode{
    ElemType data;
    struct LNode* next;
}LNode, *LinkList;
// typedef LNode* LinkList;

/*
    head => L, curNode=> L->next 
*/

Status InitList_L(LinkList* L);
//Status InitList_L_no_header_pointer(LinkList* L);
Status DestroyList_L(LinkList* L);
Status ListInsert_L(LinkList* L, int position, ElemType e, u8 flag);
Status ListErase_L(LinkList* L, int position, ElemType* e, u8 flag);
Status ListGetElem_L(LinkList L, int position, ElemType* e, u8 flag);
int ListSize_L(LinkList L);

#endif
