#include "string_view.h"


string_view string_view_create(char const *data, uint32 size)
{
    string_view result = {};
    result.data = data;
    result.size = size;
    return result;
}

string_view string_view_create_from_cstring(char const *data)
{
    string_view result = {};
    result.data = data;
    result.size = cstring_size_no0(data);
    return result;
}

string_view string_view_split_left(string_view *sv, char sep)
{
    string_view result = {};
    uint32 i;
    for (i = 0; i < sv->size; i++)
    {
        if (sv->data[i] == sep)
        {
            result.data = sv->data;
            result.size = i;
            sv->data += (i + 1);
            sv->size -= (i + 1);
            break;
        }
    }
    return result;
}

string_view string_view_split_right(string_view *sv, char sep)
{
    string_view result = {};
    uint32 i;
    for (i = 0; i < sv->size; i++)
    {
        if (sv->data[sv->size - i - 1] == sep)
        {
            result.data = sv->data + sv->size - i;
            result.size = i;
            sv->size -= (i + 1);
            break;
        }
    }
    return result;
}
