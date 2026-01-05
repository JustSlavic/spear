#include "platform.h"

#include <sys/mman.h> // mmap
#include <dlfcn.h> // dlopen
#include <sys/stat.h> // stat
#include <fcntl.h> // open, close
#include <unistd.h> // read


void *platform_allocate_pages(void *base, usize size)
{
    void *result = mmap(base, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return result;
}

usize platform_get_file_size(char const *filename)
{
    usize result = 0;
    struct stat st = {};
    if (stat(filename, &st) == 0)
    {
        result = st.st_size;
    }
    return result;
}

uint32 platform_read_file_into_memory(char const *filename, void *memory, usize size)
{
    int fd = open(filename, O_NOFOLLOW, O_RDONLY);
    if (fd)
    {
        uint32 bytes_read = read(fd, memory, size);
        close(fd);
        return bytes_read;
    }
    return 0;
}

void platform_dll_open(dll *dll, char const *path)
{
    int flags = 0;
    dll->handle = dlopen(path, flags);
}

void platform_dll_close(dll *dll)
{
    dlclose(dll->handle);
    dll->handle = NULL;
}

bool32 platform_dll_is_valid(dll *dll)
{
    return dll->handle != NULL;
}

void *platform_dll_get_function(dll *dll, char const *function_name)
{
    void *result = dlsym(dll->handle, function_name);
    return result;
}

timestamp_t platform_file_get_last_modified_time(char const *filename)
{
    struct stat file_info;
    stat(filename, &file_info);
    return file_info.st_mtime;
}

int64 platform_clock_frequency_get(void)
{
    static int64 cache;
    if (cache == 0)
    {
        struct timespec ts = {};
        int ec = clock_getres(CLOCK_MONOTONIC, &ts);
        if (ec == -1)
        {
            // @todo: process the system error, I could not run the game when there's undefined clock resolution
        }
        else if (ts.tv_sec > 0)
        {
            // @todo: process the error, because I could not run the game when the monotonic clock is so slow
        }
        else
        {
            cache = 1000000000ll / (int64) ts.tv_nsec;
        }
    }
    return cache;
}

timepoint platform_clock_now(void)
{
    timepoint result = {};

    struct timespec ts = {};
    int ec = clock_gettime(CLOCK_MONOTONIC, &ts);
    if (ec < 0)
    {
        // @todo: process the error, because I could not run the game when the monotonic clock is not ticking
    }
    else
    {
        result.microseconds = (uint64) ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
    }

    return result;
}
