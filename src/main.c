#include <stdio.h>

#include "alloc.h"
#include "hashtable.h"
#include "strings.h"

int main()
{
    printf("Hello\n");

    allocator *alloc = allocator_create(1024);

    char *s = concat(alloc, 3, "Hello", " World!", " My Dear!");
    printf("%s\n", s);

    printf("`%s`\n", trim_right(alloc, "hi"));
    printf("`%s`\n", trim_right(alloc, "hi   "));
    printf("`%s`\n", trim_right(alloc, "   hi"));
    printf("`%s`\n", trim_right(alloc, "   hi   "));

    printf("`%s`\n", left_pad(alloc, "hello", '-', 9));
    printf("`%s`\n", right_pad(alloc, "hello", '-', 11));

    printf("`%s`\n", substr(alloc, "hello", 1, 9));

    printf("`%d`\n", indexof("hello", 'l'));

    printf("`%d`\n", starts_with("hello", 'h'));
    printf("`%d`\n", starts_with("hello", 'l'));

    printf("`%d`\n", ends_with("hello", 'o'));
    printf("`%d`\n", ends_with("hello", 'l'));

    printf("`%d`\n", eq_ignore_case("hello", "HeLlO"));
    printf("`%d`\n", eq_ignore_case("foo", "bar"));

    str_split spl = split(alloc, "Hello my name is John.", " ");
    for (int i = 0; i < spl.num_parts; i++)
    {
        printf("part[%d]: `%s`\n", i, spl.parts[i]);
    }

    allocator_destroy(alloc);

    printf("Goodbye\n");

    return 0;
}
