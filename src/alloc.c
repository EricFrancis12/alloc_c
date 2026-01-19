#include "alloc.h"

allocator *alloc_create(u64 capacity)
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

    ht_u64_size_t *occupied = ht_u64_size_t_create(1024);
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

void alloc_destroy(allocator *alloc)
{
    ht_u64_u64_destroy(alloc->open);
    ht_u64_size_t_destroy(alloc->occupied);
    free(alloc->data);
    free(alloc);
}

// void *arena_alloc(Arena *arena, u64 n)
// {
//     // First fit
//     for (size_t i = 0; i < arena->open->capacity; i++)
//     {
//         ht_entry entry = arena->open->entries[i];
//         if (entry.is_empty)
//         {
//             continue;
//         }

//         u64 index = entry.key;
//         u64 open_space = entry.value;

//         if (open_space >= n)
//         {
//             void *ret = arena->data[index];

//             if (!ht_set(arena->occupied, index, n))
//             {
//                 return NULL;
//             }

//             ht_delete(arena->open, index);

//             u64 new_open_space = open_space - n;
//             if (new_open_space > 0)
//             {
//                 if (!ht_set(arena->open, index + n, new_open_space))
//                 {
//                     return NULL;
//                 }
//             }

//             return ret;
//         }
//     }

//     return NULL;
// }

// u64 arena_free(Arena *arena, void *p)
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
