#include "List.h"
#include <stddef.h>

int main() {
  List *list = newList();
  destroy_L(list, NULL);
}
