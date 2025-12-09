#include <stdio.h>
#include "sqlist.h"


int main(){
    SqList L;
    if(InitList_Sq(&L, 10)) printf("success!\n");
    else printf("failed!\n");
    if(DestroyList_Sq(&L)) printf("success!\n");
    else printf("failed!\n");

    return 0;
}









