#include "png.h"
#include <corelibs/crc.h>

/*
    PNG (Portable Network Graphics) Specification, Version 1.2
      http://www.libpng.org/pub/png/spec/1.2/PNG-Contents.html

    ZLIB Compressed Data Format Specification version 3.3
      https://www.ietf.org/rfc/rfc1950.txt

    DEFLATE Compressed Data Format Specification version 1.3:
      https://www.ietf.org/rfc/rfc1951.txt
*/

#define PNG_MAGIC_NUMBER(a, b, c, d) ((((uint32)a) << 0) | (((uint32)b) << 8) | (((uint32)c) << 16) | (((uint32)d) << 24))
enum {
    PNG_SIGN_1  = PNG_MAGIC_NUMBER(137, 80, 78, 71),
    PNG_SIGN_2  = PNG_MAGIC_NUMBER(13, 10, 26, 10),
    PNG_IHDR_ID = PNG_MAGIC_NUMBER('I', 'H', 'D', 'R'),
    PNG_sRGB_ID = PNG_MAGIC_NUMBER('s', 'R', 'G', 'B'),
    PNG_IDAT_ID = PNG_MAGIC_NUMBER('I', 'D', 'A', 'T'),
    PNG_IEND_ID = PNG_MAGIC_NUMBER('I', 'E', 'N', 'D'),
    PNG_tRNS_ID = PNG_MAGIC_NUMBER('t', 'R', 'N', 'S'),
    PNG_gAMA_ID = PNG_MAGIC_NUMBER('g', 'A', 'M', 'A'),
    PNG_cHRM_ID = PNG_MAGIC_NUMBER('c', 'H', 'R', 'M'),
    PNG_iCCP_ID = PNG_MAGIC_NUMBER('i', 'C', 'C', 'P'),
    PNG_iTXt_ID = PNG_MAGIC_NUMBER('i', 'T', 'X', 't'),
    PNG_tEXt_ID = PNG_MAGIC_NUMBER('t', 'E', 'X', 't'),
    PNG_zTXt_ID = PNG_MAGIC_NUMBER('z', 'T', 'X', 't'),
    PNG_bKGD_ID = PNG_MAGIC_NUMBER('b', 'K', 'G', 'D'),
    PNG_pHYs_ID = PNG_MAGIC_NUMBER('p', 'H', 'Y', 's'),
    PNG_sBIT_ID = PNG_MAGIC_NUMBER('s', 'B', 'I', 'T'),
    PNG_sPLT_ID = PNG_MAGIC_NUMBER('s', 'P', 'L', 'T'),
    PNG_hIST_ID = PNG_MAGIC_NUMBER('h', 'I', 'S', 'T'),
    PNG_tIME_ID = PNG_MAGIC_NUMBER('t', 'I', 'M', 'E'),
};
#undef PNG_MAGIC_NUMBER


enum
{
    PNG_GRAYSCALE_USED     = 0x0,
    PNG_PALETTE_USED       = 0x1,
    PNG_COLOR_USED         = 0x2,
    PNG_ALPHA_CHANNEL_USED = 0x4,
};

enum
{
    PNG_INTERLACE_NONE  = 0,
    PNG_INTERLACE_ADAM7 = 1,
};

png_decode_result png_check_signature(void *file_data, usize file_size)
{
    uint8 *data = (uint8 *) file_data;

    if (file_size < 8)
        return PngDecode_UnexpectedEof;

    uint32 signature1 = *(uint32 *) (data);
    uint32 signature2 = *(uint32 *) (data + 4);
    return (signature1 == PNG_SIGN_1 && signature2 == PNG_SIGN_2)
        ? PngDecode_Success
        : PngDecode_HeaderSignatureMismatch;
}

