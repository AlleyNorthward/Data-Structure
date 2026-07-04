#ifndef CONFIG_H
#define CONFIG_H
#include <stddef.h>

// #define container_of(ptr, type, member) \
//   ({ \
//     const typedef(((type *)0)->member) *__mptr = (ptr); \
//     (type *)((char *)__mptr - offsetof(type, member)); \
//   })

#define container_of(ptr, type, member)                                        \
  ((type *)((char *)(ptr) - offsetof(type, member)))
#endif
