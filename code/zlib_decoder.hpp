#include <base.hpp>
#include <memory/memory.hpp>


namespace zlib {


struct decoder
{
    bool32 initialized;

    memory_block input_stream;
    memory_block output_stream;

    uint8 *input_cursor;
    uint8 *output_cursor;

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
    if (z->bits_available < 24)
    {
        uint32 byte = *z->input_cursor++;
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
