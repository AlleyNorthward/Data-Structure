#include <stdio.h>
#include <stdlib.h>
#include "sqlist.h"


int main(){
    SqList L;
    ElemType* arr;
    u8 i, j, size = 10;
    arr = (ElemType*)malloc(sizeof(ElemType) * size);
    if(!arr) exit(OVERFLOW_);

    for (i = 0, j = 0; i < size; i++, j++){
        arr[i] = j;
    }

    if(InitList_Sq(&L, 10)) printf("success!\n");
    else printf("failed!\n");

    ListBulkInsert_Sq(&L, 0, arr, size, 0);

    for (i = 0; i < L.size; i++){
        printf("%d\n", L.base[i]);
    }

    ElemType e;
    ListErase_Sq(&L, 3, &e, 0);
    printf("%d\n", e);

    if(DestroyList_Sq(&L)) printf("success!\n");
    else printf("failed!\n");
    return 0;
}
