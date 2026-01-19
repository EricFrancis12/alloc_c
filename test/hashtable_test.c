#include <stdbool.h>

#include "../vendor/unity/unity.h"

#include "hashtable.h"

DEF_HASHTABLE(u64, u64);

static size_t count_ht_u64_u64_entries(ht_u64_u64 *table)
{
    size_t count = 0;
    for (size_t i = 0; i < table->capacity; i++)
    {
        if (!table->entries[i].is_empty)
        {
            count++;
        }
    }
    return count;
}

void test_hashtable_u64_u64()
{
    ht_u64_u64 *table = ht_u64_u64_create(1024);
    TEST_ASSERT_NOT_NULL(table);
    TEST_ASSERT_EQUAL_size_t(0, count_ht_u64_u64_entries(table));
    TEST_ASSERT_EQUAL_size_t(0, table->length);

    bool ok = ht_u64_u64_set(table, 1, 2);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(1, count_ht_u64_u64_entries(table));
    TEST_ASSERT_EQUAL_size_t(1, table->length);
    u64 out;
    ok = ht_u64_u64_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(2, out);

    ht_u64_u64_delete(table, 1);
    TEST_ASSERT_EQUAL_size_t(0, count_ht_u64_u64_entries(table));
    TEST_ASSERT_EQUAL_size_t(0, table->length);
    ok = ht_u64_u64_get(table, 1, &out);
    TEST_ASSERT_FALSE(ok);
    TEST_ASSERT_EQUAL_UINT64(2, out);

    ok = ht_u64_u64_set(table, 1, 3);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(1, count_ht_u64_u64_entries(table));
    TEST_ASSERT_EQUAL_size_t(1, table->length);
    ok = ht_u64_u64_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(3, out);

    ok = ht_u64_u64_set(table, 1, 4);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(1, count_ht_u64_u64_entries(table));
    TEST_ASSERT_EQUAL_size_t(1, table->length);
    ok = ht_u64_u64_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(4, out);

    ht_u64_u64_destroy(table);
}

DEF_HASHTABLE_H(u64, size_t);
DEF_HASHTABLE(u64, size_t);

static size_t count_ht_u64_size_t_entries(ht_u64_size_t *table)
{
    size_t count = 0;
    for (size_t i = 0; i < table->capacity; i++)
    {
        if (!table->entries[i].is_empty)
        {
            count++;
        }
    }
    return count;
}

void test_hashtable_u64_size_t()
{
    ht_u64_size_t *table = ht_u64_size_t_create(1024);
    TEST_ASSERT_EQUAL_size_t(0, count_ht_u64_size_t_entries(table));
    TEST_ASSERT_EQUAL_size_t(0, table->length);

    bool ok = ht_u64_size_t_set(table, 1, 2);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(1, count_ht_u64_size_t_entries(table));
    TEST_ASSERT_EQUAL_size_t(1, table->length);
    size_t out;
    ok = ht_u64_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(2, out);

    ht_u64_size_t_delete(table, 1);
    TEST_ASSERT_EQUAL_size_t(0, count_ht_u64_size_t_entries(table));
    TEST_ASSERT_EQUAL_size_t(0, table->length);
    ok = ht_u64_size_t_get(table, 1, &out);
    TEST_ASSERT_FALSE(ok);
    TEST_ASSERT_EQUAL_size_t(2, out);

    ok = ht_u64_size_t_set(table, 1, 3);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(1, count_ht_u64_size_t_entries(table));
    TEST_ASSERT_EQUAL_size_t(1, table->length);
    ok = ht_u64_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(3, out);

    ok = ht_u64_size_t_set(table, 1, 4);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(1, count_ht_u64_size_t_entries(table));
    TEST_ASSERT_EQUAL_size_t(1, table->length);
    ok = ht_u64_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(4, out);

    ht_u64_size_t_destroy(table);
}

DEF_HASHTABLE_H(size_t, size_t);
DEF_HASHTABLE(size_t, size_t);

static size_t count_ht_size_t_size_t_entries(ht_size_t_size_t *table)
{
    size_t count = 0;
    for (size_t i = 0; i < table->capacity; i++)
    {
        if (!table->entries[i].is_empty)
        {
            count++;
        }
    }
    return count;
}

void test_hashtable_size_t_size_t()
{
    ht_size_t_size_t *table = ht_size_t_size_t_create(1024);
    TEST_ASSERT_EQUAL_size_t(0, count_ht_size_t_size_t_entries(table));
    TEST_ASSERT_EQUAL_size_t(0, table->length);

    bool ok = ht_size_t_size_t_set(table, 1, 2);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(1, count_ht_size_t_size_t_entries(table));
    TEST_ASSERT_EQUAL_size_t(1, table->length);
    size_t out;
    ok = ht_size_t_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(2, out);

    ht_size_t_size_t_delete(table, 1);
    TEST_ASSERT_EQUAL_size_t(0, count_ht_size_t_size_t_entries(table));
    TEST_ASSERT_EQUAL_size_t(0, table->length);
    ok = ht_size_t_size_t_get(table, 1, &out);
    TEST_ASSERT_FALSE(ok);
    TEST_ASSERT_EQUAL_size_t(2, out);

    ok = ht_size_t_size_t_set(table, 1, 3);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(1, count_ht_size_t_size_t_entries(table));
    TEST_ASSERT_EQUAL_size_t(1, table->length);
    ok = ht_size_t_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(3, out);

    ok = ht_size_t_size_t_set(table, 1, 4);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(1, count_ht_size_t_size_t_entries(table));
    TEST_ASSERT_EQUAL_size_t(1, table->length);
    ok = ht_size_t_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(4, out);

    ht_size_t_size_t_destroy(table);
}
