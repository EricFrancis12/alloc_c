#include <stdio.h>

#include "hashtable.h"

DEF_HASHTABLE(u64, u64);

main()
{
    printf("Hello\n");

    ht_u64_u64 *table = ht_u64_u64_create(1024);

    ht_u64_u64_set(table, 1, 2);
    u64 out;
    bool ok = ht_u64_u64_get(table, 1, &out);
    printf("ok: %d\n", ok);
    printf("out: %llu\n", out);

    ht_u64_u64_destroy(table);

    return 0;
}
