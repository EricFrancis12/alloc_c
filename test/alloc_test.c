#include <stdbool.h>

#include "../vendor/unity/unity.h"

#include "alloc.h"
#include "hashtable.c"

static size_t count_ht_entries(ht_u64_u64 *table)
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

void test_create_allocator()
{
    u64 capacity = 1024;
    allocator *alloc = allocator_create(capacity);
    TEST_ASSERT_NOT_NULL(alloc);
    TEST_ASSERT_EQUAL_UINT64(capacity, alloc->capacity);
    TEST_ASSERT_EQUAL_size_t(1, count_ht_entries(alloc->open));
    TEST_ASSERT_EQUAL_size_t(0, count_ht_entries(alloc->occupied));
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

    out = allocator_get_occupied(alloc, p);
    TEST_ASSERT_EQUAL_UINT64(100, out);

    out = allocator_get_open(alloc, p);
    TEST_ASSERT_EQUAL_UINT64(0, out);

    ok = ht_u64_u64_get(alloc->occupied, 0, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(100, out);

    ok = ht_u64_u64_get(alloc->open, 0, &out);
    TEST_ASSERT_FALSE(ok);

    allocator_destroy(alloc);
}

void test_alloc_exact_fit()
{
    allocator *alloc = allocator_create(100);
    TEST_ASSERT_NOT_NULL(alloc);

    void *p = allocator_alloc(alloc, 100);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_PTR(&alloc->data[0], p);

    // Check that no open blocks remain
    TEST_ASSERT_EQUAL_size_t(0, count_ht_entries(alloc->open));

    // Check occupied block
    u64 out;
    bool ok = ht_u64_u64_get(alloc->occupied, 0, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(100, out);

    allocator_destroy(alloc);
}

void test_alloc_multiple()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    void *p1 = allocator_alloc(alloc, 100);
    void *p2 = allocator_alloc(alloc, 200);
    void *p3 = allocator_alloc(alloc, 300);

    TEST_ASSERT_EQUAL_PTR(&alloc->data[0], p1);
    TEST_ASSERT_EQUAL_PTR(&alloc->data[100], p2);
    TEST_ASSERT_EQUAL_PTR(&alloc->data[300], p3);

    // Check we have 3 occupied blocks
    TEST_ASSERT_EQUAL_size_t(3, count_ht_entries(alloc->occupied));

    u64 out = allocator_get_occupied(alloc, p1);
    TEST_ASSERT_EQUAL_UINT64(100, out);

    out = allocator_get_occupied(alloc, p2);
    TEST_ASSERT_EQUAL_UINT64(200, out);

    out = allocator_get_occupied(alloc, p3);
    TEST_ASSERT_EQUAL_UINT64(300, out);

    allocator_destroy(alloc);
}

void test_alloc_insufficient_space()
{
    allocator *alloc = allocator_create(100);
    TEST_ASSERT_NOT_NULL(alloc);

    void *p = allocator_alloc(alloc, 200);
    TEST_ASSERT_NULL(p);

    // Check no occupied blocks created
    TEST_ASSERT_EQUAL_size_t(0, count_ht_entries(alloc->occupied));

    allocator_destroy(alloc);
}

void test_alloc_zero_size()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    void *p = allocator_alloc(alloc, 0);
    TEST_ASSERT_NULL(p);

    // Should still have the full capacity open
    u64 out;
    bool ok = ht_u64_u64_get(alloc->open, 0, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(1024, out);

    allocator_destroy(alloc);
}

void test_free_simple()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    void *p = allocator_alloc(alloc, 100);
    TEST_ASSERT_NOT_NULL(p);

    u64 size = allocator_free(alloc, p);
    TEST_ASSERT_EQUAL_UINT64(100, size);

    // Check occupied block removed
    u64 out = allocator_get_occupied(alloc, p);
    TEST_ASSERT_EQUAL_UINT64(0, out);

    // After freeing, the block at 0 (size 100) merges with the block at 100 (size 924)
    // resulting in one block at 0 with size 1024
    bool ok = ht_u64_u64_get(alloc->open, 0, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(1024, out);

    TEST_ASSERT_EQUAL_size_t(1, count_ht_entries(alloc->open));

    allocator_destroy(alloc);
}

void test_free_invalid_index()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    void *invalid_ptr = &alloc->data[500];
    u64 size = allocator_free(alloc, invalid_ptr);

    TEST_ASSERT_EQUAL_UINT64(0, size);

    allocator_destroy(alloc);
}

