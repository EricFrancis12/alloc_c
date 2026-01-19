#include "../vendor/unity/unity.h"

#include "base.h"
#include "hashtable.h"

DEF_HASHTABLE(u64, u64);

void test_hashtable()
{
    ht_u64_u64 *table = ht_u64_u64_create(1024);

    ht_u64_u64_set(table, 1, 2);
    u64 out;
    bool ok = ht_u64_u64_get(table, 1, &out);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(2, out);

    ht_u64_u64_destroy(table);
}