png_decode_result png_extract_size(void *file_data, usize file_size, uint32 *out_image_size)
{
    uint8 *data = (uint8 *) file_data;
    uint32 index = 0;

    // Signature
    uint32 signature1 = 0;
    uint32 signature2 = 0;
    if (index + 8 <= file_size)
    {
        signature1 = *(uint32 *) (data + (index));
        signature2 = *(uint32 *) (data + (index + 4));
        index += 8;
    }
    else
    {
        return PngDecode_UnexpectedEof;
    }

    if (signature1 != PNG_SIGN_1 || signature2 != PNG_SIGN_2)
    {
        return PngDecode_HeaderSignatureMismatch;
    }

    // IHDR
    uint32 width;
    uint32 height;
    uint8  bit_depth;
    uint8  color_type;
    uint8  compression_method;
    // uint8  filter_method;
    uint8  interlace_method;

    while (index < file_size)
    {
        /*
            Chunks of PNG file follow this layout:

            +-------------------+-------------+----------------+
            |  PNG_ChunkHeader  |  Data  ...  |  PNG_ChunkCRC  |
            +-------------------+-------------+----------------+
            |      8 bytes      |   N bytes   |    4  bytes    |
            +-------------------+-------------+----------------+
        */

        // Chunk header
        uint32 chunk_size;
        uint32 chunk_type;
        if (index + 8 <= file_size)
        {
            chunk_size = *(uint32 *) (data + (index));
            chunk_size = uint32_change_endianness(chunk_size);
            chunk_type = *(uint32 *) (data + (index + 4));
            index += 8;
        }
        else
        {
            return PngDecode_UnexpectedEof;
        }

        if (chunk_type == PNG_IHDR_ID)
        {
            if (index + 13 <= file_size)
            {
                width              = *(uint32 *) (data + (index));
                width              = uint32_change_endianness(width);
                height             = *(uint32 *) (data + (index + 4));
                height             = uint32_change_endianness(height);
                bit_depth          = *(uint8  *) (data + (index + 8));
                color_type         = *(uint8  *) (data + (index + 9));
                compression_method = *(uint8  *) (data + (index + 10));
                // filter_method      = *(uint8  *) (data + (index + 11));
                interlace_method   = *(uint8  *) (data + (index + 12));
                index += 13;
            }
            else
            {
                return PngDecode_UnexpectedEof;
            }

            // @note: Only supported compression method 0
            //        (deflate/inflate compression with a sliding window of at most 32768 bytes) is defined.
            //        All standard PNG images must be compressed with this scheme.
            if (compression_method != 0)
            {
                return PngDecode_UnsupportedCompressionMethod;
            }
            // @note: Adam7 interlace method is not supported.
            if (interlace_method != PNG_INTERLACE_NONE)
            {
                return PngDecode_UnsupportedInterlacedMethod;
            }

            if (color_type == PNG_GRAYSCALE_USED)
            {
                // Each pixel is a grayscale sample.
                // Allowed bit depths: 1, 2, 4, 8, 16 bits per pixel
                uint32 bits_per_pixel = bit_depth;
                *out_image_size = (bits_per_pixel * width * height) / 8;
                ASSERT((bits_per_pixel * width * height) % 8 == 0);
                return PngDecode_Success;
            }
            else if (color_type == PNG_COLOR_USED)
            {
                // Each pixel is an RGB triple.
                // Allowed bit depths: 8, 16
                uint32 bits_per_pixel = 3 * bit_depth;
                *out_image_size = (bits_per_pixel * width * height) / 8;
                ASSERT((bits_per_pixel * width * height) % 8 == 0);
                return PngDecode_Success;
            }
            else if (color_type == (PNG_COLOR_USED | PNG_PALETTE_USED))
            {
                // Each pixel is a palette index. A PLTE chunk must appear.
                // Allowed bit depths: 1, 2, 4, 8
                uint32 bits_per_pixel = bit_depth;
                *out_image_size = (bits_per_pixel * width * height) / 8;
                ASSERT((bits_per_pixel * width * height) % 8 == 0);
                return PngDecode_Success;
            }
            else if (color_type == PNG_ALPHA_CHANNEL_USED)
            {
                // Each pixel is a grayscale sample, followed by an alpha sample.
                // Allowed bit depths: 8, 16
                uint32 bits_per_pixel = 2 * bit_depth;
                *out_image_size = (bits_per_pixel * width * height) / 8;
                ASSERT((bits_per_pixel * width * height) % 8 == 0);
                return PngDecode_Success;
            }
            else if (color_type == (PNG_COLOR_USED | PNG_ALPHA_CHANNEL_USED))
            {
                // Each pixel is an RGB triple, followed by an alpha sample.
                // Allowed bit depths: 8, 16
                uint32 bits_per_pixel = 4 * bit_depth;
                *out_image_size = (bits_per_pixel * width * height) / 8;
                ASSERT((bits_per_pixel * width * height) % 8 == 0);
                return PngDecode_Success;
            }
            else
            {
                return PngDecode_UnsupportedColorType;
            }
        }
    }

    return PngDecode_UnexpectedEof;
}

