#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <base.hpp>


namespace platform {
    

struct window
{
    uint32 width;
    uint32 height;

    byte data[32];
};


} // namespace platform


#endif // PLATFORM_HPP
