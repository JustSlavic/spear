#ifndef PLATFORM_WIN32_HPP
#define PLATFORM_WIN32_HPP

#include <base.hpp>
#include <windows.h>


namespace win32
{


int32_t width(RECT rectangle)
{
    int32_t result = rectangle.right - rectangle.left;
    return result;
}


int32_t height(RECT rectangle)
{
    int32_t result = rectangle.bottom - rectangle.top;
    return result;
}


} // namespace win32


#endif // PLATFORM_WIN32_HPP
