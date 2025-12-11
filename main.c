#include <stdio.h>
#include <stdlib.h>
#include "linklist.h"
#include "sqlist.h"


int main(){
    LinkList L;
    ElemType* arr;
    u8 i, j, size = 10;
    arr = (ElemType*)malloc(sizeof(ElemType) * size);
    if(!arr) exit(OVERFLOW_);

    for (i = 0, j = 0; i < size; i++, j++){
        arr[i] = j;
    }
    if(InitList_L(&L)) 
        printf("success\n");

    for(i = 0; i < size; i++){
        ListInsert_L(&L, i, arr[i], 0);
    }

    ElemType e;
    ListErase_L(&L, 3, &e, 0);
    ElemType ee;
    ListGetElem_L(L, 8, &ee, 0);
    int length = ListSize_L(L);
    
    printf("%d %d %d", e, ee, length);
    if(DestroyList_L(&L))
        printf("success\n");

    return 0;
}












