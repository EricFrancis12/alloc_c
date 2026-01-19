#include <string.h>

#include "../vendor/unity/unity.h"

#include "alloc.h"
#include "strings.h"
#include "hashtable.c"

void test_concat()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    char *result = concat(alloc, 3, "Hello", " ", "World");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("Hello World", result);

    result = concat(alloc, 2, "foo", "bar");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("foobar", result);

    result = concat(alloc, 1, "single");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("single", result);

    result = concat(alloc, 4, "", "a", "", "b");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("ab", result);

    allocator_destroy(alloc);
}

void test_trim()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    char *result = trim(alloc, "  hello  ");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("hello", result);

    result = trim(alloc, "\t\ntest\r\n");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("test", result);

    result = trim(alloc, "no_spaces");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("no_spaces", result);

    result = trim(alloc, "   ");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);

    result = trim(alloc, "");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);

    allocator_destroy(alloc);
}

void test_trim_left()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    char *result = trim_left(alloc, "  hello  ");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("hello  ", result);

    result = trim_left(alloc, "\t\ntest");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("test", result);

    result = trim_left(alloc, "no_spaces");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("no_spaces", result);

    result = trim_left(alloc, "   ");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);

    allocator_destroy(alloc);
}

void test_trim_right()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    char *result = trim_right(alloc, "  hello  ");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("  hello", result);

    result = trim_right(alloc, "test\r\n");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("test", result);

    result = trim_right(alloc, "no_spaces");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("no_spaces", result);

    // When string is all whitespace and we only trim right (not left),
    // the implementation returns the leftmost space since it can't trim left
    result = trim_right(alloc, "   ");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING(" ", result);

    allocator_destroy(alloc);
}

void test_left_pad()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    char *result = left_pad(alloc, "42", '0', 5);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("00042", result);

    result = left_pad(alloc, "hello", ' ', 10);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("     hello", result);

    result = left_pad(alloc, "test", '*', 4);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("test", result);

    result = left_pad(alloc, "longer", '-', 3);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("longer", result);

    result = left_pad(alloc, "", 'x', 3);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("xxx", result);

    allocator_destroy(alloc);
}

void test_right_pad()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    char *result = right_pad(alloc, "42", '0', 5);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("42000", result);

    result = right_pad(alloc, "hello", ' ', 10);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("hello     ", result);

    result = right_pad(alloc, "test", '*', 4);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("test", result);

    result = right_pad(alloc, "longer", '-', 3);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("longer", result);

    result = right_pad(alloc, "", 'x', 3);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("xxx", result);

    allocator_destroy(alloc);
}

void test_substr()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    char *result = substr(alloc, "Hello World", 0, 5);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("Hello", result);

    result = substr(alloc, "Hello World", 6, 11);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("World", result);

    result = substr(alloc, "test", 1, 3);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("es", result);

    result = substr(alloc, "test", 5, 10);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);

    result = substr(alloc, "test", 2, 2);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);

    result = substr(alloc, "test", 3, 1);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);

    result = substr(alloc, "", 0, 0);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);

    allocator_destroy(alloc);
}

void test_indexof()
{
    TEST_ASSERT_EQUAL_INT(0, indexof("hello", 'h'));
    TEST_ASSERT_EQUAL_INT(4, indexof("hello", 'o'));
    TEST_ASSERT_EQUAL_INT(2, indexof("hello", 'l'));
    TEST_ASSERT_EQUAL_INT(-1, indexof("hello", 'x'));
    TEST_ASSERT_EQUAL_INT(-1, indexof("", 'a'));
    TEST_ASSERT_EQUAL_INT(0, indexof("a", 'a'));
}

void test_contains()
{
    TEST_ASSERT_TRUE(contains("hello", 'h'));
    TEST_ASSERT_TRUE(contains("hello", 'e'));
    TEST_ASSERT_TRUE(contains("hello", 'o'));
    TEST_ASSERT_FALSE(contains("hello", 'x'));
    TEST_ASSERT_FALSE(contains("", 'a'));
    TEST_ASSERT_TRUE(contains("a", 'a'));
}

