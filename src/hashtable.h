#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "base.h"

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

#define DEF_HASHTABLE_NAMED(ht_name, ht_entry_name, k, v)                         \
    ht_name *ht_name##_create(size_t capacity)                                    \
    {                                                                             \
        ht_name *table = malloc(sizeof(ht_name));                                 \
        if (table == NULL)                                                        \
        {                                                                         \
            return NULL;                                                          \
        }                                                                         \
        table->length = 0;                                                        \
        table->capacity = capacity;                                               \
                                                                                  \
        table->entries = calloc(capacity, sizeof(ht_entry_name));                 \
        if (table->entries == NULL)                                               \
        {                                                                         \
            free(table);                                                          \
            return NULL;                                                          \
        }                                                                         \
        for (size_t i = 0; i < table->capacity; i++)                              \
        {                                                                         \
            table->entries[i].is_empty = true;                                    \
        }                                                                         \
                                                                                  \
        return table;                                                             \
    }                                                                             \
                                                                                  \
    void ht_name##_destroy(ht_name *table)                                        \
    {                                                                             \
        free(table->entries);                                                     \
        free(table);                                                              \
    }                                                                             \
                                                                                  \
    static u64 ht_name##_hash_key(k key)                                          \
    {                                                                             \
        u64 hash = FNV_OFFSET;                                                    \
        unsigned char *p = (unsigned char *)&key;                                 \
        for (int i = 0; i < sizeof(u64); i++)                                     \
        {                                                                         \
            hash ^= (u64)p[i];                                                    \
            hash *= FNV_PRIME;                                                    \
        }                                                                         \
        return hash;                                                              \
    }                                                                             \
                                                                                  \
    bool ht_name##_get(ht_name *table, k key, v *out)                             \
    {                                                                             \
        u64 hash = ht_name##_hash_key(key);                                       \
        size_t index = (size_t)(hash & (u64)(table->capacity - 1));               \
                                                                                  \
        while (!table->entries[index].is_empty)                                   \
        {                                                                         \
            if (key == table->entries[index].key)                                 \
            {                                                                     \
                *out = table->entries[index].value;                               \
                return true;                                                      \
            }                                                                     \
            index++;                                                              \
            if (index >= table->capacity)                                         \
            {                                                                     \
                index = 0;                                                        \
            }                                                                     \
        }                                                                         \
                                                                                  \
        return false;                                                             \
    }                                                                             \
                                                                                  \
    void ht_name##_set_entry(                                                     \
        ht_entry_name *entries, size_t capacity,                                  \
        k key, v value, size_t *plength)                                          \
    {                                                                             \
        u64 hash = ht_name##_hash_key(key);                                       \
        size_t index = (size_t)(hash & (u64)(capacity - 1));                      \
                                                                                  \
        while (!entries[index].is_empty)                                          \
        {                                                                         \
            if (key == entries[index].key)                                        \
            {                                                                     \
                entries[index].value = value;                                     \
                return;                                                           \
            }                                                                     \
            index++;                                                              \
            if (index >= capacity)                                                \
            {                                                                     \
                index = 0;                                                        \
            }                                                                     \
        }                                                                         \
                                                                                  \
        if (plength != NULL)                                                      \
        {                                                                         \
            (*plength)++;                                                         \
        }                                                                         \
        entries[index].key = key;                                                 \
        entries[index].value = value;                                             \
        entries[index].is_empty = false;                                          \
    }                                                                             \
                                                                                  \
    static bool ht_name##_expand(ht_name *table)                                  \
    {                                                                             \
        size_t new_capacity = table->capacity * 2;                                \
        if (new_capacity < table->capacity)                                       \
        {                                                                         \
            return false;                                                         \
        }                                                                         \
        ht_entry_name *new_entries = calloc(new_capacity, sizeof(ht_entry_name)); \
        if (new_entries == NULL)                                                  \
        {                                                                         \
            return false;                                                         \
        }                                                                         \
                                                                                  \
        for (size_t i = 0; i < new_capacity; i++)                                 \
        {                                                                         \
            new_entries[i].is_empty = true;                                       \
        }                                                                         \
                                                                                  \
        for (size_t i = 0; i < table->capacity; i++)                              \
        {                                                                         \
            ht_entry_name entry = table->entries[i];                              \
            if (!entry.is_empty)                                                  \
            {                                                                     \
                ht_name##_set_entry(                                              \
                    new_entries, new_capacity,                                    \
                    entry.key, entry.value, NULL);                                \
            }                                                                     \
        }                                                                         \
                                                                                  \
        free(table->entries);                                                     \
        table->entries = new_entries;                                             \
        table->capacity = new_capacity;                                           \
        return true;                                                              \
    }                                                                             \
                                                                                  \
    bool ht_name##_set(ht_name *table, u64 key, u64 value)                        \
    {                                                                             \
        if (table->length >= table->capacity / 2)                                 \
        {                                                                         \
            if (!ht_name##_expand(table))                                         \
            {                                                                     \
                return false;                                                     \
            }                                                                     \
        }                                                                         \
        ht_name##_set_entry(                                                      \
            table->entries, table->capacity,                                      \
            key, value, &table->length);                                          \
        return true;                                                              \
    }                                                                             \
                                                                                  \
    void ht_name##_delete(ht_name *table, k key)                                  \
    {                                                                             \
        u64 hash = ht_name##_hash_key(key);                                       \
        size_t index = (size_t)(hash & (u64)(table->capacity - 1));               \
                                                                                  \
        while (!table->entries[index].is_empty)                                   \
        {                                                                         \
            if (key == table->entries[index].key)                                 \
            {                                                                     \
                table->entries[index].is_empty = true;                            \
                return;                                                           \
            }                                                                     \
            index++;                                                              \
            if (index >= table->capacity)                                         \
            {                                                                     \
                index = 0;                                                        \
            }                                                                     \
        }                                                                         \
    }

#define DEF_HASHTABLE(k, v) DEF_HASHTABLE_NAMED(ht_##k##_##v, ht_##k##_##v##_entry, k, v)

#define DEF_HASHTABLE_H_NAMED(ht_name, ht_entry_name, k, v) \
    typedef struct ht_entry_name                            \
    {                                                       \
        k key;                                              \
        v value;                                            \
        bool is_empty;                                      \
    } ht_entry_name;                                        \
                                                            \
    typedef struct ht_name                                  \
    {                                                       \
        ht_entry_name *entries;                             \
        size_t capacity;                                    \
        size_t length;                                      \
    } ht_name;                                              \
                                                            \
    ht_name *ht_name##_create(size_t capacity);             \
                                                            \
    void ht_name##_destroy(ht_name *table);                 \
    bool ht_name##_get(ht_name *table, k key, v *out);      \
                                                            \
    void ht_name##_set_entry(                               \
        ht_entry_name *entries, size_t capacity,            \
        k key, v value, size_t *plength);                   \
                                                            \
    bool ht_name##_set(ht_name *table, u64 key, u64 value); \
                                                            \
    void ht_name##_delete(ht_name *table, k key);

#define DEF_HASHTABLE_H(k, v) DEF_HASHTABLE_H_NAMED(ht_##k##_##v, ht_##k##_##v##_entry, k, v)

#ifndef MODE_TEST

DEF_HASHTABLE_H(u64, u64);
DEF_HASHTABLE_H(u64, size_t);

#endif

#endif