png_decode_result png_decode(void *file_data, usize file_size,
                             void *image_data, usize image_size,
                             uint32 *out_width,
                             uint32 *out_height,
                             uint32 *out_bits_per_pixel,
                             uint32 *out_color_mode,
                             bool32 *out_is_top_down)
{
    uint8 *data = (uint8 *) file_data;
    uint32 index = 0;

    // Signature
    uint32 signature1 = 0;
    uint32 signature2 = 0;
    if (index + 8 <= file_size)
    {
        signature1 = *(uint32 *) (data + (index));
        signature2 = *(uint32 *) (data + (index + 4));
        index += 8;
    }
    else
    {
        return PngDecode_UnexpectedEof;
    }

    if (signature1 != PNG_SIGN_1 || signature2 != PNG_SIGN_2)
    {
        return PngDecode_HeaderSignatureMismatch;
    }

    // IHDR
    uint32 width;
    uint32 height;
    uint8  bit_depth;
    uint8  color_type;
    uint8  compression_method;
    // uint8  filter_method;
    uint8  interlace_method;

    while (index < file_size)
    {
        /*
            Chunks of PNG file follow this layout:

            +-------------------+-------------+----------------+
            |  PNG_ChunkHeader  |  Data  ...  |  PNG_ChunkCRC  |
            +-------------------+-------------+----------------+
            |      8 bytes      |   N bytes   |    4  bytes    |
            +-------------------+-------------+----------------+
        */

        // Chunk header
        uint32 chunk_size;
        uint32 chunk_type;
        uint32 chunk_computed_crc;
        if (index + 8 <= file_size)
        {
            chunk_size = *(uint32 *) (data + (index));
            chunk_size = uint32_change_endianness(chunk_size);
            chunk_type = *(uint32 *) (data + (index + 4));
            chunk_computed_crc = crc_compute(data + (index + 4), chunk_size + sizeof(chunk_type));
            index += 8;
        }
        else
        {
            return PngDecode_UnexpectedEof;
        }

        if (chunk_type == PNG_IHDR_ID)
        {
            if (index + 13 <= file_size)
            {
                width              = *(uint32 *) (data + (index));
                width              = uint32_change_endianness(width);
                height             = *(uint32 *) (data + (index + 4));
                height             = uint32_change_endianness(height);
                bit_depth          = *(uint8  *) (data + (index + 8));
                color_type         = *(uint8  *) (data + (index + 9));
                compression_method = *(uint8  *) (data + (index + 10));
                // filter_method      = *(uint8  *) (data + (index + 11));
                interlace_method   = *(uint8  *) (data + (index + 12));
                index += 13;
            }
            else
            {
                return PngDecode_UnexpectedEof;
            }

            // @note: Only supported compression method 0
            //        (deflate/inflate compression with a sliding window of at most 32768 bytes) is defined.
            //        All standard PNG images must be compressed with this scheme.
            if (compression_method != 0)
            {
                return PngDecode_UnsupportedCompressionMethod;
            }
            // @note: Adam7 interlace method is not supported.
            if (interlace_method != PNG_INTERLACE_NONE)
            {
                return PngDecode_UnsupportedInterlacedMethod;
            }

            if (color_type == PNG_GRAYSCALE_USED)
            {
                // Each pixel is a grayscale sample.
                // Allowed bit depths: 1, 2, 4, 8, 16 bits per pixel
                uint32 bits_per_pixel = bit_depth;
                ASSERT((bits_per_pixel * width * height) % 8 == 0);
            }
            else if (color_type == PNG_COLOR_USED)
            {
                // Each pixel is an RGB triple.
                // Allowed bit depths: 8, 16
                uint32 bits_per_pixel = 3 * bit_depth;
                ASSERT((bits_per_pixel * width * height) % 8 == 0);
            }
            else if (color_type == (PNG_COLOR_USED | PNG_PALETTE_USED))
            {
                // Each pixel is a palette index. A PLTE chunk must appear.
                // Allowed bit depths: 1, 2, 4, 8
                uint32 bits_per_pixel = bit_depth;
                ASSERT((bits_per_pixel * width * height) % 8 == 0);
            }
            else if (color_type == PNG_ALPHA_CHANNEL_USED)
            {
                // Each pixel is a grayscale sample, followed by an alpha sample.
                // Allowed bit depths: 8, 16
                uint32 bits_per_pixel = 2 * bit_depth;
                ASSERT((bits_per_pixel * width * height) % 8 == 0);
            }
            else if (color_type == (PNG_COLOR_USED | PNG_ALPHA_CHANNEL_USED))
            {
                // Each pixel is an RGB triple, followed by an alpha sample.
                // Allowed bit depths: 8, 16
                uint32 bits_per_pixel = 4 * bit_depth;
                ASSERT((bits_per_pixel * width * height) % 8 == 0);
            }
            else
            {
                return PngDecode_UnsupportedColorType;
            }
        }
        else
        {
            return PngDecode_UnrecognizedChunk;
        }

        uint32 chunk_crc;
        if (index + 4 <= file_size)
        {
            chunk_crc = *(uint32 *) (data);
            chunk_crc = uint32_change_endianness(chunk_crc);
            index += 4;
        }
        else
        {
            return PngDecode_UnexpectedEof;
        }

        if (chunk_crc != chunk_computed_crc)
        {
            return PngDecode_CrcNotEqual;
        }
    }

    return PngDecode_UnexpectedEof;
}




















