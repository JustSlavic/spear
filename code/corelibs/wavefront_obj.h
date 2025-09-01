#ifndef SPEAR_CORELIBS_WAVEFRONT_OBJ_H
#define SPEAR_CORELIBS_WAVEFRONT_OBJ_H

#include <corelibs/base.h>
#include <corelibs/math.h>
#include <corelibs/memory/allocator.h>


typedef struct
{
    vector3 *vertices;
    usize vertex_count;
} wavefront_obj;


wavefront_obj wavefront_obj_parse(memory_allocator allocator, void *data, usize size);


#endif /* SPEAR_CORELIBS_WAVEFRONT_OBJ_H */
