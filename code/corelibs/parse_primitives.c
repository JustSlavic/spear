#include "parse_primitives.h"


int spear_parse_while(char const *data, uint32 size, int (*predicate)(char))
{
    char c;
    int index = 0;
    while (index < size)
    {
        c = data[index];
        if (predicate(c))
            index += 1;
        else
            break;
    }
    return index;
}

int spear_parse_until(char const *data, uint32 size, int (*predicate)(char))
{
    char c;
    int index = 0;
    while (index < size)
    {
        c = data[index];
        if (!predicate(c))
            index += 1;
        else
            break;
    }
    return index;
}

int spear_parse_cstring(char const *data, uint32 size, char const *cstr)
{
    int index = 0;
    while (index < size)
    {
        char c1 = data[index];
        char c2 = cstr[index];
        if (c2 == 0 || c1 != c2) break;
        index += 1;
    }
    return index;
}

int spear_parse_int32(char const *data, uint32 size, int32 *out)
{
    char c = 0;
    uint32 index = 0;
    int32 sign = 1;
    int32 integer = 0;

    if (index < size)
    {
        c = data[index];
        if (c == '+')
        {
            index += 1;
            if (index < size)
            {
                c = data[index];
                if (!is_ascii_digit(c))
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else if (c == '-')
        {
            sign = -1;
            index += 1;
            if (index < size)
            {
                c = data[index];
                if (!is_ascii_digit(c))
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
    }

    while (index < size)
    {
        c = data[index];
        if (!is_ascii_digit(c)) break;
        integer *= 10;
        integer += (c - '0');
        index += 1;
    }

    *out = (sign * integer);
    return index;
}

int spear_parse_float32(char const *data, uint32 size, float32 *out)
{
    // @note: this naive algorithm does not account to the 1e-3 form.
    // @note: this implementation uses naive algorithm that has
    // poor accuracy, you might want to replace it with another one.

    char c = 0;
    uint32 index = 0;
    float32 sign = 1;
    float32 integer = 0;
    float32 floating = 0;

    if (index < size)
    {
        c = data[index];
        if (c == '+')
        {
            index += 1;
            if (index < size)
            {
                c = data[index];
                if (!is_ascii_digit(c))
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else if (c == '-')
        {
            sign = -1;
            index += 1;
            if (index < size)
            {
                c = data[index];
                if (!is_ascii_digit(c))
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
    }

    while (index < size)
    {
        c = data[index];
        if (!is_ascii_digit(c)) break;
        integer *= 10;
        integer += (c - '0');
        index += 1;
    }

    if (c == '.')
    {
        float32 mult = 0.1f;
        index += 1;
        while (index < size)
        {
            c = data[index];
            if (!is_ascii_digit(c)) break;
            floating += (mult * (c - '0'));
            mult *= 0.1f;
            index += 1;
        }
    }

    *out = (sign * (integer + floating));
    return index;
}
