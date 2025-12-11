#include <stdio.h>
#include <stdlib.h>
#include "sqlist.h"
#include "a.h"


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

    ListTraverse_Sq(L, PrintElem_);
    ElemType e = 6;
    int position = ListFind_Sq(L, e, 0);
    printf("%d\n", position);
    if(DestroyList_Sq(&L)) printf("success!\n");
    else printf("failed!\n");

    test_();
    return 0;
}