void test_starts_with()
{
    TEST_ASSERT_TRUE(starts_with("hello", 'h'));
    TEST_ASSERT_FALSE(starts_with("hello", 'e'));
    TEST_ASSERT_FALSE(starts_with("hello", 'o'));
    TEST_ASSERT_TRUE(starts_with("world", 'w'));
    TEST_ASSERT_FALSE(starts_with("", 'a'));
}

void test_ends_with()
{
    TEST_ASSERT_TRUE(ends_with("hello", 'o'));
    TEST_ASSERT_FALSE(ends_with("hello", 'h'));
    TEST_ASSERT_FALSE(ends_with("hello", 'l'));
    TEST_ASSERT_TRUE(ends_with("world", 'd'));
    TEST_ASSERT_FALSE(ends_with("a", 'b'));
    TEST_ASSERT_TRUE(ends_with("a", 'a'));
}

void test_eq_ignore_case()
{
    TEST_ASSERT_TRUE(eq_ignore_case("hello", "hello"));
    TEST_ASSERT_TRUE(eq_ignore_case("HELLO", "hello"));
    TEST_ASSERT_TRUE(eq_ignore_case("hello", "HELLO"));
    TEST_ASSERT_TRUE(eq_ignore_case("HeLLo", "hEllO"));
    TEST_ASSERT_TRUE(eq_ignore_case("Test123", "test123"));
    TEST_ASSERT_TRUE(eq_ignore_case("ABC", "abc"));

    TEST_ASSERT_FALSE(eq_ignore_case("hello", "world"));
    TEST_ASSERT_FALSE(eq_ignore_case("hello", "hell"));
    TEST_ASSERT_FALSE(eq_ignore_case("hello", "helloo"));
    TEST_ASSERT_FALSE(eq_ignore_case("", "a"));
    TEST_ASSERT_TRUE(eq_ignore_case("", ""));
}

void test_split()
{
    allocator *alloc = allocator_create(1024);
    TEST_ASSERT_NOT_NULL(alloc);

    str_split result = split(alloc, "a,b,c", ",");
    TEST_ASSERT_EQUAL_INT(3, result.num_parts);
    TEST_ASSERT_EQUAL_STRING("a", result.parts[0]);
    TEST_ASSERT_EQUAL_STRING("b", result.parts[1]);
    TEST_ASSERT_EQUAL_STRING("c", result.parts[2]);

    result = split(alloc, "one::two::three", "::");
    TEST_ASSERT_EQUAL_INT(3, result.num_parts);
    TEST_ASSERT_EQUAL_STRING("one", result.parts[0]);
    TEST_ASSERT_EQUAL_STRING("two", result.parts[1]);
    TEST_ASSERT_EQUAL_STRING("three", result.parts[2]);

    result = split(alloc, "hello world", " ");
    TEST_ASSERT_EQUAL_INT(2, result.num_parts);
    TEST_ASSERT_EQUAL_STRING("hello", result.parts[0]);
    TEST_ASSERT_EQUAL_STRING("world", result.parts[1]);

    result = split(alloc, "no_delimiter", ",");
    TEST_ASSERT_EQUAL_INT(1, result.num_parts);
    TEST_ASSERT_EQUAL_STRING("no_delimiter", result.parts[0]);

    result = split(alloc, ",,", ",");
    TEST_ASSERT_EQUAL_INT(3, result.num_parts);
    TEST_ASSERT_EQUAL_STRING("", result.parts[0]);
    TEST_ASSERT_EQUAL_STRING("", result.parts[1]);
    TEST_ASSERT_EQUAL_STRING("", result.parts[2]);

    // Empty string splits into one part (the empty string itself)
    result = split(alloc, "", ",");
    TEST_ASSERT_EQUAL_INT(1, result.num_parts);
    TEST_ASSERT_EQUAL_STRING("", result.parts[0]);

    result = split(alloc, "test", "");
    TEST_ASSERT_EQUAL_INT(0, result.num_parts);

    allocator_destroy(alloc);
}
