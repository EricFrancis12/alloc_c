#include "../vendor/unity/unity.h"

#include "hashtable.h"

DEF_HASHTABLE(u64, u64);

void test_hashtable_u64_u64()
{
    ht_u64_u64 *table = ht_u64_u64_create(1024);

    bool ok = ht_u64_u64_set(table, 1, 2);
    TEST_ASSERT_TRUE(ok);
    u64 out;
    ok = ht_u64_u64_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(2, out);

    ht_u64_u64_delete(table, 1);
    ok = ht_u64_u64_get(table, 1, &out);
    TEST_ASSERT_FALSE(ok);
    TEST_ASSERT_EQUAL_UINT64(2, out);

    ok = ht_u64_u64_set(table, 1, 3);
    TEST_ASSERT_TRUE(ok);
    ok = ht_u64_u64_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(3, out);

    ok = ht_u64_u64_set(table, 1, 4);
    TEST_ASSERT_TRUE(ok);
    ok = ht_u64_u64_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(4, out);

    ht_u64_u64_destroy(table);
}

DEF_HASHTABLE(u64, size_t);

void test_hashtable_u64_size_t()
{
    ht_u64_size_t *table = ht_u64_size_t_create(1024);

    bool ok = ht_u64_size_t_set(table, 1, 2);
    TEST_ASSERT_TRUE(ok);
    size_t out;
    ok = ht_u64_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(2, out);

    ht_u64_size_t_delete(table, 1);
    ok = ht_u64_size_t_get(table, 1, &out);
    TEST_ASSERT_FALSE(ok);
    TEST_ASSERT_EQUAL_size_t(2, out);

    ok = ht_u64_size_t_set(table, 1, 3);
    TEST_ASSERT_TRUE(ok);
    ok = ht_u64_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(3, out);

    ok = ht_u64_size_t_set(table, 1, 4);
    TEST_ASSERT_TRUE(ok);
    ok = ht_u64_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(4, out);

    ht_u64_size_t_destroy(table);
}

DEF_HASHTABLE(size_t, size_t);

void test_hashtable_size_t_size_t()
{
    ht_size_t_size_t *table = ht_size_t_size_t_create(1024);

    bool ok = ht_size_t_size_t_set(table, 1, 2);
    TEST_ASSERT_TRUE(ok);
    size_t out;
    ok = ht_size_t_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(2, out);

    ht_size_t_size_t_delete(table, 1);
    ok = ht_size_t_size_t_get(table, 1, &out);
    TEST_ASSERT_FALSE(ok);
    TEST_ASSERT_EQUAL_size_t(2, out);

    ok = ht_size_t_size_t_set(table, 1, 3);
    TEST_ASSERT_TRUE(ok);
    ok = ht_size_t_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(3, out);

    ok = ht_size_t_size_t_set(table, 1, 4);
    TEST_ASSERT_TRUE(ok);
    ok = ht_size_t_size_t_get(table, 1, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(4, out);

    ht_size_t_size_t_destroy(table);
}
