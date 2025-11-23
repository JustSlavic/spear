#include "crc.h"

//
// Reference: http://www.libpng.org/pub/png/spec/1.2/PNG-CRCAppendix.html
//

//
// Table of CRCs of all 8-bit messages.
//
static uint32 crc_table[256];
static bool32 crc_table_computed = false;

//
// Make the table for a fast CRC.
//
void crc_table_make()
{
    uint32 n, k;
    for (n = 0; n < 256; n++)
    {
        uint32 c = n;
        for (k = 0; k < 8; k++)
        {
            if (c & 1)
            {
                // polynomial: 1+x+x^2+x^4+x^5+x^7+x^8+x^10+x^11+x^12+x^16+x^22+x^23+x26+x^32
                // 1110'1101'1011'1000'1000'0011'0010'0000
                // Note: x^32 is not written explicitly, it is implied; To account this last 1 bit
                //       we take 1's compliment of the result.
                uint32 polynomial = 0xedb88320; // Hex representation of this polynomial
                c = polynomial ^ (c >> 1);
            }
            else
            {
                c = c >> 1;
            }
        }
        crc_table[n] = c;
    }
    crc_table_computed = true;
}

//
// Update a running CRC with the bytes buf[0..len-1] -- the CRC
// should be initialized to all 1's, and the transmitted value
// is the 1's complement of the final running CRC (see the crc() routine below).
//
uint32 crc_update(uint32 init, uint8 *buf, usize len)
{
    uint32 n;
    uint32 crc = init;

    if (!crc_table_computed)
    {
        crc_table_make();
    }

    for (n = 0; n < len; n++)
    {
        crc = crc_table[(crc ^ buf[n]) & 0xff] ^ (crc >> 8);
        //              ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        //                   What is going on here???
    }

    return crc;
}

//
// Return the CRC of the bytes buf[0..len-1].
//
uint32 crc_compute(uint8 *buf, usize len)
{
    return crc_update(0xffffffff, buf, len) ^ 0xffffffff;
    //                ^^^^^^^^^^              ^^^^^^^^^^
    //                Initialize all 1's      Take 1's complement of the result
}
