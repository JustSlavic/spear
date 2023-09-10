#include "bmp.hpp"


namespace image {


#pragma pack(push, 1)
struct bmp_header
{
    uint16 signature;
    uint32 filesize;
    uint32 reserved; // always 0
    uint32 data_offset;
};
#pragma pack(pop)

enum bmp_compression : uint32
{
    BMP_BI_RGB  = 0, // no compression
    BMP_BI_RLE8 = 1, // 8bit RLE encoding
    BMP_BI_RLE4 = 2, // 4bit RLE encoding
};

struct bmp_info_header
{
    uint32 size; // of this header (always 40)
    uint32 width;
    uint32 height;
    uint16 planes; // always 1
    uint16 bits_per_pixel;
    bmp_compression compression;
    uint32 image_size; // can be 0 if compression is BI_RGB
    uint32 x_pixels_per_meter;
    uint32 y_pixels_per_meter;
    uint32 color_used; // for 8bit per pixel bitmap it will be equal to 256
    uint32 important_colors; // 0 - all colors are important
};

// Can be ignored since we use only not compressed BI_RGB images
struct bmp_color_table
{
    uint8 red;
    uint8 green;
    uint8 blue;
    uint8 reserved; // always 0
};


#define BMP_GET_DATA(MEMORY, TYPE) (TYPE *) data; data += sizeof(TYPE)
#define BMP_MAGIC_NUMBER(a, b) ((((uint32)a) << 0) | (((uint32)b) << 8))
enum { BMP_SIGNATURE = BMP_MAGIC_NUMBER('B', 'M') };


bitmap load_bmp(memory::allocator *allocator, memory_block raw_data)
{
    bitmap result = {};

    uint8 *data = (uint8 *) raw_data.memory;

    auto *header = BMP_GET_DATA(data, bmp_header);
    if ((header->signature != BMP_SIGNATURE) ||
        (header->reserved != 0))
    {
        return result;
    }

    auto *info = BMP_GET_DATA(data, bmp_info_header);
    if ((info->planes != 1) ||
        (info->image_size == 0))
    {
        return result;
    }

    uint32 *pixels = (uint32 *) (((uint8 *) raw_data.memory) + header->data_offset);

    result.pixels = (uint8 *) ALLOCATE_BUFFER_(allocator, info->image_size);
    memory::copy(result.pixels, pixels, info->image_size);
    result.size   = info->image_size;
    result.width  = info->width;
    result.height = info->height;
    result.bits_per_pixel = info->bits_per_pixel;
    result.color_type = IMAGE_BGR;
    result.top_down = false;

    return result;
}


} // namespace image
