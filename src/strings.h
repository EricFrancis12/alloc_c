#ifndef STRINGS_H
#define STRINGS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "alloc.h"

char *concat(allocator *alloc, int count, ...);

bool is_whitespace(char c);

char *trim(allocator *alloc, const char *s);

char *trim_left(allocator *alloc, const char *s);

char *trim_right(allocator *alloc, const char *s);

char *left_pad(allocator *alloc, const char *s, char pad_char, u32 total_len);

char *right_pad(allocator *alloc, const char *s, char pad_char, u32 total_len);

char *substr(allocator *alloc, const char *s, u32 start, u32 end);

int indexof(const char *s, char c);

bool contains(const char *s, char c);

bool starts_with(const char *s, char c);

bool ends_with(const char *s, char c);

bool eq_ignore_case(const char *a, const char *b);

typedef struct
{
    char **parts;
    int num_parts;
} str_split;

str_split split(allocator *alloc, const char *s, const char *pat);

#endif
