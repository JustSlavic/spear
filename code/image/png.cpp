#include "png.hpp"
#include <math/integer.hpp>
#include <memory/crc.hpp>


//
// PNG (Portable Network Graphics) Specification, Version 1.2
//   http://www.libpng.org/pub/png/spec/1.2/PNG-Contents.html
//
// DEFLATE Compressed Data Format Specification version 1.3:
//   https://www.ietf.org/rfc/rfc1951.txt
//
// ZLIB Compressed Data Format Specification version 3.3
//   https://www.ietf.org/rfc/rfc1950.txt
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

enum png_interlace_method : uint8 {
    PNG_INTERLACE_NONE  = 0,
    PNG_INTERLACE_ADAM7 = 1,
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


struct zlib_decoder
{
    bool32 initialized;

    memory_block input_stream;
    memory_block output_stream;

    uint8 *input_cursor;
    uint8 *output_cursor;

    uint32 bits;
    uint32 bits_available;
};

uint32 get_bits_unsafe(zlib_decoder *decoder, uint32 n)
{
    uint32 result = decoder->bits & ((1 << n) - 1);
    decoder->bits = decoder->bits >> n;
    decoder->bits_available = decoder->bits_available - n;
    return result;
}

uint32 get_bits(zlib_decoder *decoder, uint32 requested_bits)
{
    ASSERT(requested_bits <= 32);

    uint32 result = 0;
    uint32 fetched_bits = 0;
    uint32 remaining_bits = requested_bits;

    do
    {
        if (decoder->bits_available < remaining_bits)
        {
            if (decoder->bits_available > 0)
            {
                uint32 bits_available = decoder->bits_available;
                result = result | (get_bits_unsafe(decoder, decoder->bits_available) << fetched_bits);

                fetched_bits   += bits_available;
                remaining_bits -= bits_available;
            }
        }
        else
        {
            result = result | (get_bits_unsafe(decoder, remaining_bits) << fetched_bits);

            fetched_bits   += remaining_bits;
            remaining_bits -= remaining_bits;
        }

        while (decoder->bits_available < 24)
        {
            decoder->bits = decoder->bits | ((*decoder->input_cursor++) << decoder->bits_available);
            decoder->bits_available += 8;
        }
    }
    while (fetched_bits < requested_bits);

    return result;
}

void skip_byte(zlib_decoder *decoder)
{
    get_bits_unsafe(decoder, decoder->bits_available % 8);
}

namespace png {

void *consume_memory(uint8 **data, usize size) {
    void *result = *data;
    *data += size;
    return result;
}

using crc_t = uint32;

} // namespace png

#define PNG_CONSUME_STRUCT(POINTER, TYPE) (TYPE *)png::consume_memory(&POINTER, sizeof(TYPE))

bool32 decode_idat_chunk(zlib_decoder *decoder, memory::allocator *temporary_allocator);
bitmap load_png(memory::allocator *allocator, memory::allocator *temporary, memory_block contents)
{
    bitmap result = {};

    uint8 *data = (uint8 *) contents.memory;

    uint32 signature1 = *PNG_CONSUME_STRUCT(data, uint32);
    uint32 signature2 = *PNG_CONSUME_STRUCT(data, uint32);

    if (signature1 != PNG_SIGN_1 || signature2 != PNG_SIGN_2)
    {
        // @todo: log "Could not load PNG file from data"
        return result;
    }

    png__rendering_intent *rendering_intent;
    float64 gamma = 0.f;
    zlib_decoder decoder = {};

    bool32 is_end = false;
    while ((is_end == false) && (data < ((uint8 *) contents.memory + contents.size)))
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
        chunk_header.size_of_data = change_endianness(chunk_header.size_of_data);

        osOutputDebugString("%.*s (%u bytes):\n", 4, (char const *) &chunk_header.type, chunk_header.size_of_data);

        auto chunk_type_size = sizeof(chunk_header.type);
        png::crc_t computed_crc = change_endianness(compute_crc(data - chunk_type_size, chunk_header.size_of_data + chunk_type_size));

        if (chunk_header.type == PNG_IHDR_ID)
        {
            png__ihdr_header *ihdr = PNG_CONSUME_STRUCT(data, png__ihdr_header);

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

            result.width  = change_endianness(ihdr->width);
            result.height = change_endianness(ihdr->height);
            result.size = result.width * result.height * (result.bits_per_pixel / 8);
            result.pixels = (uint8 *) ALLOCATE_BUFFER_(allocator, result.size);

            osOutputDebugString("image: %dx%d %d bits per pixel\n", result.width, result.height, result.bits_per_pixel);

            decoder.output_stream = memory_block{result.pixels, result.size};
            decoder.output_cursor = result.pixels;
        }
        else if (chunk_header.type == PNG_IDAT_ID)
        {
            decoder.input_stream = memory_block{data, chunk_header.size_of_data};
            decoder.input_cursor = data;

            decode_idat_chunk(&decoder, temporary);
            data += chunk_header.size_of_data;
        }
        else if (chunk_header.type == PNG_IEND_ID)
        {
            // IEND chunk has no data.
            is_end = true;
        }
        else if (chunk_header.type == PNG_sRGB_ID)
        {
            rendering_intent = PNG_CONSUME_STRUCT(data, png__rendering_intent);
            // sRGB chunk has no data.
        }
        else if (chunk_header.type == PNG_gAMA_ID)
        {
            uint32 gamma_ = change_endianness(*PNG_CONSUME_STRUCT(data, uint32));
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

    return result;
}

struct huffman_entry
{
    uint8 symbol;
    uint8 code_length;
};

array<huffman_entry> compute_huffman(zlib_decoder *decoder,
                     memory::allocator *temporary_allocator,
                     uint32 *code_lengths,
                     usize code_lengths_count);
uint32 decode_litlen(zlib_decoder *decoder) { return 0; }
uint32 decode_distance(zlib_decoder *decoder) { return 0; }

bool32 decode_idat_chunk(zlib_decoder *decoder, memory::allocator *temporary_allocator)
{
    if (not decoder->initialized)
    {
        decoder->initialized = true;

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

        osOutputDebugString("    FCHECK = %d\n", FCHECK);
        osOutputDebugString("    FDICT  = %d\n", FDICT );
        osOutputDebugString("    FLEVEL = %d\n", FLEVEL);

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
    }

    uint8 BFINAL = 0;
    uint8 BTYPE;

    while (BFINAL == 0)
    {
        BFINAL = (uint8) get_bits(decoder, 1); // ((*zlib_data) & 0b0000'0001);
        BTYPE  = (uint8) get_bits(decoder, 2); // ((*zlib_data) & 0b0000'0110) >> 1;

        osOutputDebugString("    BFINAL = %d\n", BFINAL);
        osOutputDebugString("    BTYPE  = %d\n", BTYPE);

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

            if (LEN != ~NLEN)
            {
                // @todo: something wrong
                ASSERT_FAIL();
                return false;
            }

            for (int i = 0; i < LEN; i++) get_bits(decoder, 8);
        }
        else if ((BTYPE == 1) || (BTYPE == 2))
        {
            if (BTYPE == 1)
            {
                // Compressed with fixed Huffman code
            }
            if (BTYPE == 2)
            {
                // Compressed with dynamic Huffman code

                uint32 HLIT = get_bits(decoder, 5);
                uint32 HDIST = get_bits(decoder, 5);
                uint32 HCLEN = get_bits(decoder, 4);

                uint32 number_of_LITLEN_codes = HLIT + 257;
                uint32 number_of_DIST_codes = HDIST + 1;
                uint32 number_of_code_length_codes = HCLEN + 4;

                uint32 code_lengths_swizzle[] =
                {
                    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
                };
                uint32 code_lengths[ARRAY_COUNT(code_lengths_swizzle)] = {};

                for (int i = 0; i < ARRAY_COUNT(code_lengths); i++)
                {
                    code_lengths[code_lengths_swizzle[i]] = get_bits(decoder, 3);
                }

                auto clen_huffman = compute_huffman(decoder, temporary_allocator, code_lengths, ARRAY_COUNT(code_lengths));

                // Decoding the LITLEN code lengths...
                auto LITLEN_code_lengths = ALLOCATE_ARRAY_OPEN(temporary_allocator, uint32, number_of_LITLEN_codes);

                for (uint32 index = 0; index < number_of_LITLEN_codes; index++)
                {
                    uint8 bits = (uint8) (decoder->bits & 0xff);

                    auto h_entry = clen_huffman[bits];
                    osOutputDebugString("        h_entry = {%d, %d}\n", h_entry.symbol, h_entry.code_length);

                    get_bits(decoder, h_entry.code_length);
                }
            }

            while (true)
            {
                uint32 LITLEN = decode_litlen(decoder);
                if (LITLEN < 256)
                {
                    // Copy value (literal byte) to output stream
                }
                else if (LITLEN == 256) // End of block
                {
                    break;
                }
                else
                {
                    uint32 DIST = decode_distance(decoder);
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

array<huffman_entry> compute_huffman(zlib_decoder *decoder, memory::allocator *a, uint32 *code_lengths, usize code_lengths_count)
{
    auto bl_count = ALLOCATE_ARRAY_OPEN(a, uint32, code_lengths_count);
    auto next_code = ALLOCATE_ARRAY_OPEN(a, uint32, code_lengths_count);
    struct huffman_table_entry
    {
        uint32 symbol;
        uint32 code_length;
        uint32 code;
    };
    auto huffman_table = ALLOCATE_ARRAY_OPEN(a, huffman_table_entry, code_lengths_count);
    auto huffman = ALLOCATE_ARRAY_OPEN(a, huffman_entry, 0xffu);

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
        for (int bits = 1; bits < code_lengths_count; bits++)
        {
            code = (code + bl_count[bits-1]) << 1;
            next_code[bits] = code;
        }
    }

    {
        for (int i = 0; i < code_lengths_count; i++)
        {
            auto len = code_lengths[i];
            if (len > 0)
            {
                huffman_table[i].symbol = i;
                huffman_table[i].code_length = len;
                huffman_table[i].code = next_code[len]++;
            }
        }
    }

    {
        for (int symbol = 0; symbol < huffman_table.size(); symbol++)
        {
            uint32 code = huffman_table[symbol].code;
            uint32 len = huffman_table[symbol].code_length;
            uint32 important_bits = ((1u << len) - 1);

            if (len > 0) for (int index = 0; index < huffman.size(); index++)
            {
                if ((index & important_bits) == code)
                {
                    huffman[index].symbol = (uint8) huffman_table[symbol].symbol;
                    huffman[index].code_length = (uint8) len;
                }
            }
        }
    }

    DEALLOCATE(a, bl_count.data());
    DEALLOCATE(a, next_code.data());
    DEALLOCATE(a, huffman_table.data());

    return huffman;
}


} // namespace image
