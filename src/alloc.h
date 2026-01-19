#ifndef ALLOC_H
#define ALLOC_H

#include <stdbool.h>

#include "base.h"
#include "hashtable.h"

typedef struct allocator
{
    u64 capacity;
    u8 *data;
    ht_u64_u64 *open;
    ht_u64_u64 *occupied;
} allocator;

allocator *allocator_create(u64 capacity);

void allocator_destroy(allocator *alloc);

void *allocator_alloc(allocator *alloc, u64 n);

bool allocator_get_open(allocator *alloc, void *p, u64 *out);

bool allocator_get_occupied(allocator *alloc, void *p, u64 *out);

#endif
