#ifndef SPEAR_CORELIBS_FILE_FORMATS_PNG_H
#define SPEAR_CORELIBS_FILE_FORMATS_PNG_H

#include <corelibs/base.h>


typedef enum
{
    PngDecode_Success = 0,
    PngDecode_UnexpectedEof = -1,
    PngDecode_HeaderSignatureMismatch = -2,
    PngDecode_IHDR_HeaderIsNotFirst = -3,
    PngDecode_UnsupportedCompressionMethod = -4,
    PngDecode_UnsupportedInterlacedMethod = -5,
    PngDecode_UnsupportedColorType = -6,
    PngDecode_UnrecognizedChunk = -7,
    PngDecode_CrcDoesNotMatch = -8,
    PngDecode_ImageSizeMismatch = -9,
    PngDecode_ZlibFailed = -10,
    PngDecode_UnknownFilterMethod = -11,
} png_decode_result;

char const *png_decode_result_to_cstring(png_decode_result result);
png_decode_result png_check_signature(void *file_data, usize file_size);
png_decode_result png_extract_size(void *file_data, usize file_size, uint32 *out_image_size);
png_decode_result png_decode(void *file_data, usize file_size,
                             void *image_data, usize image_size,
                             uint32 *out_width,
                             uint32 *out_height,
                             uint32 *out_bits_per_pixel,
                             uint32 *out_color_mode,
                             bool32 *out_is_top_down);


#endif // SPEAR_CORELIBS_FILE_FORMATS_PNG_H
