#include <string.h>

#include "strings.h"

char *concat(allocator *alloc, int count, ...)
{
    va_list args;

    size_t total_len = 0;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        const char *str = va_arg(args, const char *);
        total_len += strlen(str);
    }
    va_end(args);

    char *ret = allocator_alloc(alloc, sizeof(char) * (total_len + 1));
    if (ret == NULL)
    {
        return NULL;
    }

    size_t pos = 0;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        const char *str = va_arg(args, const char *);
        size_t len = strlen(str);
        memcpy(ret + pos, str, len);
        pos += len;
    }
    va_end(args);

    ret[pos] = '\0';
    return ret;
}

bool is_whitespace(char c)
{
    return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v';
}

char *trim_internal(allocator *alloc, const char *s, bool trim_left, bool trim_right)
{
    // Move the pointer forward to skip leading whitespace
    const char *start = s;
    if (trim_left)
    {
        while (is_whitespace(*start))
        {
            start++;
        }
    }

    // If the string is entirely whitespace, return an empty string
    if (*start == '\0')
    {
        char *ret = allocator_alloc(alloc, sizeof(char));
        ret[0] = '\0';
        return ret;
    }

    // Find the end of the string
    const char *end = start + strlen(start) - 1;

    // Move the end pointer backward to skip trailing whitespace
    if (trim_right)
    {
        while (end > start && is_whitespace(*end))
        {
            end--;
        }
    }

    // Calculate the length of the trimmed string
    size_t len = end - start + 1;

    // Allocate memory for the new string
    char *ret = allocator_alloc(alloc, sizeof(char) * (len + 1));
    if (ret == NULL)
    {
        return NULL;
    }

    // Copy the trimmed content to the new string
    memcpy(ret, start, len);
    ret[len] = '\0';

    return ret;
}

char *trim(allocator *alloc, const char *s)
{
    return trim_internal(alloc, s, 1, 1);
}

char *trim_left(allocator *alloc, const char *s)
{
    return trim_internal(alloc, s, 1, 0);
}

char *trim_right(allocator *alloc, const char *s)
{
    return trim_internal(alloc, s, 0, 1);
}

char *left_pad(allocator *alloc, const char *s, char pad_char, u32 total_len)
{
    size_t s_len = strlen(s);

    // If the string is already longer than or equal to total_len, return a copy
    if (s_len >= total_len)
    {
        char *ret = allocator_alloc(alloc, sizeof(char) * (s_len + 1));
        if (ret == NULL)
        {
            return NULL;
        }
        memcpy(ret, s, s_len + 1);
        return ret;
    }

    // Calculate how many padding characters we need
    u32 pad_count = total_len - s_len;

    // Allocate memory for the padded string
    char *ret = allocator_alloc(alloc, sizeof(char) * (total_len + 1));
    if (ret == NULL)
    {
        return NULL;
    }

    // Fill the left side with padding characters
    for (u32 i = 0; i < pad_count; i++)
    {
        ret[i] = pad_char;
    }

    // Copy the original string after the padding
    memcpy(ret + pad_count, s, s_len);
    ret[total_len] = '\0';

    return ret;
}

char *right_pad(allocator *alloc, const char *s, char pad_char, u32 total_len)
{
    size_t s_len = strlen(s);

    // If the string is already longer than or equal to total_len, return a copy
    if (s_len >= total_len)
    {
        char *ret = allocator_alloc(alloc, sizeof(char) * (s_len + 1));
        if (ret == NULL)
        {
            return NULL;
        }
        memcpy(ret, s, s_len + 1);
        return ret;
    }

    // Calculate how many padding characters we need
    u32 pad_count = total_len - s_len;

    // Allocate memory for the padded string
    char *ret = allocator_alloc(alloc, sizeof(char) * (total_len + 1));
    if (ret == NULL)
    {
        return NULL;
    }

    // Copy the original string first
    memcpy(ret, s, s_len);

    // Fill the right side with padding characters
    for (u32 i = 0; i < pad_count; i++)
    {
        ret[s_len + i] = pad_char;
    }

    ret[total_len] = '\0';

    return ret;
}

char *substr(allocator *alloc, const char *s, u32 start, u32 end)
{
    size_t s_len = strlen(s);

    // Clamp start and end to valid bounds
    if (start >= s_len)
        start = s_len;
    if (end > s_len)
        end = s_len;

    // If start is after end, return empty string
    if (start >= end)
    {
        char *ret = allocator_alloc(alloc, sizeof(char));
        if (ret == NULL)
        {
            return NULL;
        }
        ret[0] = '\0';
        return ret;
    }

    // Calculate substring length
    u32 len = end - start;

    // Allocate memory for the substring
    char *ret = allocator_alloc(alloc, sizeof(char) * (len + 1));
    if (ret == NULL)
    {
        return NULL;
    }

    // Copy the substring
    memcpy(ret, s + start, len);
    ret[len] = '\0';

    return ret;
}

int indexof(const char *s, char c)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == c)
        {
            return i;
        }
    }
    return -1;
}

bool contains(const char *s, char c)
{
    return indexof(s, c) != -1;
}

bool starts_with(const char *s, char c)
{
    return s[0] == c;
}

bool ends_with(const char *s, char c)
{
    while (*s != '\0')
    {
        s++;
    }
    s--;
    return *s == c;
}

bool eq_ignore_case(const char *a, const char *b)
{
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        char ca = a[i];
        char cb = b[i];

        // Convert to lowercase for comparison
        if (ca >= 'A' && ca <= 'Z')
        {
            ca = ca + ('a' - 'A');
        }
        if (cb >= 'A' && cb <= 'Z')
        {
            cb = cb + ('a' - 'A');
        }

        if (ca != cb)
        {
            return 0;
        }

        i++;
    }

    // Both strings must be at the end to be equal
    return a[i] == '\0' && b[i] == '\0';
}

str_split split(allocator *alloc, const char *s, const char *pat)
{
    str_split ret = {0};

    if (s == NULL || pat == NULL || *pat == '\0')
    {
        return ret;
    }

    size_t pat_len = strlen(pat);

    // First pass: count how many parts we'll have
    int count = 1; // At least one part
    const char *p = s;
    while ((p = strstr(p, pat)) != NULL)
    {
        count++;
        p += pat_len;
    }

    // Allocate the array of string pointers
    ret.parts = allocator_alloc(alloc, sizeof(char *) * count);
    if (ret.parts == NULL)
    {
        return ret;
    }
    ret.num_parts = count;

    // Second pass: extract each part
    const char *start = s;
    int index = 0;

    while (index < count - 1)
    {
        char *end = strstr(start, pat);
        if (end == NULL)
        {
            break;
        }

        size_t len = end - start;
        ret.parts[index] = allocator_alloc(alloc, sizeof(char) * (len + 1));
        if (ret.parts[index] == NULL)
        {
            ret.num_parts = 0;
            return ret;
        }

        memcpy(ret.parts[index], start, len);
        ret.parts[index][len] = '\0';

        start = end + pat_len;
        index++;
    }

    // Last part (everything remaining)
    size_t len = strlen(start);
    ret.parts[index] = allocator_alloc(alloc, sizeof(char) * (len + 1));
    if (ret.parts[index] == NULL)
    {
        ret.num_parts = 0;
        return ret;
    }

    memcpy(ret.parts[index], start, len + 1);

    return ret;
}
