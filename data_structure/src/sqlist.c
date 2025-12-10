#include "sqlist.h"
#include <malloc.h>
#include<stdlib.h>

Status InitList_Sq(SqList* L, int capacity){
    L->base = (ElemType* )malloc(capacity * sizeof(ElemType));
    if(!L->base) exit(OVERFLOW_);

    L->capacity = capacity;
    L->size = 0;
    return OK_;
}

Status DestroyList_Sq(SqList *L){
    if(!L->base) return ERROR_;

    free(L->base);
    L->base = NULL;
    L->size = 0;
    return OK_;
}

Status ListClear_Sq(SqList *L){
    if(L->base){
        L->size = 0;
        return OK_;
    }
    return ERROR_;
}

Status ListInsert_Sq(SqList *L, int position, ElemType e, u8 flag){
    /* 
        is position legal?
        realloc?
        position start at 0 or 1? if flag == 0 -> start at 0 else start at 1
        updata capacity?

        understand operation space
            for array, we use [0, n), which is 0-based.
            so, to solve this problem, the position parameter also use 0-based.
            the position parameter's operation space is [0, L->size]( L->base is [0, L->size) )
            so, the illegal operation space for position parameter is (0, L->size) easily.
            for k, its operation space is [0, L->size) ( [0, L->size - 1] to fit position parameter' space operation( [0, L->size] ) )
            so, if k == position, we'll insert. if k > position, we'll not insert
            based this, we know the for loop condition , which is (k = L->size - 1; k >= position; k--)
        the for loop condition we've got, is based on the position operation space and the k(L->base) operation space.
    */
    int k;
    if (flag == 1) position = position - 1;

//     if(position <= -1 || position >= L->size + 1) what do you think?
    if(position < 0 || position > L->size) return ERROR_;
    if(L->size == L->capacity){
        L->base = (ElemType*)realloc(L->base, 2 * L->capacity * sizeof(ElemType));

        if(!L->base) exit(OVERFLOW_);
        L->capacity *= 2;
    }

    for(k = L->size - 1; k >= position; k--){
        L->base[k+1] = L->base[k];
    }

    L->base[position] = e;
    L->size ++;
    return OK_;
}

Status ListBulkInsert_Sq(SqList* L, int position, int* arr, int arr_size, u8 flag){
    int j, k; 
    if(flag) position = position - 1;
    if(position < 0 || position > L->size) return ERROR_;

    if(L->capacity <= L->size + arr_size){
        L->base = (ElemType*)realloc(L->base, 2 * (L->size + arr_size) * sizeof(ElemType));
        if(!L->base) exit(OVERFLOW_);
    }

    for(k = L->size - 1; k >= position; k--){
        L->base[k + arr_size] = L->base[k];
    }

    for(j = 0; j < arr_size; j++){
        L->base[position + j] = arr[j];
    }

    L->size += arr_size;
    return OK_;
}

Status ListErase_Sq(SqList* L, int position, ElemType* e, u8 flag){
    int k;
    if(flag) position = position - 1;
    if(position < 0 || position > L->size - 1) return ERROR_;
//     position => [0, L->size - 1]
//     k = position , delete , position > k 
    *e = L->base[position];
//     e = &L->base[position];  这样写行吗?
    for(k = position; k < L->size - 1; k ++){
        L->base[k] = L->base[k+1];
    }
    L->size--;
    return OK_;
}












