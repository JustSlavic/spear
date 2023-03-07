#ifndef STRING_HPP
#define STRING_HPP

#include <base.hpp>


usize size_no0(const char *str)
{
    usize size = 0;
    while (str[size]) { size += 1; }
    return size;
}

usize size_with0(const char *s)
{
    usize size = size_no0(s) + 1;
    return size;
}


struct string_view
{
    char *data = NULL;
    usize size = 0;

    string_view() = default;

    string_view(char const *c_string)
    {
        data = (char *) c_string;
        size = size_no0(c_string);
    }
};


#endif // STRING_HPP
