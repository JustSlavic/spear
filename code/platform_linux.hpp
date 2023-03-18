#ifndef PLATFORM_LINUX_HPP
#define PLATFORM_LINUX_HPP

#include <base.hpp>
#include <memory/memory.hpp>

#include <sys/mman.h>
#include <X11/Xlib.h>


namespace gfx::gl {
bool32 create_opengl_window(int32, int32, void *, void *);
}


namespace linux
{


struct window
{
    Display   *x_display;
    Window     x_window;
    Colormap   x_colormap;
};


bool32 create_opengl_window(int32 width, int32 height, void *window, void *driver)
{
    bool32 result = gfx::gl::create_opengl_window(width, height, window, driver);
    return result;
}


INLINE memory_block allocate_memory(void *base_address, usize size)
{
    memory_block result;
    result.memory = mmap(base_address, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    result.size = size;
    ASSERT_MSG(result.memory != MAP_FAILED, "mmap failed");
    return result;
}

INLINE memory_block allocate_memory(usize size)
{
    memory_block result = allocate_memory(NULL, size);
    return result;
}

INLINE void free_memory(memory_block block)
{
    int ec = munmap(block.memory, block.size);
    ASSERT_MSG(ec == 0, "munmap failed");
}


} // namespace linux


#endif // PLATFORM_LINUX_HPP
