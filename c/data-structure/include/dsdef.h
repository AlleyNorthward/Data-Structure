#ifndef DSDEF_H
#define DSDEF_H
#include <stdbool.h>
#include <stddef.h>

// #define container_of(ptr, type, member) \
//   ({ \
//     const typedef(((type *)0)->member) *__mptr = (ptr); \
//     (type *)((char *)__mptr - offsetof(type, member)); \
//   })

#define container_of(ptr, type, member)                                        \
  ((type *)((char *)(ptr) - offsetof(type, member)))

typedef int (*cmp_t)(void *a, void *b);

#endif
