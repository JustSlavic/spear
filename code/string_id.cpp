#ifndef STRING_ID_CPP
#define STRING_ID_CPP

#include "string_id.hpp"
#include <string.hpp>


struct string_id_storage
{
    memory_block memory;
    memory_allocator arena;

    #define HASH_TABLE_SIZE 1024
    uint32 hash_table_hashes[HASH_TABLE_SIZE];
    uint32 hash_table_offsets[HASH_TABLE_SIZE];
    #undef HASH_TABLE_SIZE
};

string_id_storage *initialize_string_id_storage(memory_block block)
{
    memory_allocator arena = memory_allocator__create_arena_from_memory_block(block);

    string_id_storage *storage = ALLOCATE(arena, string_id_storage);
    storage->arena = arena;
    storage->memory = block;

    return storage;
}

int32 get_index_in_hash_table(string_id_storage *storage, uint32 hash)
{
    int32 result = -1;
    for (uint32 offset = 0; offset < ARRAY_COUNT(storage->hash_table_hashes); offset++)
    {
        int32 index = (hash + offset) % ARRAY_COUNT(storage->hash_table_hashes);
        uint32 slot = storage->hash_table_hashes[index];
        if (slot == hash || slot == 0)
        {
            result = index;
            break;
        }
    }
    return result;
}

GLOBAL uint32 primes[32] = {
      2,   3,  5,  7, 11,  13,  17,  19,  23,  29,
     31,  37, 41, 43, 47,  53,  59,  61,  67,  71,
     73,  79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131,
};

uint32 make_string_id_hash(char const *buffer, usize size)
{
    uint32 hash = 0;
    for (uint32 index = 0; index < size; index++)
    {
        hash += buffer[index] * primes[index % ARRAY_COUNT(primes)];
    }
    return hash;
}

string_id make_string_id(string_id_storage *storage, char const *cstr, usize size)
{
    string_id result;

    uint32 hash = make_string_id_hash(cstr, size);
    uint32 index = get_index_in_hash_table(storage, hash);

    if (storage->hash_table_offsets[index] == 0)
    {
        memory_block buffer = ALLOCATE_BUFFER_ALIGNED_(storage->arena, (size + 1) + sizeof(usize), alignof(usize));
        usize *size_in_buffer  = (usize *) buffer.memory;
        char *string_in_buffer = (char *)  buffer.memory + sizeof(usize);

        *size_in_buffer = size;
        memory__copy(string_in_buffer, cstr, size);
        string_in_buffer[size] = 0; // null-terminator just in case I have to pass this buffer to a function, which expects c-string

        uint32 offset = truncate_to_uint32((intptr) string_in_buffer - (intptr) storage->memory.memory);
        storage->hash_table_offsets[index] = offset;
        result.id = offset;
    }
    else
    {
        result.id = storage->hash_table_offsets[index];
    }

    return result;
}

string_id make_string_id(string_id_storage *storage, char const *cstr)
{
    usize size = size_no0(cstr);
    string_id result = make_string_id(storage, cstr, size);
    return result;
}

char const *get_cstring_by_id(string_id_storage *storage, string_id id)
{
    char const *result = (char const *) storage->memory.memory + id.id;
    return result;
}

#endif // STRING_ID_CPP
