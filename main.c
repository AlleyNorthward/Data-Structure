#include <stdio.h>
#include "sqlist.h"


int main(){
    SqList L;
    if(InitList_Sq(&L)) printf("创立成功!\n");
    else printf("创立失败!\n");
    if(DestroyList_Sq(&L)) printf("销毁成功!\n");
    else printf("销毁失败!\n");

    return 0;
}









