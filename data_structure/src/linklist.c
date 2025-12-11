#include "linklist.h"
#include <stdlib.h>

Status InitList_L(LinkList* L){
    // header pointer
    *L = (LNode*)malloc(sizeof(LNode));
    if(! *L) exit(OVERFLOW_);

    (*L)->next = NULL_;
    return OK_;
}

// Status InitList_L_no_head_pointer(LinkList* L){
//     *L = NULL_;
//     return OK_;
// }

Status DestroyList_L(LinkList* L){
    // double pointer 
    // book => curNode = *L, nextNode = curNode->next
    LNode*curNode = (*L)->next;
    LNode* prevNode = *L;

    while(curNode){
        free(prevNode);
        prevNode = curNode;
        curNode = curNode->next;
    }
    free(prevNode);
    *L = NULL_;
    return OK_;
}

Status ListInsert_L(LinkList* L, int position, ElemType e, u8 flag){
    if(flag) position = position - 1;
    if(position < 0) return ERROR_;

    int pos = 0;
    LNode* prevNode = *L;

    while(pos < position && prevNode){
        prevNode = prevNode->next;
        pos ++;
    }
    if(!prevNode) return ERROR_;
    else{
        LNode* node = (LNode*)malloc(sizeof(LNode));
        if(!node) exit(OVERFLOW_);
        node->data = e;

        node->next = prevNode->next;
        prevNode->next = node;
    }
    return OK_;
}

Status ListErase_L(LinkList* L, int position, ElemType* e, u8 flag){
    if(flag) position = position - 1;
    if(position < 0) return ERROR_;

    LNode* prevNode = *L;
    LNode* curNode = prevNode->next;
    int pos = 0;

    while(pos < position && curNode){
        prevNode = curNode;
        curNode = curNode->next;
        pos++;
    }
    if(curNode == NULL_ || pos != position) return ERROR_;
    else{
        prevNode->next = curNode->next;
        *e = curNode->data;
        free(curNode);
    }
    return OK_;
}

Status ListGetElem_L(LinkList L, int position, ElemType* e, u8 flag){
    if(flag) position = position - 1;
    if(position < 0) return ERROR_;
    int pos = 0;
    LNode* curNode = L->next;
    while(curNode && pos < position){
        curNode = curNode->next;
        pos++;
    }
    if(curNode == NULL_ || pos != position) return ERROR_;
    *e = curNode->data;
    return OK_;
}

int ListSize_L(LinkList L){
    int size = 0;
    LNode* curNode = L->next;
    while(curNode){
        size++;
        curNode = curNode->next;
    }
    return size;
}












