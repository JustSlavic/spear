#ifndef PLATFORM_LINUX_HPP
#define PLATFORM_LINUX_HPP

#include <base.h>
#include <memory.h>
#include <memory_allocator.h>

#include <time.hpp>
#include <time.h>
#include <sys/mman.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <input.hpp>


#define KEYCODE_ESC                  9
#define KEYCODE_Q                    24
#define KEYCODE_W                    25
#define KEYCODE_E                    26
#define KEYCODE_R                    27
#define KEYCODE_T                    28
#define KEYCODE_Y                    29
#define KEYCODE_U                    30
#define KEYCODE_I                    31
#define KEYCODE_O                    32
#define KEYCODE_P                    33
#define KEYCODE_BRACKET_OPEN         34
#define KEYCODE_BRACKET_CLOSE        35
#define KEYCODE_A                    38
#define KEYCODE_S                    39
#define KEYCODE_D                    40
#define KEYCODE_Z                    52
#define KEYCODE_SPACE                65
#define KEYCODE_F1                   67
#define KEYCODE_F2                   68
#define KEYCODE_F3                   69
#define KEYCODE_F4                   70
#define KEYCODE_F5                   71
#define KEYCODE_F6                   72
#define KEYCODE_F7                   73
#define KEYCODE_F8                   74
#define KEYCODE_F9                   75
#define KEYCODE_F10                  76
#define KEYCODE_F11                  95
#define KEYCODE_F12                  96
#define KEYCODE_UP                   111
#define KEYCODE_LEFT                 113
#define KEYCODE_RIGHT                114
#define KEYCODE_DOWN                 116
#define KEYCODE_CTRL                 37
#define KEYCODE_SHIFT                50


bool32 gl__create_window(int32, int32, void *, void *);


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
    result.memory = (byte *) mmap(base_address, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
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

memory_block load_file(memory_allocator allocator, char const *filename)
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

    auto block = ALLOCATE_BUFFER(allocator, st.st_size);
    uint32 bytes_read = read(fd, block.memory, st.st_size);

    if (bytes_read < st.st_size)
    {
        DEALLOCATE(allocator, block);
        return result;
    }

    result = block;
    return result;
}

keyboard_key map_button_from_virtual_key_code(uint32 vk)
{
    switch (vk)
    {
        case KEYCODE_ESC: return KB_ESC; break;
        case KEYCODE_Q: return KB_Q; break;
        case KEYCODE_W: return KB_W; break;
        case KEYCODE_E: return KB_E; break;
        case KEYCODE_R: return KB_R; break;
        case KEYCODE_T: return KB_T; break;
        case KEYCODE_Y: return KB_Y; break;
        case KEYCODE_U: return KB_U; break;
        case KEYCODE_I: return KB_I; break;
        case KEYCODE_O: return KB_O; break;
        case KEYCODE_P: return KB_P; break;
        case KEYCODE_BRACKET_OPEN: return KB_BRACKET_OPEN; break;
        case KEYCODE_BRACKET_CLOSE: return KB_BRACKET_CLOSE; break;
        case KEYCODE_A: return KB_A; break;
        case KEYCODE_S: return KB_S; break;
        case KEYCODE_D: return KB_D; break;
        case KEYCODE_Z: return KB_Z; break;
        case KEYCODE_SPACE: return KB_SPACE; break;
        case KEYCODE_F1: return KB_F1; break;
        case KEYCODE_F2: return KB_F2; break;
        case KEYCODE_F3: return KB_F3; break;
        case KEYCODE_F4: return KB_F4; break;
        case KEYCODE_F5: return KB_F5; break;
        case KEYCODE_F6: return KB_F6; break;
        case KEYCODE_F7: return KB_F7; break;
        case KEYCODE_F8: return KB_F8; break;
        case KEYCODE_F9: return KB_F9; break;
        case KEYCODE_F10: return KB_F10; break;
        case KEYCODE_F11: return KB_F11; break;
        case KEYCODE_F12: return KB_F12; break;
        case KEYCODE_CTRL: return KB_CTRL; break;
        case KEYCODE_SHIFT: return KB_SHIFT; break;
    }

    return KB_NONE;
}


} // namespace linux


#endif // PLATFORM_LINUX_HPP
