#ifndef MEMORY_CRC_HPP
#define MEMORY_CRC_HPP

#include <base.h>

//
// Return the CRC of the bytes buf[0..len-1].
//
uint32 compute_crc(uint8 *buffer, usize len);


#endif // MEMORY_CRC_HPP
