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

enum
{
    PNG_FILTER_NONE    = 0,
    PNG_FILTER_SUB     = 1,
    PNG_FILTER_UP      = 2,
    PNG_FILTER_AVERAGE = 3,
    PNG_FILTER_PAETH   = 4,
};

// LITLEN_symbol - 257 => index in this array
static uint32 LEN_bases[] = {
     3,  4,  5,   6,   7,   8,   9,  10,  11, 13,
    15, 17, 19,  23,  27,  31,  35,  43,  51, 59,
    67, 83, 99, 115, 131, 163, 195, 227, 258,
};
static uint32 LEN_extras[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
    4, 4, 4, 4, 5, 5, 5, 5, 0,
};
// DIST_symbol => index in this array
static uint32 DIST_bases[] = {
       1,    2,    3,    4,    5,    7,    9,    13,    17,    25,
      33,   49,   65,   97,  129,  193,  257,   385,   513,   769,
    1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577,
};
static uint32 DIST_extras[] = {
    0, 0,  0,  0,  1,  1,  2,  2,  3,  3,
    4, 4,  5,  5,  6,  6,  7,  7,  8,  8,
    9, 9, 10, 10, 11, 11, 12, 12, 13, 13,
};
// Static Huffman code lengths
static const uint32 fixed_litlen_lenghts[] =
{
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8
};
static const uint32 fixed_dist_lengths[32] =
{
   5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
};

typedef struct
{
    bool32 is_mid_stream;
    uint32 bits;
    uint32 bits_available;

    uint32 stream_in_cursor;
    uint32 stream_in_size;
    uint8 *stream_in;

    uint32 stream_out_cursor;
    uint32 stream_out_size;
    uint8 *stream_out;
} zlib_decoder;

typedef struct
{
    uint32 symbol;
    uint32 code_length;
    uint32 code;
} zlib_huffman_table_entry;

typedef struct
{
    uint32 max_code_length;
    uint32 entry_count;
    zlib_huffman_table_entry *entries;
} zlib_huffman_table;

uint32 zlib_get_bits(zlib_decoder *z, uint32 bit_count);
bool32 zlib_write_byte(zlib_decoder *z, uint8 byte);
bool32 zlib_decode(zlib_decoder *z);
zlib_huffman_table zlib_huffman_compute(uint32 const *code_lengths, uint32 code_length_count);
uint32 zlib_huffman_decode(zlib_decoder *z, zlib_huffman_table huffman);

