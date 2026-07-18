#ifndef DSCOMMON_H
#define DSCOMMON_H

void swap_bytes(void* a, void* b, int elem_size);

#define max(x, y) ({                      \
    typeof(x) _max1 = (x);                \
    typeof(y) _max2 = (y);                \
    (void) (&_max1 == &_max2);            \
    _max1 > _max2 ? _max1 : _max2; })

#endif
