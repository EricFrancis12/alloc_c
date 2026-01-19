#include "base.h"
#include "hashtable.h"

typedef struct Allocator
{
    u64 capacity;
    u8 *data;
    ht_u64_u64 *open;
    ht_u64_size_t *occupied;
} Allocator;
