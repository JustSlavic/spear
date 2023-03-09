#ifndef PLATFORM_WIN32_HPP
#define PLATFORM_WIN32_HPP

#include <base.hpp>
#include <memory/memory.hpp>

#include <windows.h>


namespace win32
{


uint32 get_program_path(HINSTANCE instance, char *buffer, uint32 buffer_size)
{
    uint32 result_size = GetModuleFileNameA(instance, buffer, buffer_size);
    return result_size;
}


INLINE int32 width(RECT rectangle)
{
    int32 result = rectangle.right - rectangle.left;
    return result;
}

INLINE int32 height(RECT rectangle)
{
    int32 result = rectangle.bottom - rectangle.top;
    return result;
}

INLINE memory_block allocate_memory(void *base_address, usize size)
{
    memory_block result;
    result.memory = VirtualAlloc(base_address, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    result.size = size;
    ASSERT_MSG(result.memory, "VirtualAlloc failed");
    return result;
}

INLINE memory_block allocate_memory(usize size)
{
    memory_block result = allocate_memory(NULL, size);
    return result;
}

INLINE void free_memory(memory_block memory)
{
    bool32 success = VirtualFree(memory.memory, 0, MEM_RELEASE);
    ASSERT_MSG(success, "VirtualFree failed");
}

INLINE uint64 get_file_time(char const *file_path)
{
    WIN32_FILE_ATTRIBUTE_DATA file_data;
    GetFileAttributesExA(file_path, GetFileExInfoStandard, &file_data);
    FILETIME file_time = file_data.ftLastWriteTime;
    uint64 result = ((uint64) file_time.dwLowDateTime) | (((uint64) file_time.dwHighDateTime) << 32);
    return result;
}


struct dll
{
    HMODULE  handle;
    uint64 timestamp;

    template <typename FunctionPointer> [[nodiscard]]
    FunctionPointer get_function(char const *name)
    {
        void *address = GetProcAddress(handle, name);
        return (FunctionPointer) address;
    }

    operator bool ()
    {
        return (bool) handle;
    }
};


dll load_dll(char const *file_path)
{
    dll result;
    result.handle = LoadLibraryA(file_path);
    if (result.handle)
    {
        result.timestamp = get_file_time(file_path);
    }
    return result;
}


void unload_dll(dll *library)
{
    if (library->handle)
    {
        FreeLibrary(library->handle);
        library->handle = NULL;
    }
}


#define THREAD_FUNCTION(NAME) uint32 NAME(void *parameter)
typedef THREAD_FUNCTION(thread_function_t);


} // namespace win32


#endif // PLATFORM_WIN32_HPP
