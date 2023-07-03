#ifndef STRING_ID_HPP
#define STRING_ID_HPP

#include <base.hpp>
#include <memory/allocator.hpp>


struct string_id_storage;
struct string_id
{
    uint32 id;
};

bool32 operator == (string_id id1, string_id id2)
{
    bool32 result = (id1.id == id2.id);
    return result;
}

bool32 operator != (string_id id1, string_id id2)
{
    bool32 result = (id1.id != id2.id);
    return result;
}


string_id_storage *initialize_string_id_storage(memory_block block);
string_id make_string_id(string_id_storage *storage, char const *string, usize size);
string_id make_string_id(string_id_storage *storage, char const *cstring);
char const *get_cstring_by_id(string_id_storage *storage, string_id id);
FORCE_INLINE uint64 hash(string_id id) { return id.id; }

#define STRID(CSTR) make_string_id(context->strid_storage, CSTR)


#endif // STRING_ID_HPP
