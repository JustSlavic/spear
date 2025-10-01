#ifndef SPEAR_CORELIBS_OBJ_H
#define SPEAR_CORELIBS_OBJ_H

#include <corelibs/base.h>


typedef enum
{
    ObjDecode_Success = 0,

    ObjDecode_VertexBufferOverflow = -1,
    ObjDecode_IndexBufferOverflow = -2,
} obj_decode_result;

char const *obj_decode_result_to_cstring(obj_decode_result result);
obj_decode_result
obj_extract_size(void *file_data,
                 usize file_size,
                 uint32 *out_vertex_buffer_size,
                 uint32 *out_index_buffer_size);
obj_decode_result
obj_decode(void *file_data,
           usize file_size,
           void *vertex_data,
           uint32 vertex_size,
           void *index_data,
           uint32 index_size);


#endif /* SPEAR_CORELIBS_OBJ_H */
