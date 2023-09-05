#include <unit_testing.hpp>
#include <zlib_decoder.hpp>


TEST(BitFetcher)
{
    uint8 data[] = {
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
        0xaa,
    };

    zlib::decoder decoder;
    decoder.input = zlib::create_stream(data, sizeof(data));

    decoder.bits = 0;
    decoder.bits_available = 0;

    uint32 bits;

    bits = get_bits(&decoder, 1);
    TEST_ASSERT_EQ(bits, 0b0);

    bits = get_bits(&decoder, 2);
    TEST_ASSERT_EQ(bits, 0b01);

    bits = get_bits(&decoder, 3);
    TEST_ASSERT_EQ(bits, 0b101);

    bits = get_bits(&decoder, 4);
    TEST_ASSERT_EQ(bits, 0b1010);

    bits = get_bits(&decoder, 5);
    TEST_ASSERT_EQ(bits, 0b01010);

    bits = get_bits(&decoder, 6);
    TEST_ASSERT_EQ(bits, 0b010101);

    bits = get_bits(&decoder, 7);
    TEST_ASSERT_EQ(bits, 0b1010101);

    bits = get_bits(&decoder, 8);
    TEST_ASSERT_EQ(bits, 0b10101010);

    bits = get_bits(&decoder, 9);
    TEST_ASSERT_EQ(bits, 0b010101010);

    bits = get_bits(&decoder, 10);
    TEST_ASSERT_EQ(bits, 0b0101010101);

    bits = get_bits(&decoder, 11);
    TEST_ASSERT_EQ(bits, 0b10101010101);
}


int main()
{
    TEST_BEGIN();

    TEST_RUN(BitFetcher);

    TEST_END();
    return 0;
}
