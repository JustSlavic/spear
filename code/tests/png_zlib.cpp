#include <unit_testing.hpp>
#include <zlib_decoder.hpp>


TEST(BitFetcher)
{
    // 16 bits
    {
        uint8 data[] = {
            0x35,
            0x35,
            0x8f,
            0x88,
            0x7c,
            0xd7,
            0xfb,
            0x4d,
            0xb2,
            0xd8,
            0xec,
            0xe6,
            0xdc,
            0x3b,
            0x77,
        };

        zlib::decoder decoder;
        decoder.input_stream = memory_block{ data, sizeof(data) };
        decoder.input_cursor = (uint8 *) decoder.input_stream.memory;

        decoder.bits = 0;
        decoder.bits_available = 0;

        for (int i = 0; i < sizeof(data); i++)
        {
            uint32 bits = 0;
            uint32 shift = 0;

            for (int j = 0; j < 4; j++)
            {
                uint32 bit = get_bits(&decoder, 2);

                bits = bits | ((bit & 1) << shift++);
                bit >>= 1;
                bits = bits | ((bit & 1) << shift++);
            }

            printf("0x%x ", bits);
        }
        printf("\n");
    }
}


int main()
{
    TEST_BEGIN();

    TEST_RUN(BitFetcher);

    TEST_END();
    return 0;
}
