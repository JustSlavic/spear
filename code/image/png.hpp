#ifndef IMAGE_PNG_HPP
#define IMAGE_PNG_HPP

#include <base.h>
#include <memory.h>
#include <image/bitmap.hpp>


namespace image {


bitmap load_png(memory_allocator allocator, memory_allocator temporary, memory_buffer memory);


} // namespace image


#endif // IMAGE_PNG_HPP
