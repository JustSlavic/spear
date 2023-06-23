#ifndef IMAGE_PNG_HPP
#define IMAGE_PNG_HPP

#include <base.hpp>
#include <memory/memory.hpp>
#include <image/bitmap.hpp>


namespace image {


bitmap load_png(char const *filename);
bitmap load_png(memory::allocator *allocator, memory::allocator *temporary, memory_block memory);


} // namespace image


#endif // IMAGE_PNG_HPP
