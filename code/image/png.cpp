#include "png.hpp"
#include <crc.hpp>
#include <math/float32.h>
#include <math/integer.h>
#include <memory_bucket.hpp>

//
// PNG (Portable Network Graphics) Specification, Version 1.2
//   http://www.libpng.org/pub/png/spec/1.2/PNG-Contents.html
//
// ZLIB Compressed Data Format Specification version 3.3
//   https://www.ietf.org/rfc/rfc1950.txt
//
// DEFLATE Compressed Data Format Specification version 1.3:
//   https://www.ietf.org/rfc/rfc1951.txt
//


namespace image {


#pragma pack(push, 1)
struct png__chunk_header {
    uint32 size_of_data; // in bytes
    uint32 type;
};

struct png__ihdr_header {
    uint32 width;
    uint32 height;
    uint8  bit_depth;
    uint8  color_type;
    uint8  compression_method;
    uint8  filter_method;
    uint8  interlace_method;
};
#pragma pack(pop)

enum png__color_type : uint8 {
    PNG_GRAYSCALE_USED     = 0x0,
    PNG_PALETTE_USED       = 0x1,
    PNG_COLOR_USED         = 0x2,
    PNG_ALPHA_CHANNEL_USED = 0x4,
};

enum png__rendering_intent : uint8 {
    PNG_PERCEPTUAL = 0,
    PNG_RELATIVE_COLORIMETRIC = 1,
    PNG_SATURATION = 2,
    PNG_ABSOLUTE_COLORIMETRIC = 3,
};

enum png__interlace_method : uint8 {
    PNG_INTERLACE_NONE  = 0,
    PNG_INTERLACE_ADAM7 = 1,
};

enum png__filter_method : uint8 {
    PNG_FILTER_NONE    = 0,
    PNG_FILTER_SUB     = 1,
    PNG_FILTER_UP      = 2,
    PNG_FILTER_AVERAGE = 3,
    PNG_FILTER_PAETH   = 4,
};

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


// LITLEN_symbol - 257 => index in this array
GLOBAL uint32 LEN_bases[] = {
     3,  4,  5,   6,   7,   8,   9,  10,  11, 13,
    15, 17, 19,  23,  27,  31,  35,  43,  51, 59,
    67, 83, 99, 115, 131, 163, 195, 227, 258,
};
GLOBAL uint32 LEN_extras[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
    4, 4, 4, 4, 5, 5, 5, 5, 0,
};
// DIST_symbol => index in this array
GLOBAL uint32 DIST_bases[] = {
       1,    2,    3,    4,    5,    7,    9,    13,    17,    25,
      33,   49,   65,   97,  129,  193,  257,   385,   513,   769,
    1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577,
};
GLOBAL uint32 DIST_extras[] = {
    0, 0,  0,  0,  1,  1,  2,  2,  3,  3,
    4, 4,  5,  5,  6,  6,  7,  7,  8,  8,
    9, 9, 10, 10, 11, 11, 12, 12, 13, 13,
};
// Static Huffman code lengths
GLOBAL const uint32 fixed_litlen_lenghts[] =
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
GLOBAL const uint32 fixed_dist_lengths[32] =
{
   5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
};


namespace zlib {

struct stream
{
    uint8 *memory;
    usize size;

    uint8 *cursor;
    uint8 *end_of_stream;
};

stream create_stream(void *memory, usize size)
{
    stream result;
    result.memory = (uint8 *) memory;
    result.size = size;
    result.cursor = (uint8 *) memory;
    result.end_of_stream = ((uint8 *) memory) + size;

    return result;
}

struct decoder
{
    bool32 initialized;

    stream input;
    stream output;

