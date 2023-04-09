#ifndef STRING_ID_HPP
#define STRING_ID_HPP

#include <base.hpp>


struct string_id_storage_hash_entry
{
    uint32 *hash;
    uint32 *offset;
};

struct string_id_storage
{
    memory::allocator arena;

    #define HASH_TABLE_SIZE 1024
    uint32 hash_table_hashes[HASH_TABLE_SIZE];
    uint32 hash_table_offsets[HASH_TABLE_SIZE];
    #undef HASH_TABLE_SIZE
};

struct string_id
{
    uint32 id;
};

bool32 operator == (string_id id1, string_id id2)
{
    bool32 result = (id1.id == id2.id);
    return result;
}


string_id make_string_id(string_id_storage *, char const *string, usize size);
string_id make_string_id(string_id_storage *, char const *string);
char const *get_cstring_by_id(string_id_storage *, string_id id);

#define STRID(CSTR) make_string_id(&context->strid_storage, CSTR)


#endif // STRING_ID_HPP
