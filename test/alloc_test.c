#include <stdbool.h>

#include "../vendor/unity/unity.h"

#include "alloc.h"
#include "hashtable.c"

void test_create_allocator()
{
    u64 capacity = 1024;
    allocator *alloc = allocator_create(capacity);
    TEST_ASSERT_NOT_NULL(alloc);
    TEST_ASSERT_EQUAL_UINT64(capacity, alloc->capacity);
    TEST_ASSERT_EQUAL_size_t(1, alloc->open->length);
    TEST_ASSERT_EQUAL_UINT64(0, alloc->occupied->length);
}

void test_alloc_simple()
{
    u64 capacity = 1024;
    allocator *alloc = allocator_create(capacity);
    TEST_ASSERT_NOT_NULL(alloc);

    u64 out;
    bool ok = ht_u64_u64_get(alloc->open, 0, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(capacity, out);

    void *p = allocator_alloc(alloc, 100);
    TEST_ASSERT_NOT_NULL(p);

    ok = allocator_get_occupied(alloc, p, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(100, out);

    ok = allocator_get_open(alloc, p, &out);
    TEST_ASSERT_FALSE(ok);

    ok = ht_u64_u64_get(alloc->occupied, 0, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(100, out);

    ok = ht_u64_u64_get(alloc->open, 0, &out);
    TEST_ASSERT_FALSE(ok);

    allocator_destroy(alloc);
}