    uint32 bits;
    uint32 bits_available;
};

uint32 get_bits_unsafe(decoder *z, uint32 n)
{
    uint32 result = z->bits & ((1 << n) - 1);
    z->bits = z->bits >> n;
    z->bits_available = z->bits_available - n;
    return result;
}

void refill_byte(decoder *z)
{
    if ((z->bits_available < 24) && (z->input.cursor < z->input.end_of_stream))
    {
        uint32 byte = *z->input.cursor++;
        z->bits = z->bits | (byte << z->bits_available);
        z->bits_available += 8;
    }
}

uint32 get_bits(decoder *z, uint32 requested_bit_count)
{
    ASSERT(0 < requested_bit_count && requested_bit_count <= 32);

    while (z->bits_available < requested_bit_count)
    {
        refill_byte(z);
    }

    uint32 result = z->bits & ((1 << requested_bit_count) - 1);
    z->bits = z->bits >> requested_bit_count;
    z->bits_available -= requested_bit_count;

    return result;
}

void skip_byte(decoder *z)
{
    get_bits_unsafe(z, z->bits_available % 8);
}

} // namespace zlib


namespace png {

using crc_t = uint32;

void *consume_memory(uint8 **data, usize size) {
    void *result = *data;
    *data += size;
    return result;
}

int paeth_predictor(int a, int b, int c)
{
    // a - left, b - above, c - upper left
    int p = a + b - c; // initial estimate
    int pa = absolute(p - a); // distances to a, b, c
    int pb = absolute(p - b);
    int pc = absolute(p - c);
    // return nearest of a, b, c,
    // breaking ties in order a, b, c.
    if ((pa <= pb) && (pa <= pc)) return a;
    else if (pb <= pc) return b;
    else return c;
}

} // namespace png

#define PNG_CONSUME_STRUCT(POINTER, TYPE) (TYPE *)png::consume_memory(&POINTER, sizeof(TYPE))

bool32 decode_idat_chunk(zlib::decoder *decoder, memory_allocator *temporary_allocator);
bitmap load_png(memory_allocator *allocator, memory_allocator *temporary, memory_buffer contents)
{
    bitmap result = {};

    if (!contents) return result;

    auto zlib_stream = memory_bucket::from(ALLOCATE_BUFFER(*temporary, contents.size));

    uint8 *data = (uint8 *) contents.data;

    uint32 signature1 = *PNG_CONSUME_STRUCT(data, uint32);
    uint32 signature2 = *PNG_CONSUME_STRUCT(data, uint32);

    if (signature1 != PNG_SIGN_1 || signature2 != PNG_SIGN_2)
    {
        // @todo: log "Could not load PNG file from data"
        return result;
    }

    png__ihdr_header *ihdr;
    png__rendering_intent *rendering_intent;
    UNUSED(rendering_intent);
    float64 gamma = 0.f;
    UNUSED(gamma);

    zlib::decoder decoder = {};

    bool32 is_end = false;
    while ((is_end == false) && (data < ((uint8 *) contents.data + contents.size)))
    {
        //
        // Chunks of PNG file follow this layout:
        //
        // +-------------------+-------------+----------------+
        // |  PNG_ChunkHeader  |  Data  ...  |  PNG_ChunkCRC  |
        // +-------------------+-------------+----------------+
        // |<-    8 bytes    ->|<- N bytes ->|<-  4  bytes  ->|
        //

        png__chunk_header chunk_header = *PNG_CONSUME_STRUCT(data, png__chunk_header);
        chunk_header.size_of_data = uint32__change_endianness(chunk_header.size_of_data);

        auto chunk_type_size = sizeof(chunk_header.type);
        png::crc_t computed_crc = uint32__change_endianness(
            compute_crc(data - chunk_type_size,
                        chunk_header.size_of_data + chunk_type_size)
        );

        if (chunk_header.type == PNG_IHDR_ID)
        {
            ihdr = PNG_CONSUME_STRUCT(data, png__ihdr_header);

            // @note: Only compression method 0 (deflate/inflate compression with
            // a sliding window of at most 32768 bytes) is defined.
            // All standard PNG images must be compressed with this scheme.
            // @note: Adam7 interlace method is not supported.
            if ((ihdr->compression_method != 0) || (ihdr->interlace_method != PNG_INTERLACE_NONE))
            {
                return result;
            }

            if (ihdr->color_type == 0)
            {
                // Each pixel is a grayscale sample.
                // Allowed bit depths: 1, 2, 4, 8, 16 bits per pixel
                result.bits_per_pixel = ihdr->bit_depth;
                result.color_type     = IMAGE_W;
            }
            else if (ihdr->color_type == 2)
            {
                // Each pixel is an RGB triple.
                // Allowed bit depths: 8, 16
                result.bits_per_pixel = ihdr->bit_depth * 3;
                result.color_type     = IMAGE_RGB;
            }
            else if (ihdr->color_type == 3)
            {
                // Each pixel is a palette index. A PLTE chunk must appear.
                // Allowed bit depths: 1, 2, 4, 8
                result.bits_per_pixel = ihdr->bit_depth;
            }
            else if (ihdr->color_type == 4)
            {
                // Each pixel is a grayscale sample, followed by an alpha sample.
                // Allowed bit depths: 8, 16
                result.bits_per_pixel = ihdr->bit_depth * 2;
                result.color_type     = IMAGE_WA;
            }
            else if (ihdr->color_type == 6)
            {
                // Each pixel is an RGB triple, followed by an alpha sample.
                // Allowed bit depths: 8, 16
                result.bits_per_pixel = ihdr->bit_depth * 4;
                result.color_type     = IMAGE_RGBA;
            }
            else
            {
                // @todo: log error!
                return {};
            }

            result.width  = uint32__change_endianness(ihdr->width);
            result.height = uint32__change_endianness(ihdr->height);
            // @note additional + height for filter types at the start of each scanline
            result.size = result.width * result.height * (result.bits_per_pixel / 8) + result.height;

            auto buffer_1 = ALLOCATE_ALIGNED_BUFFER(*allocator, result.size, alignof(uint32));
            result.pixels = buffer_1.data;

            decoder.output = zlib::create_stream(result.pixels, result.size);
        }
        else if (chunk_header.type == PNG_IDAT_ID)
        {
            zlib_stream.append((void *) data, (usize) chunk_header.size_of_data);
            data += chunk_header.size_of_data;
        }
        else if (chunk_header.type == PNG_IEND_ID)
        {
            // IEND chunk has no data.
            is_end = true;

            decoder.input = zlib::create_stream(zlib_stream.data, zlib_stream.used);
            decode_idat_chunk(&decoder, temporary);
        }
        else if (chunk_header.type == PNG_sRGB_ID)
        {
            rendering_intent = PNG_CONSUME_STRUCT(data, png__rendering_intent);
            // sRGB chunk has no data.
        }
        else if (chunk_header.type == PNG_gAMA_ID)
        {
            uint32 gamma_ = uint32__change_endianness(*PNG_CONSUME_STRUCT(data, uint32));
            gamma = ((float64) gamma_) / 100000.0;
            // Gamma should have no effect on the alpha channel, which is always linear
            // fraction of full opacity.
        }
        else if ((chunk_header.type == PNG_tRNS_ID) ||
                 (chunk_header.type == PNG_cHRM_ID) ||
                 (chunk_header.type == PNG_iCCP_ID) ||
                 (chunk_header.type == PNG_iTXt_ID) ||
                 (chunk_header.type == PNG_tEXt_ID) ||
                 (chunk_header.type == PNG_zTXt_ID) ||
                 (chunk_header.type == PNG_bKGD_ID) ||
                 (chunk_header.type == PNG_pHYs_ID) ||
                 (chunk_header.type == PNG_sBIT_ID) ||
                 (chunk_header.type == PNG_sPLT_ID) ||
                 (chunk_header.type == PNG_hIST_ID) ||
                 (chunk_header.type == PNG_tIME_ID))
        {
            data += chunk_header.size_of_data;
        }
        else
        {
            ASSERT_FAIL("UNRECOGNIZED CHUNK");
        }

        png::crc_t chunk_crc = *PNG_CONSUME_STRUCT(data, png::crc_t);
        if (chunk_crc != computed_crc)
        {
            // @todo: handle invalid CRC !!!
            return {};
        }
    }

    // Defilter the image
    {
        uint8 *input_stream = result.pixels;
        uint8 *output_stream = result.pixels;
        uint32 bytes_per_pixel = result.bits_per_pixel / 8;
        uint32 stride = result.width * bytes_per_pixel;

        // Process first scanline separately, because references to previous scanline result to 0
        {
            auto filter_method = (png__filter_method) *input_stream++;
            for (uint32 x = 0; x < bytes_per_pixel; x++)
            {
                *output_stream++ = *input_stream++;
            }
            for (uint32 x = bytes_per_pixel; x < stride; x++)
            {
                uint32 additional_value = 0;
                switch (filter_method)
                {
                    case PNG_FILTER_NONE:
                        break;
                    case PNG_FILTER_SUB:
                        additional_value = *(output_stream - bytes_per_pixel);
                        break;
                    case PNG_FILTER_UP:
                        break; // Because first scanline
                    case PNG_FILTER_AVERAGE:
                        additional_value = (*(output_stream - bytes_per_pixel)) >> 1;
                        break;
                    case PNG_FILTER_PAETH:
                        additional_value = png::paeth_predictor(*(output_stream - bytes_per_pixel), 0, 0);
                        break;
                    default: ASSERT_FAIL("Unknown filter method");
                }
                *output_stream++ = *input_stream++ + (uint8) additional_value;
            }
        }

        for (uint32 y = 1; y < result.height; y++)
        {
            auto filter_method = (png__filter_method) *input_stream++;
            for (uint32 x = 0; x < bytes_per_pixel; x++)
            {
                uint32 additional_value = 0;
                switch (filter_method)
                {
                    case PNG_FILTER_NONE:
                    case PNG_FILTER_SUB:
                        break;
                    case PNG_FILTER_UP:
                        additional_value = *(output_stream - stride);
                        break;
                    case PNG_FILTER_AVERAGE:
                        additional_value = (*(output_stream - stride)) >> 1;
                        break;
                    case PNG_FILTER_PAETH:
                        additional_value = png::paeth_predictor(0, *(output_stream - stride), 0);
                        break;
                    default: ASSERT_FAIL("Unknown filter method");
                }
                *output_stream++ = *input_stream++ + (uint8) additional_value;
            }
            for (uint32 x = bytes_per_pixel; x < stride; x++)
            {
                uint32 additional_value = 0;
                switch (filter_method)
                {
                    case PNG_FILTER_NONE:
                        break;
                    case PNG_FILTER_SUB:
                        additional_value = *(output_stream - bytes_per_pixel);
                        break;
                    case PNG_FILTER_UP:
                        additional_value = *(output_stream - stride);
                        break;
                    case PNG_FILTER_AVERAGE:
                        additional_value = (*(output_stream - bytes_per_pixel) + *(output_stream - stride)) >> 1;
                        break;
                    case PNG_FILTER_PAETH:
                        additional_value =
                            png::paeth_predictor(
                                *(output_stream - bytes_per_pixel),
                                *(output_stream - stride),
                                *(output_stream - stride - bytes_per_pixel));
                        break;
                    default: ASSERT_FAIL("Unknown filter method");
                }
                *output_stream++ = *input_stream++ + (uint8) additional_value;
            }
        }
    }

    return result;
}

struct huffman_entry
{
    uint32 symbol;
    uint32 code_length;
};

bool32 operator != (huffman_entry a, huffman_entry b)
{
    bool32 result = (a.symbol != b.symbol);
    if (!result) ASSERT(a.code_length == b.code_length);
    return result;
}


array<huffman_entry> compute_huffman(memory_allocator *temporary_allocator,
                                     uint32 const *code_lengths,
                                     usize code_lengths_count);
uint32 decode_huffman(zlib::decoder *decoder, array<huffman_entry> huffman)
{
    usize A = 0, B = huffman.size();

    uint32 consumed_bits = 0;
    while (huffman[A] != huffman[B - 1])
    {
        uint32 bit = get_bits(decoder, 1);
        consumed_bits += 1;
        if (bit == 0)
            B = (A + B) / 2;
        else
            A = (A + B) / 2;
    }
    ASSERT(huffman[A].code_length == consumed_bits);
    return huffman[A].symbol;
}

bool32 decode_idat_chunk(zlib::decoder *decoder, memory_allocator *temporary_allocator)
{
    if (!decoder->initialized)
    {
        uint32 CMF = get_bits(decoder, 8);

        // Compression Method
        uint8 CM  = 0x0F & CMF;

        if (CM != 8)
        {
            // CM = 8 denotes the "deflate" compression method with a window size up
            // to 32K. This is the method used by gzip and PNG.
            return false;
        }

        // Compression Info
        uint8 CINFO = (0xF0 & CMF) >> 4;

        // CINFO = log_2(LZ77_window_size) - 8;
        // Therefore => uint32 LZ77_window_size = 1 << (CINFO + 8);
        //
        // CINFO=7 indicates 32768 window size.

        if (CINFO > 7)
        {
            // CINFO above 7 are not allowed by the specification
            // CINFO is not defined if CM != 8
            return false;
        }

        uint32 LZ77_window_size = 1 << (CINFO + 8);

        uint32 FLG = get_bits(decoder, 8); // Flags

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

        decoder->initialized = true;
    }

    uint8 BFINAL = 0;
    uint8 BTYPE;

    while (BFINAL == 0)
    {
        BFINAL = (uint8) get_bits(decoder, 1); // ((*zlib_data) & 0b0000'0001);
        BTYPE  = (uint8) get_bits(decoder, 2); // ((*zlib_data) & 0b0000'0110) >> 1;

        if (BTYPE == 0)
        {
            // Stored with no compression

            //   0   1   2   3   4...
            // +---+---+---+---+================================+
            // |  LEN  | NLEN  |... LEN bytes of literal data...|
            // +---+---+---+---+================================+

            skip_byte(decoder);
            uint16 LEN = (uint16) get_bits(decoder, 16);
            uint16 NLEN = (uint16) get_bits(decoder, 16);

            if (LEN != ((uint16) ~NLEN))
            {
                // @todo: something wrong
                ASSERT_FAIL("Something wrong");
                return false;
            }

            for (int i = 0; i < LEN; i++)
            {
                *(decoder->output.cursor) = (uint8) get_bits(decoder, 8);
                decoder->output.cursor += 1;
            }
        }
        else if ((BTYPE == 1) || (BTYPE == 2))
        {
            array<huffman_entry> litlen_huffman = {};
            array<huffman_entry> dist_huffman = {};

            if (BTYPE == 1)
            {
                // Compressed with fixed Huffman code
                litlen_huffman = compute_huffman(temporary_allocator, fixed_litlen_lenghts, ARRAY_COUNT(fixed_litlen_lenghts));
                dist_huffman = compute_huffman(temporary_allocator, fixed_dist_lengths, ARRAY_COUNT(fixed_dist_lengths));
            }
            if (BTYPE == 2)
            {
                // Compressed with dynamic Huffman code

                uint32 HLIT = get_bits(decoder, 5) + 257;
                uint32 HDIST = get_bits(decoder, 5) + 1;
                uint32 HCLEN = get_bits(decoder, 4) + 4;

                uint32 code_lengths_swizzle[] =
                {
                    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
                };
                uint32 CLEN_code_lengths[ARRAY_COUNT(code_lengths_swizzle)] = {};

                for (uint32 i = 0; i < HCLEN; i++)
                {
                    CLEN_code_lengths[code_lengths_swizzle[i]] = get_bits(decoder, 3);
                }

                auto clen_huffman = compute_huffman(temporary_allocator, CLEN_code_lengths, ARRAY_COUNT(CLEN_code_lengths));

                auto LITLEN_DIST_code_length_buffer = ALLOCATE_BUFFER(*temporary_allocator, (HLIT + HDIST)*sizeof(uint32));
                auto LITLEN_DIST_code_lengths = make_array<uint32>(LITLEN_DIST_code_length_buffer);

                LITLEN_DIST_code_lengths.resize(LITLEN_DIST_code_lengths.capacity());

                uint32 index = 0;
                while (index < HLIT + HDIST)
                {
                    uint32 symbol = decode_huffman(decoder, clen_huffman);
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
                            repeat_length = zlib::get_bits(decoder, 2) + 3;
                        }
                        else if (symbol == 17)
                        {
                            repeat_length = zlib::get_bits(decoder, 3) + 3;
                        }
                        else if (symbol == 18)
                        {
                            repeat_length = zlib::get_bits(decoder, 7) + 11;
                        }

                        for (uint32 repeat = 0; repeat < repeat_length; repeat++)
                        {
                            LITLEN_DIST_code_lengths[index++] = repeat_value;
                        }
                    }
                }
                litlen_huffman = compute_huffman(temporary_allocator, LITLEN_DIST_code_lengths.data(), HLIT);
                dist_huffman = compute_huffman(temporary_allocator, LITLEN_DIST_code_lengths.data() + HLIT, HDIST);
            }

            while (true)
            {
                uint32 LITLEN = decode_huffman(decoder, litlen_huffman);
                if (LITLEN < 256)
                {
                    // Copy value (literal byte) to output stream
                    *(decoder->output.cursor) = (uint8) LITLEN;
                    decoder->output.cursor += 1;
                }
                else if (LITLEN == 256) // End of block
                {
                    break;
                }
                else if (LITLEN < 286)
                {
                    auto LEN_index = LITLEN - 257;
                    auto LEN = LEN_bases[LEN_index];
                    if (LEN_extras[LEN_index]) LEN += zlib::get_bits(decoder, LEN_extras[LEN_index]);

                    uint32 DIST_index = decode_huffman(decoder, dist_huffman);
                    auto DIST = DIST_bases[DIST_index];
                    if (DIST_extras[DIST_index]) DIST += zlib::get_bits(decoder, DIST_extras[DIST_index]);

                    uint8 *repeat_cursor = decoder->output.cursor - DIST;
                    for (uint32 repeat = 0; repeat < LEN; repeat++)
                    {
                        *(decoder->output.cursor) = *repeat_cursor;
                        decoder->output.cursor += 1;
                        repeat_cursor += 1;
                    }
                }
                else
                {
                    ASSERT_FAIL("Decoding PNG error!");
                }
            }
        }
        else
        {
            // @debug: BTYPE != 0,1,2 is an error!
            return false;
        }
    }

    return true;
}


