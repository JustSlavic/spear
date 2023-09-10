#ifndef PLATFORM_LINUX_HPP
#define PLATFORM_LINUX_HPP

#include <base.h>
#include <memory/memory.hpp>
#include <memory/allocator.hpp>

#include <time.hpp>
#include <time.h>
#include <sys/mman.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


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


struct monitor
{
    int32 resolution_x;
    int32 resolution_y;
    int32 refresh_rate;
};


monitor get_monitor_stats(void *w)
{
    auto *linux_window = (window *) w;

    int32 num_sizes = 0;
    XRRScreenSize *xrrs = XRRSizes(linux_window->x_display, 0, &num_sizes);
    XRRScreenConfiguration *conf = XRRGetScreenInfo(linux_window->x_display, linux_window->x_window);

    Rotation current_rotation;
    SizeID current_size_id = XRRConfigCurrentConfiguration(conf, &current_rotation);

    monitor result;
    result.resolution_x = xrrs[current_size_id].width;
    result.resolution_y = xrrs[current_size_id].height;
    result.refresh_rate = XRRConfigCurrentRate(conf);

    return result;
}


[[nodiscard]]
int64 get_wall_clock_frequency()
{
    PERSIST int64 cache;
    if (cache == 0)
    {
        struct timespec ts = {};
        int ec = clock_getres(CLOCK_MONOTONIC, &ts);
        if (ec == -1)
        {
            // @todo: process the system error, I could not run the game when there's undefined clock resolution
            return 0;
        }
        if (ts.tv_sec > 0)
        {
            // @todo: process the error, because I could not run the game when the monotonic clock is so slow
            return 0;
        }
        cache = (int64) 1000000000 / (int64) ts.tv_nsec;
    }
    return cache;
}


[[nodiscard]]
timepoint get_wall_clock()
{
    struct timespec ts = {};
    int ec = clock_gettime(CLOCK_MONOTONIC, &ts);
    if (ec == -1)
    {
        // @todo: process the error, because I could not run the game when the monotonic clock is not ticking
        return timepoint{};
    }

    timepoint result;
    result.counts = ts.tv_sec * 1000000000 + ts.tv_nsec;
    return result;
}


float64 get_seconds(duration d)
{
    float64 result = d.counts / (float64) get_wall_clock_frequency();
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

memory_block load_file(memory::allocator *allocator, char const *filename)
{
    memory_block result = {};

    int fd = open(filename, O_NOFOLLOW, O_RDONLY);
    if (fd < 0)
    {
        return result;
    }

    struct stat st;
    int ec = fstat(fd, &st);
    if (ec < 0)
    {
        return result;
    }

    void *memory = ALLOCATE_BUFFER(allocator, st.st_size);
    uint32 bytes_read = read(fd, memory, st.st_size);

    if (bytes_read < st.st_size)
    {
        DEALLOCATE(allocator, memory);
        return result;
    }

    result.memory = memory;
    result.size   = st.st_size;
    return result;
}


} // namespace linux


#endif // PLATFORM_LINUX_HPP
