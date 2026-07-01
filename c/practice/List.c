#include "List.h"
#include <stdlib.h>

List* newList(){
  List* list = (List*)malloc(sizeof(List));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;

  return list;
}