int png_paeth_predictor(int a, int b, int c)
{
    // a - left, b - above, c - upper left

    // initial estimate
    int p = a + b - c;
    // distances to a, b, c
    int pa = p < a ? a - p : p - a;
    int pb = p < b ? b - p : p - b;
    int pc = p < c ? c - p : p - c;
    // return nearest of a, b, c,
    // breaking ties in order a, b, c.
    if ((pa <= pb) && (pa <= pc)) return a;
    else if (pb <= pc) return b;
    else return c;
}

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

    uint32 bits_per_pixel = 0;
    bool32 is_ihdr_done = false;

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

    bool32 is_end = false;
    while (!is_end && index < file_size)
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
            if (is_ihdr_done)
            {
                return PngDecode_IHDR_HeaderIsNotFirst;
            }
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
                bits_per_pixel = bit_depth;
            }
            else if (color_type == PNG_COLOR_USED)
            {
                // Each pixel is an RGB triple.
                // Allowed bit depths: 8, 16
                bits_per_pixel = 3 * bit_depth;
            }
            else if (color_type == (PNG_COLOR_USED | PNG_PALETTE_USED))
            {
                // Each pixel is a palette index. A PLTE chunk must appear.
                // Allowed bit depths: 1, 2, 4, 8
                bits_per_pixel = bit_depth;
            }
            else if (color_type == PNG_ALPHA_CHANNEL_USED)
            {
                // Each pixel is a grayscale sample, followed by an alpha sample.
                // Allowed bit depths: 8, 16
                bits_per_pixel = 2 * bit_depth;
            }
            else if (color_type == (PNG_COLOR_USED | PNG_ALPHA_CHANNEL_USED))
            {
                // Each pixel is an RGB triple, followed by an alpha sample.
                // Allowed bit depths: 8, 16
                bits_per_pixel = 4 * bit_depth;
            }
            else
            {
                return PngDecode_UnsupportedColorType;
            }

            *out_image_size = (bits_per_pixel * width * height) / 8;
            ASSERT((bits_per_pixel * width * height) % 8 == 0);
            is_end = true;
            is_ihdr_done = true;
        }
        else if (chunk_type == PNG_IEND_ID)
        {
            if (!is_ihdr_done)
                return PngDecode_IHDR_HeaderIsNotFirst;
            // IEND chunk has no data.
            is_end = true;
        }
        else if ((chunk_type == PNG_sRGB_ID) ||
                 (chunk_type == PNG_IDAT_ID) ||
                 (chunk_type == PNG_tRNS_ID) ||
                 (chunk_type == PNG_cHRM_ID) ||
                 (chunk_type == PNG_iCCP_ID) ||
                 (chunk_type == PNG_iTXt_ID) ||
                 (chunk_type == PNG_tEXt_ID) ||
                 (chunk_type == PNG_zTXt_ID) ||
                 (chunk_type == PNG_bKGD_ID) ||
                 (chunk_type == PNG_pHYs_ID) ||
                 (chunk_type == PNG_sBIT_ID) ||
                 (chunk_type == PNG_sPLT_ID) ||
                 (chunk_type == PNG_hIST_ID) ||
                 (chunk_type == PNG_tIME_ID))
        {
            // Ignored chunks
            if (!is_ihdr_done)
                return PngDecode_IHDR_HeaderIsNotFirst;
            index += chunk_size;
        }

        uint32 chunk_crc;
        if (index + 4 <= file_size)
        {
            chunk_crc = *(uint32 *) (data + (index));
            chunk_crc = uint32_change_endianness(chunk_crc);
            index += 4;
        }
        else
        {
            return PngDecode_UnexpectedEof;
        }

        if (chunk_crc != chunk_computed_crc)
        {
            return PngDecode_CrcDoesNotMatch;
        }
    }

    return is_ihdr_done ? PngDecode_Success : PngDecode_UnexpectedEof;
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

    uint32 bits_per_pixel = 0;
    uint32 computed_image_size = 0;
    bool32 is_ihdr_done = false;
    zlib_decoder decoder = {};

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
    uint32 width = 0;
    uint32 height = 0;
    uint8  bit_depth;
    uint8  color_type;
    uint8  compression_method;
    // uint8  filter_method;
    uint8  interlace_method;
    // sRGB
    uint8 rendering_intent;
    // gAMA
    float64 gamma = 0;

    bool32 is_end = false;
    while (!is_end && index < file_size)
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
                bits_per_pixel = bit_depth;
            }
            else if (color_type == PNG_COLOR_USED)
            {
                // Each pixel is an RGB triple.
                // Allowed bit depths: 8, 16
                bits_per_pixel = 3 * bit_depth;
            }
            else if (color_type == (PNG_COLOR_USED | PNG_PALETTE_USED))
            {
                // Each pixel is a palette index. A PLTE chunk must appear.
                // Allowed bit depths: 1, 2, 4, 8
                bits_per_pixel = bit_depth;
            }
            else if (color_type == PNG_ALPHA_CHANNEL_USED)
            {
                // Each pixel is a grayscale sample, followed by an alpha sample.
                // Allowed bit depths: 8, 16
                bits_per_pixel = 2 * bit_depth;
            }
            else if (color_type == (PNG_COLOR_USED | PNG_ALPHA_CHANNEL_USED))
            {
                // Each pixel is an RGB triple, followed by an alpha sample.
                // Allowed bit depths: 8, 16
                bits_per_pixel = 4 * bit_depth;
            }
            else
            {
                return PngDecode_UnsupportedColorType;
            }

            computed_image_size = (bits_per_pixel * width * height) / 8;
            ASSERT((bits_per_pixel * width * height) % 8 == 0);
            if (computed_image_size != image_size)
            {
                return PngDecode_ImageSizeMismatch;
            }

            // @note: +height for filter types at the start of each scanline
            decoder.stream_out_cursor = 0;
            decoder.stream_out_size = computed_image_size + height;
            decoder.stream_out = malloc(decoder.stream_out_size);

            is_ihdr_done = true;
        }
        else if (chunk_type == PNG_sRGB_ID)
        {
            if (!is_ihdr_done)
                return PngDecode_IHDR_HeaderIsNotFirst;
            if (index + 1 <= file_size)
            {
                rendering_intent = *(uint8 *) (data + (index));
                index += 1;
            }
            else
            {
                return PngDecode_UnexpectedEof;
            }
        }
        else if (chunk_type == PNG_gAMA_ID)
        {
            if (!is_ihdr_done)
                return PngDecode_IHDR_HeaderIsNotFirst;
            if (index + 4 <= file_size)
            {
                uint32 gamma_ = *(uint32 *) (data + (index));
                gamma_ = uint32_change_endianness(gamma_);
                gamma = ((float64) gamma_) / 100000.0;
                // Gamma should have no effect on the alpha channel, which is always linear
                // fraction of full opacity.
                index += 4;
            }
            else
            {
                PngDecode_UnexpectedEof;
            }
        }
        else if (chunk_type == PNG_IDAT_ID)
        {
            if (!is_ihdr_done)
                return PngDecode_IHDR_HeaderIsNotFirst;

            decoder.stream_in_cursor = 0;
            decoder.stream_in_size = chunk_size;
            decoder.stream_in = data + index;

            bool32 zlib_result = zlib_decode(&decoder);
            if (zlib_result != true)
            {
                return PngDecode_ZlibFailed;
            }

            index += chunk_size;
        }
        else if (chunk_type == PNG_IEND_ID)
        {
            if (!is_ihdr_done)
                return PngDecode_IHDR_HeaderIsNotFirst;
            // IEND chunk has no data.
            is_end = true;
        }
        else if ((chunk_type == PNG_tRNS_ID) ||
                 (chunk_type == PNG_cHRM_ID) ||
                 (chunk_type == PNG_iCCP_ID) ||
                 (chunk_type == PNG_iTXt_ID) ||
                 (chunk_type == PNG_tEXt_ID) ||
                 (chunk_type == PNG_zTXt_ID) ||
                 (chunk_type == PNG_bKGD_ID) ||
                 (chunk_type == PNG_pHYs_ID) ||
                 (chunk_type == PNG_sBIT_ID) ||
                 (chunk_type == PNG_sPLT_ID) ||
                 (chunk_type == PNG_hIST_ID) ||
                 (chunk_type == PNG_tIME_ID))
        {
            // Ignored chunks
            if (!is_ihdr_done)
                return PngDecode_IHDR_HeaderIsNotFirst;
            index += chunk_size;
        }
        else
        {
            return PngDecode_UnrecognizedChunk;
        }

        uint32 chunk_crc;
        if (index + 4 <= file_size)
        {
            chunk_crc = *(uint32 *) (data + (index));
            chunk_crc = uint32_change_endianness(chunk_crc);
            index += 4;
        }
        else
        {
            return PngDecode_UnexpectedEof;
        }

        if (chunk_crc != chunk_computed_crc)
        {
            return PngDecode_CrcDoesNotMatch;
        }
    }

    if (!is_ihdr_done || !is_end)
    {
        return PngDecode_UnrecognizedChunk;
    }

    // Defilter the image
    {
        uint8 *in  = (uint8 *) decoder.stream_out;
        uint32 in_cursor = 0;
        uint8 *out = (uint8 *) image_data;
        uint32 out_cursor = 0;
        uint32 bytes_per_pixel = bits_per_pixel / 8;
        uint32 stride = width * bytes_per_pixel;

        // Process first scanline separately, because references to previous
        // scanline should result to 0
        {
            uint32 x;
            uint8 filter_method = *(in + in_cursor);
            in_cursor += 1;
            for (x = 0; x < bytes_per_pixel; x++)
            {
                *(out + out_cursor) = *(in + in_cursor);
                in_cursor += 1;
                out_cursor += 1;
            }
            for (x = bytes_per_pixel; x < stride; x++)
            {
                uint32 additional_value = 0;
                switch (filter_method)
                {
                    case PNG_FILTER_NONE: break;
                    case PNG_FILTER_SUB:
                        additional_value = *(out + (out_cursor - bytes_per_pixel));
                        break;
                    case PNG_FILTER_UP: break; // Nothong because on first scanline
                    case PNG_FILTER_AVERAGE:
                        additional_value = (*(out + (out_cursor - bytes_per_pixel))) >> 1;
                        break;
                    case PNG_FILTER_PAETH:
                        additional_value = png_paeth_predictor(*(out + (out_cursor - bytes_per_pixel)), 0, 0);
                        break;
                    default:
                        ASSERT_FAIL("PNG: Unknown filter method");
                        return PngDecode_UnknownFilterMethod;
                }
                *(out + out_cursor) = *(in + in_cursor) + (uint8) additional_value;
                in_cursor += 1;
                out_cursor += 1;
            }
        }

        uint32 x, y;
        for (y = 1; y < height; y++)
        {
            uint32 filter_method = *(in + in_cursor);
            in_cursor += 1;
            for (x = 0; x < bytes_per_pixel; x++)
            {
                uint32 additional_value = 0;
                switch (filter_method)
                {
                    case PNG_FILTER_NONE:
                        break;
                    case PNG_FILTER_SUB:
                        break;
                    case PNG_FILTER_UP:
                        additional_value = *(out + (out_cursor - stride));
                        break;
                    case PNG_FILTER_AVERAGE:
                        additional_value = (*(out + (out_cursor - stride))) >> 1;
                        break;
                    case PNG_FILTER_PAETH:
                        additional_value = png_paeth_predictor(0, *(out + (out_cursor - stride)), 0);
                        break;
                    default:
                        ASSERT_FAIL("PNG: Unknown filter method");
                        return PngDecode_UnknownFilterMethod;
                }
                *(out + out_cursor) = *(in + in_cursor) + (uint8) additional_value;
                in_cursor += 1;
                out_cursor += 1;
            }
            for (x = bytes_per_pixel; x < stride; x++)
            {
                uint32 additional_value = 0;
                switch (filter_method)
                {
                    case PNG_FILTER_NONE:
                        break;
                    case PNG_FILTER_SUB:
                        additional_value = *(out + (out_cursor - bytes_per_pixel));
                        break;
                    case PNG_FILTER_UP:
                        additional_value = *(out + (out_cursor - stride));
                        break;
                    case PNG_FILTER_AVERAGE:
                        additional_value = (*(out + (out_cursor - bytes_per_pixel)) + *(out + (out_cursor - stride))) >> 1;
                        break;
                    case PNG_FILTER_PAETH:
                        additional_value =
                            png_paeth_predictor(
                                *(out + (out_cursor - bytes_per_pixel)),
                                *(out + (out_cursor - stride)),
                                *(out + (out_cursor - stride - bytes_per_pixel)));
                        break;
                    default: ASSERT_FAIL("Unknown filter method");
                }
                *(out + out_cursor) = *(in + in_cursor) + (uint8) additional_value;
                in_cursor += 1;
                out_cursor += 1;
            }
        }
    }

    *out_width = width;
    *out_height = height;
    *out_bits_per_pixel = bits_per_pixel;
    *out_color_mode = Bitmap_RGBA;
    *out_is_top_down = false;

    free(decoder.stream_out);

    return PngDecode_Success;
}

