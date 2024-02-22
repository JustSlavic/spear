#ifndef IMAGE_BMP_HPP
#define IMAGE_BMP_HPP

#include <base.h>
#include <memory.h>

#include <image/bitmap.hpp>


namespace image {


bitmap load_bmp(memory_allocator a, memory_buffer memory);


} // namespace image


#endif // IMAGE_BMP_HPP
