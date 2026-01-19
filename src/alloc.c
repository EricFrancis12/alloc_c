#include "alloc.h"

allocator *allocator_create(u64 capacity)
{
    allocator *alloc = malloc(sizeof(allocator));
    if (alloc == NULL)
    {
        return NULL;
    }

    u8 *data = malloc(capacity);
    if (data == NULL)
    {
        free(alloc);
        return NULL;
    }

    ht_u64_u64 *open = ht_u64_u64_create(1024);
    if (open == NULL)
    {
        free(alloc);
        free(data);
        return NULL;
    }
    if (!ht_u64_u64_set(open, 0, capacity))
    {
        free(alloc);
        free(data);
        ht_u64_u64_destroy(open);
        return NULL;
    }

    ht_u64_u64 *occupied = ht_u64_u64_create(1024);
    if (occupied == NULL)
    {
        free(alloc);
        free(data);
        ht_u64_u64_destroy(open);
        return NULL;
    }

    alloc->capacity = capacity;
    alloc->data = data;
    alloc->open = open;
    alloc->occupied = occupied;

    return alloc;
}

void allocator_destroy(allocator *alloc)
{
    ht_u64_u64_destroy(alloc->open);
    ht_u64_u64_destroy(alloc->occupied);
    free(alloc->data);
    free(alloc);
}

void *allocator_alloc(allocator *alloc, u64 n)
{
    // First fit
    for (u64 i = 0; i < alloc->open->capacity; i++)
    {
        ht_u64_u64_entry entry = alloc->open->entries[i];
        if (entry.is_empty)
        {
            continue;
        }

        u64 index = entry.key;
        u64 open_space = entry.value;

        if (open_space >= n)
        {
            void *ret = &alloc->data[index];

            if (!ht_u64_u64_set(alloc->occupied, index, n))
            {
                return NULL;
            }

            ht_u64_u64_delete(alloc->open, index);

            u64 new_open_space = open_space - n;
            if (new_open_space > 0)
            {
                if (!ht_u64_u64_set(alloc->open, index + n, new_open_space))
                {
                    return NULL;
                }
            }

            return ret;
        }
    }

    return NULL;
}

// u64 allocator_free(allocator *alloc, void *p)
// {
//     // TODO: ...

//     // u64 n;
//     // ht_get(arena->occupied, );
//     // n, ok := a.occupied[index]
//     // if !ok {
//     // 	return 0, false
//     // }

//     // delete(a.occupied, index)

//     // a.open[index] = n

//     // a.mergeOpenBlocks()

//     // return n, true
// }

bool allocator_get_open(allocator *alloc, void *p, u64 *out)
{
    u64 index = (u8 *)p - alloc->data;
    return ht_u64_u64_get(alloc->open, index, out);
}

bool allocator_get_occupied(allocator *alloc, void *p, u64 *out)
{
    u64 index = (u8 *)p - alloc->data;
    return ht_u64_u64_get(alloc->occupied, index, out);
}
