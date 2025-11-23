#ifndef MEMORY_CRC_HPP
#define MEMORY_CRC_HPP

#include <corelibs/base.h>

//
// Return the CRC of the bytes buf[0..len-1].
//
uint32 crc_compute(uint8 *buffer, usize len);


#endif // MEMORY_CRC_HPP
