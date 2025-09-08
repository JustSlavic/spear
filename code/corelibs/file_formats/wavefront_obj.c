#include "wavefront_obj.h"


wavefront_obj wavefront_obj_parse(memory_allocator allocator, void *data, usize size)
{
    char *buffer = (char *) data;

    usize index = 0;
    while (index < size)
    {
        char c = *buffer++;
        if (c == '\r' || c == '\n')
            break;

        printf("%c", c);
        index += 1;
    }

    printf("\n");

    return (wavefront_obj){};
}
