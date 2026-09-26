#ifndef SPEAR_BASE_STRING_VIEW_H
#define SPEAR_BASE_STRING_VIEW_H

#include "base.h"


typedef struct {
    char const *data;
    uint32 size;
} string_view;

string_view string_view_create(char const *data, uint32 size);
string_view string_view_create_from_cstring(char const *data);
string_view string_view_split_left(string_view *sv, char sep);
string_view string_view_split_right(string_view *sv, char sep);


#endif // SPEAR_BASE_STRING_VIEW_H