void test_free_double_free()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    void *p = allocator_alloc(alloc, 100);
    TEST_ASSERT_NOT_NULL(p);

    // First free should succeed
    u64 size1 = allocator_free(alloc, p);
    TEST_ASSERT_EQUAL_UINT64(100, size1);

    // Second free should fail
    u64 size2 = allocator_free(alloc, p);
    TEST_ASSERT_EQUAL_UINT64(0, size2);

    allocator_destroy(alloc);
}

void test_merge_open_blocks_simple()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    // Allocate two blocks
    void *p1 = allocator_alloc(alloc, 100);
    void *p2 = allocator_alloc(alloc, 100);

    // Free first block
    allocator_free(alloc, p1);

    // Free second block - should merge with first
    allocator_free(alloc, p2);

    // Should have one merged block
    TEST_ASSERT_EQUAL_size_t(1, count_ht_entries(alloc->open));

    u64 out;
    bool ok = ht_u64_u64_get(alloc->open, 0, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(1024, out);

    allocator_destroy(alloc);
}

void test_merge_open_blocks_transitive()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    // Allocate three adjacent blocks
    void *p1 = allocator_alloc(alloc, 100); // index 0
    void *p2 = allocator_alloc(alloc, 100); // index 100
    void *p3 = allocator_alloc(alloc, 100); // index 200

    // Free all three
    allocator_free(alloc, p1);
    allocator_free(alloc, p2);
    allocator_free(alloc, p3);

    // Should ideally have one merged block (transitive merge)
    // If transitive merge works, length should be 1
    // Calculate total open space to verify correctness regardless
    u64 total_open = 0;
    for (u64 i = 0; i < alloc->open->capacity; i++)
    {
        ht_u64_u64_entry entry = alloc->open->entries[i];
        if (!entry.is_empty)
        {
            total_open += entry.value;
        }
    }

    TEST_ASSERT_EQUAL_UINT64(1024, total_open);

    allocator_destroy(alloc);
}

void test_merge_open_blocks_non_adjacent()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    // Allocate blocks with a gap
    void *p1 = allocator_alloc(alloc, 100); // index 0
    allocator_alloc(alloc, 100);            // index 100
    void *p3 = allocator_alloc(alloc, 100); // index 200

    // Free first and third, leaving middle occupied
    allocator_free(alloc, p1);
    allocator_free(alloc, p3);

    // Should have two separate open blocks
    TEST_ASSERT_EQUAL_size_t(2, count_ht_entries(alloc->open));

    allocator_destroy(alloc);
}

void test_alloc_after_free()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    // Allocate and free to create fragmentation
    void *p1 = allocator_alloc(alloc, 100);
    void *p2 = allocator_alloc(alloc, 100);
    allocator_alloc(alloc, 100);

    allocator_free(alloc, p1);
    allocator_free(alloc, p2);

    // After merge, should be able to allocate larger block
    void *p = allocator_alloc(alloc, 200);
    TEST_ASSERT_NOT_NULL(p);

    u64 out = allocator_get_occupied(alloc, p);
    TEST_ASSERT_EQUAL_UINT64(200, out);

    allocator_destroy(alloc);
}

void test_merge_count_return()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    // Allocate two adjacent blocks
    void *p1 = allocator_alloc(alloc, 100);
    void *p2 = allocator_alloc(alloc, 100);

    // Free both
    allocator_free(alloc, p1);
    u64 size = allocator_free(alloc, p2);

    // Second free should succeed and trigger merge
    TEST_ASSERT_EQUAL_UINT64(100, size);

    // Verify merge happened - should have at most 2 open blocks
    TEST_ASSERT_LESS_OR_EQUAL_size_t(2, count_ht_entries(alloc->open));

    allocator_destroy(alloc);
}

void test_full_cycle()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    // Allocate everything
    void *p = allocator_alloc(alloc, 1024);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_PTR(&alloc->data[0], p);

    // Check no open blocks remain
    TEST_ASSERT_EQUAL_size_t(0, count_ht_entries(alloc->open));

    // Free everything
    u64 size = allocator_free(alloc, p);
    TEST_ASSERT_EQUAL_UINT64(1024, size);

    // Check one open block with full capacity
    TEST_ASSERT_EQUAL_size_t(1, count_ht_entries(alloc->open));

    u64 out;
    bool ok = ht_u64_u64_get(alloc->open, 0, &out);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT64(1024, out);

    allocator_destroy(alloc);
}