uint32 reverse_bits(uint32 t, uint32 n)
{
    uint32 result = 0;
    for (uint32 i = 0; i < n; i++)
    {
        result = result << 1;
        result = result | (t & 1);
        t = t >> 1;
    }
    return result;
}


array<huffman_entry> compute_huffman(memory_allocator *a, uint32 const *code_lengths, usize code_lengths_count)
{
    auto bl_count = make_static_array<17, uint32>();
    bl_count.resize(bl_count.capacity());

    auto next_code = make_static_array<16, uint32>();
    next_code.resize(next_code.capacity());

    struct huffman_table_entry
    {
        uint32 symbol;
        uint32 code_length;
        uint32 code;
    };

    auto huffman_table_buffer = ALLOCATE_BUFFER(*a, code_lengths_count * sizeof(huffman_table_entry));
    auto huffman_table = make_array<huffman_table_entry>(huffman_table_buffer);
    huffman_table.resize(huffman_table.capacity());

    // Count the number of codes for each code length.  Let
    // bl_count[N] be the number of codes of length N, N >= 1.
    {
        for (int i = 0; i < code_lengths_count; i++)
        {
            bl_count[code_lengths[i]] += 1;
        }
    }

    // Find the numerical value of the smallest code for each
    // code length:
    {
        uint32 code = 0;
        bl_count[0] = 0;
        for (int bits = 1; bits < 16; bits++)
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
        for (int symbol = 0; symbol < code_lengths_count; symbol++)
        {
            auto len = code_lengths[symbol];
            if (len > 0)
            {
                huffman_table[symbol].symbol = symbol;
                huffman_table[symbol].code_length = len;
                huffman_table[symbol].code = next_code[len]++;
            }
        }
    }

    uint32 maximum_code_length = 0;
    for (uint32 index = 0; index < code_lengths_count; index++)
    {
        if (code_lengths[index] > maximum_code_length)
        {
            maximum_code_length = code_lengths[index];
        }
    }

    auto huffman_buffer = ALLOCATE_BUFFER(*a, (1ull << maximum_code_length) * sizeof(huffman_entry));
    auto huffman = make_array<huffman_entry>(huffman_buffer);
    huffman.resize(huffman.capacity());
    {
        for (uint32 symbol = 0; symbol < code_lengths_count; symbol++)
        {
            uint32 code_length = huffman_table[symbol].code_length;
            if (code_length == 0) continue;

            uint32 code = huffman_table[symbol].code;
            code = reverse_bits(code, code_length);

            usize A = 0, B = huffman.size();
            for (uint32 i = 0; i < code_length; i++)
            {
                uint32 bit = (code & 1);
                code = (code >> 1);
                if (bit == 0)
                {
                    B = (A + B) / 2;
                }
                else
                {
                    A = (A + B) / 2;
                }
            }
            huffman[A].symbol = symbol;
            huffman[A].code_length = code_length;
            huffman[B - 1] = huffman[A];
        }
    }

    // DEALLOCATE(a, bl_count.data());
    // DEALLOCATE(a, next_code.data());
    // DEALLOCATE(a, huffman_table.data());

    a->deallocate(huffman_table_buffer);

    return huffman;
}


} // namespace image
