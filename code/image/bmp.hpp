#ifndef IMAGE_BMP_HPP
#define IMAGE_BMP_HPP

#include <base.hpp>
#include <memory/memory.hpp>
#include <image/bitmap.hpp>


namespace image {


bitmap load_bmp(char const *filename);
bitmap load_bmp(memory::allocator *allocator, memory_block memory);


} // namespace image


#endif // IMAGE_BMP_HPP
