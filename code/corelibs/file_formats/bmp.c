#include "bmp.h"
#include <corelibs/bitmap.h>


enum
{
    BI_RGB  = 0, // No compression
    BI_RLE8 = 1, // 8bit RLE encoding
    BI_RLE4 = 2, // 4bit RLE encoding
};

char const *bmp_decode_result_to_cstring(bmp_decode_result result)
{
    switch (result)
    {
        case BmpDecode_Success: return "BmpDecode_Success";
        case BmpDecode_UnexpectedEof: return "BmpDecode_UnexpectedEof";
        case BmpDecode_HeaderSignatureMismatch: return "BmpDecode_HeaderSignatureMismatch";
        case BmpDecode_HeaderReservedBytesNotZero: return "BmpDecode_HeaderReservedBytesNotZero";
        case BmpDecode_InfoHeaderSizeNotForty: return "BmpDecode_InfoHeaderSizeNotForty";
        case BmpDecode_InfoHeaderPlanesNotOne: return "BmpDecode_InfoHeaderPlanesNotOne";
    }
}

#define BMP_READ(Type, Variable)                \
    Type Variable = 0;                          \
    if (index + sizeof(Type) <= file_size) {    \
        Variable = *(Type *) (data + index);    \
        index += sizeof(Type);                  \
    } else {                                    \
        return BmpDecode_UnexpectedEof;         \
    }                                           \

bmp_decode_result bmp_extract_size(void *file_data, uint32 file_size, uint32 *image_size)
{
    uint8 *data = (uint8 *) file_data;
    uint32 index = 0;

    BMP_READ(uint16, header_signature);
    if (header_signature != 0x4d42) // Signature should be "BM"
    {
        return BmpDecode_HeaderSignatureMismatch;
    }
    BMP_READ(uint32, header_filesize);
    UNUSED(header_filesize);
    BMP_READ(uint32, header_reserved);
    if (header_reserved != 0)
    {
        return BmpDecode_HeaderReservedBytesNotZero;
    }
    BMP_READ(uint32, header_data_offset);
    UNUSED(header_data_offset);
    BMP_READ(uint32, info_header_size); // Size of info header (always 40)
    if (info_header_size != 40)
    {
        return BmpDecode_InfoHeaderSizeNotForty;
    }
    BMP_READ(uint32, info_header_width);
    BMP_READ(uint32, info_header_height);
    BMP_READ(uint16, info_header_planes);
    if (info_header_planes != 1)
    {
        return BmpDecode_InfoHeaderPlanesNotOne;
    }
    BMP_READ(uint16, info_header_bits_per_pixel);
    BMP_READ(uint32, info_header_compression);
    BMP_READ(uint32, info_header_image_size); // Can be 0 if compression is BI_RGB

    uint32 compited_image_size = info_header_image_size;
    if (info_header_image_size == 0 && info_header_compression == BI_RGB)
    {
        compited_image_size = info_header_width * info_header_height * info_header_bits_per_pixel / 8;
    }
    *image_size = compited_image_size;

    return BmpDecode_Success;
}

bmp_decode_result bmp_decode(void *file_data, uint32 file_size, void *image_data, uint32 image_size, uint32 *out_width, uint32 *out_height, uint32 *bits_per_pixel, uint32 *out_color_mode, bool32 *is_top_down)
{
    uint8 *data = (uint8 *) file_data;
    uint32 index = 0;

    BMP_READ(uint16, header_signature);
    if (header_signature != 0x4d42) // Signature should be "BM"
    {
        return BmpDecode_HeaderSignatureMismatch;
    }
    BMP_READ(uint32, header_filesize);
    UNUSED(header_filesize);
    BMP_READ(uint32, header_reserved);
    if (header_reserved != 0)
    {
        return BmpDecode_HeaderReservedBytesNotZero;
    }
    BMP_READ(uint32, header_data_offset);
    BMP_READ(uint32, info_header_size); // Size of info header (always 40)
    if (info_header_size != 40)
    {
        return BmpDecode_InfoHeaderSizeNotForty;
    }
    BMP_READ(uint32, info_header_width);
    BMP_READ(uint32, info_header_height);
    BMP_READ(uint16, info_header_planes);
    if (info_header_planes != 1)
    {
        return BmpDecode_InfoHeaderPlanesNotOne;
    }
    BMP_READ(uint16, info_header_bits_per_pixel);
    BMP_READ(uint32, info_header_compression);
    UNUSED(info_header_compression);
    BMP_READ(uint32, info_header_image_size); // Can be 0 if compression is BI_RGB
    BMP_READ(uint32, info_header_x_pixels_per_meter);
    UNUSED(info_header_x_pixels_per_meter);
    BMP_READ(uint32, info_header_y_pixels_per_meter);
    UNUSED(info_header_y_pixels_per_meter);
    BMP_READ(uint32, info_header_color_count); // For 8bit per pixel bitmap it will be equal to 256
    UNUSED(info_header_color_count);
    BMP_READ(uint32, info_header_important_color_count); // 0 -> All colors are important
    UNUSED(info_header_important_color_count);

    if (header_data_offset + info_header_image_size <= file_size)
    {
        memcpy(image_data, data + header_data_offset, info_header_image_size);
    }
    else
    {
        return BmpDecode_UnexpectedEof;
    }

    if (out_width) *out_width = info_header_width;
    if (out_height) *out_height = info_header_height;
    if (bits_per_pixel) *bits_per_pixel = info_header_bits_per_pixel;
    if (out_color_mode) *out_color_mode = Bitmap_BGR;
    if (is_top_down) *is_top_down = false;

    return BmpDecode_Success;
}

#undef BMP_READ
