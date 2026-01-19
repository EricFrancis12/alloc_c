#ifndef ALLOC_H
#define ALLOC_H

#include "base.h"
#include "hashtable.h"

typedef struct allocator
{
    u64 capacity;
    u8 *data;
    ht_u64_u64 *open;
    ht_u64_size_t *occupied;
} allocator;

allocator *alloc_create(u64 capacity);

void alloc_destroy(allocator *alloc);

#endif
