#include "dscommon.h"
#include <stdlib.h>
#include <string.h>

void swap_bytes(void *a, void *b, int elem_size) {
  if (a == b)
    return;
  if (elem_size <= 64) {
    char tmp[64];
    memcpy(tmp, a, elem_size);
    memcpy(a, b, elem_size);
    memcpy(b, tmp, elem_size);
  } else {
    void *tmp = malloc(elem_size);
    memcpy(tmp, a, elem_size);
    memcpy(a, b, elem_size);
    memcpy(b, tmp, elem_size);
    free(tmp);
  }
}
