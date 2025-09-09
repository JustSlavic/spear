#ifndef SPEAR_CORELIBS_FILE_FORMATS_H
#define SPEAR_CORELIBS_FILE_FORMATS_H

#include <corelibs/base.h>


typedef enum
{
    BmpDecode_Success = 0,
    BmpDecode_UnexpectedEof = -1,
    BmpDecode_HeaderSignatureMismatch = -2,
    BmpDecode_HeaderReservedBytesNotZero = -3,
    BmpDecode_InfoHeaderSizeNotForty = -4,
    BmpDecode_InfoHeaderPlanesNotOne = -5,
} bmp_decode_result;

char const *bmp_decode_result_to_cstring(bmp_decode_result result);
bmp_decode_result bmp_extract_size(void *file_data, usize file_size, usize *out_size);
bmp_decode_result bmp_decode(void *filt_data, usize file_size, void *out_memory, usize out_size, uint32 *out_width, uint32 *out_height, uint32 *bits_per_pixel, uint32 *out_color_mode, bool32 *is_top_down);


#endif // SPEAR_CORELIBS_FILE_FORMATS_H