uint32 zlib_get_bits(zlib_decoder *z, uint32 bit_count)
{
    uint32 result = 0;
    if (0 < bit_count && bit_count < 24)
    {
        while (z->bits_available < bit_count)
        {
            if (z->stream_in_cursor < z->stream_in_size)
            {
                uint32 b = *(z->stream_in + z->stream_in_cursor++);
                z->bits = z->bits | (b << z->bits_available);
                z->bits_available += 8;
            }
            else
            {
                return 0;
            }
        }
        result = z->bits & ((1 << bit_count) - 1);
        z->bits = (z->bits >> bit_count);
        z->bits_available = z->bits_available - bit_count;
    }
    return result;
}

bool32 zlib_write_byte(zlib_decoder *z, uint8 byte)
{
    if (z->stream_out_cursor < z->stream_out_size)
    {
        *(z->stream_out + z->stream_out_cursor) = byte;
        z->stream_out_cursor += 1;
        return true;
    }
    return false;
}

void zlib_skip_byte(zlib_decoder *z)
{
    uint32 n = z->bits_available % 8;
    z->bits = z->bits >> n;
    z->bits_available = z->bits_available - n;
}

bool32 zlib_decode(zlib_decoder *z)
{
    // @note: zlib stream chunks are not always aligned with iDAT chunks
    // of PNG file, I need to make zlib_decoder have option to continue
    // from previous state!

    if (!z->is_mid_stream)
    {
        uint32 CMF = zlib_get_bits(z, 8);
        uint8 CM  = 0x0F & CMF;
        if (CM != 8)
        {
            // CM = 8 denotes the "deflate" compression method with a window size up to 32K.
            // This is the method used by gzip and PNG. It MUST be equal to 8.
            return false;
        }
        uint8 CINFO = (0xF0 & CMF) >> 4;
        // CINFO = log_2(LZ77_window_size) - 8;
        // Therefore => uint32 LZ77_window_size = 1 << (CINFO + 8);
        // CINFO=7 indicates 32768 window size.
        if (CINFO > 7)
        {
            // CINFO above 7 are not allowed by the spec
            // CINFO is not defined if CM != 8
            return false;
        }
        uint32 LZ77_window_size = 1 << (CINFO + 8);
        uint32 FLG = zlib_get_bits(z, 8);
        uint8 FCHECK = (0x1F & FLG);
        uint8 FDICT  = (0x20 & FLG) >> 5;
        uint8 FLEVEL = (0xC0 & FLG) >> 6;
        if (((CMF * 256 + FLG) % 31) != 0)
        {
            // The FCHECK value must be such that CMF and FLG, when viewed as
            // a 16-bit unsigned integer stored in MSB order (CMF*256 + FLG),
            // is a multiple of 31.
            return false;
        }
        if (FDICT != 0)
        {
            // If FDICT is set, a DICT dictionary identifier is present
            // immediately after the FLG byte. The dictionary is a sequence of
            // bytes which are initially fed to the compressor without
            // producing any compressed output. DICT is the Adler-32 checksum
            // of this sequence of bytes (see the definition of ADLER32
            // below).  The decompressor can use this identifier to determine
            // which dictionary has been used by the compressor.
            return false;
        }
        z->is_mid_stream = true;
    }

    uint8 BFINAL = 0;
    uint8 BTYPE;
    while (BFINAL == 0)
    {
        BFINAL = zlib_get_bits(z, 1);
        BTYPE  = zlib_get_bits(z, 2);

        if (BTYPE == 0)
        {
            // Stored with no compression

            //   0   1   2   3   4...
            // +---+---+---+---+================================+
            // |  LEN  | NLEN  |... LEN bytes of literal data...|
            // +---+---+---+---+================================+

            zlib_skip_byte(z);
            uint16 LEN = zlib_get_bits(z, 16);
            uint16 NLEN = zlib_get_bits(z, 16);

            if (LEN != ((uint16) ~NLEN))
            {
                ASSERT_FAIL("Something wrong");
                return false;
            }
            for (int i = 0; i < LEN; i++)
            {
                uint8 bits = zlib_get_bits(z, 8);
                bool32 ok = zlib_write_byte(z, bits);
                ASSERT_MSG(ok, "PNG: Buffer overrun while writing uncompressed data.");
                if (!ok) return false;
            }
        }
        else if ((BTYPE == 1) || (BTYPE == 2))
        {
            zlib_huffman_table litlen_huffman = {};
            zlib_huffman_table dist_huffman = {};

            if (BTYPE == 1)
            {
                // Compressed with fixed Huffman code
                litlen_huffman = zlib_huffman_compute(fixed_litlen_lenghts, ARRAY_COUNT(fixed_litlen_lenghts));
                dist_huffman = zlib_huffman_compute(fixed_dist_lengths, ARRAY_COUNT(fixed_dist_lengths));
            }
            if (BTYPE == 2)
            {
                // Compressed with dynamic Huffman code

                uint32 HLIT = zlib_get_bits(z, 5) + 257;
                uint32 HDIST = zlib_get_bits(z, 5) + 1;
                uint32 HCLEN = zlib_get_bits(z, 4) + 4;

                uint32 code_lengths_swizzle[] =
                {
                    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
                };

                uint32 CLEN_code_lengths[19] = {};
                for (uint32 i = 0; i < HCLEN; i++)
                {
                    CLEN_code_lengths[code_lengths_swizzle[i]] = zlib_get_bits(z, 3);
                }

                zlib_huffman_table clen_huffman = zlib_huffman_compute(CLEN_code_lengths, ARRAY_COUNT(CLEN_code_lengths));

                uint32 *LITLEN_DIST_code_lengths = malloc((HLIT + HDIST) * sizeof(uint32));

                uint32 index = 0;
                while (index < (HLIT + HDIST))
                {
                    uint32 symbol = zlib_huffman_decode(z, clen_huffman);
                    if (symbol < 16)
                    {
                        LITLEN_DIST_code_lengths[index++] = symbol;
                    }
                    else
                    {
                        uint32 repeat_value = 0;
                        uint32 repeat_length = 0;
                        if (symbol == 16)
                        {
                            repeat_value = LITLEN_DIST_code_lengths[index - 1];
                            repeat_length = zlib_get_bits(z, 2) + 3;
                        }
                        else if (symbol == 17)
                        {
                            repeat_length = zlib_get_bits(z, 3) + 3;
                        }
                        else if (symbol == 18)
                        {
                            repeat_length = zlib_get_bits(z, 7) + 11;
                        }

                        uint32 repeat;
                        for (repeat = 0; repeat < repeat_length; repeat++)
                        {
                            LITLEN_DIST_code_lengths[index++] = repeat_value;
                        }
                    }
                }
                litlen_huffman = zlib_huffman_compute(LITLEN_DIST_code_lengths, HLIT);
                dist_huffman = zlib_huffman_compute(LITLEN_DIST_code_lengths + HLIT, HDIST);

                free(LITLEN_DIST_code_lengths);
            }

            while (true)
            {
                uint32 LITLEN = zlib_huffman_decode(z, litlen_huffman);
                if (LITLEN < 256)
                {
                    // Copy value (literal byte) to output stream
                    uint8 bits = LITLEN;
                    bool32 ok = zlib_write_byte(z, bits);
                    ASSERT_MSG(ok, "PNG: Buffer overrun while writing literal byte in LZ77 decompression.");
                    if (!ok) return false;
                }
                else if (LITLEN == 256) // End of block
                {
                    break;
                }
                else if (LITLEN < 286)
                {
                    uint32 LEN_index = LITLEN - 257;
                    uint32 LEN = LEN_bases[LEN_index];
                    if (LEN_extras[LEN_index])
                    {
                        LEN += zlib_get_bits(z, LEN_extras[LEN_index]);
                    }

                    uint32 DIST_index = zlib_huffman_decode(z, dist_huffman);
                    uint32 DIST = DIST_bases[DIST_index];
                    if (DIST_extras[DIST_index])
                    {
                        DIST += zlib_get_bits(z, DIST_extras[DIST_index]);
                    }

                    uint8 *repeat_pointer = z->stream_out + (z->stream_out_cursor - DIST);
                    uint32 repeat;
                    for (repeat = 0; repeat < LEN; repeat++)
                    {
                        uint8 bits = *repeat_pointer++;
                        bool32 ok = zlib_write_byte(z, bits);
                        ASSERT_MSG(ok, "PNG: Buffer overrun while writing repeat bytes in LZ77 decompression.");
                        if (!ok) return false;
                    }
                }
                else
                {
                    ASSERT_FAIL("Decodingn PNG error!");
                    return false;
                }
            }

            if (BTYPE == 2)
            {
                free(litlen_huffman.entries);
                free(dist_huffman.entries);
            }
        }
        else
        {
            ASSERT_FAIL("BTYPE != 0, 1, or 2;");
            return false;
        }
    }

    return true;
}

zlib_huffman_table zlib_huffman_compute(uint32 const *code_lengths, uint32 code_length_count)
{
    // DEFLATE algorithm allows to compute huffman tree just from the sequence of code lengths.

    uint32 max_len = 0;
    uint32 bl_count[17] = {};
    uint32 next_code[16] = {};

    zlib_huffman_table_entry *huffman_table = malloc(code_length_count * sizeof(*huffman_table));

    // Count the number of codes for each code length.  Let
    // bl_count[N] be the number of codes of length N, N >= 1.
    {
        int i;
        for (i = 0; i < code_length_count; i++)
        {
            bl_count[code_lengths[i]] += 1;
        }
    }

    // Find the numerical value of the smallest code for each
    // code length:
    {
        int bits;
        uint32 code = 0;
        bl_count[0] = 0;
        for (bits = 1; bits < 16; bits++)
        {
            code = (code + bl_count[bits-1]) << 1;
            next_code[bits] = code;
        }
    }

    // Assign numerical values to all codes, using consecutive
    // values for all codes of the same length with the base
    // values determined at step 2. Codes that are never used
    // (which have a bit length of zero) must not be assigned a
    // value.
    {
        int symbol;
        for (symbol = 0; symbol < code_length_count; symbol++)
        {
            uint32 len = code_lengths[symbol];
            if (len > 0)
            {
                huffman_table[symbol].symbol = symbol;
                huffman_table[symbol].code_length = len;
                huffman_table[symbol].code = next_code[len]++;
            }
            else
            {
                huffman_table[symbol].symbol = 0;
                huffman_table[symbol].code_length = 0;
                huffman_table[symbol].code = 0;
            }
            if (max_len < len)
            {
                max_len = len;
            }
        }
    }

    zlib_huffman_table result = {};
    result.max_code_length = max_len;
    result.entry_count = code_length_count;
    result.entries = huffman_table;
    return result;
}

uint32 zlib_huffman_decode(zlib_decoder *z, zlib_huffman_table huffman)
{
    uint32 bit_count = 0;
    uint32 bits = 0;

    while (bit_count < huffman.max_code_length)
    {
        uint32 bit = zlib_get_bits(z, 1);
        bits = (bits << 1) | bit;
        bit_count += 1;

        uint32 i;
        for (i = 0; i < huffman.entry_count; i++)
        {
            zlib_huffman_table_entry entry = huffman.entries[i];
            if ((bit_count == entry.code_length) && (bits == entry.code))
            {
                return entry.symbol;
            }
        }
    }

    return 0;
}
