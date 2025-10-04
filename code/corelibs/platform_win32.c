#include "platform.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


struct dll
{
    HMODULE handle;
};

void *platform_allocate_pages(void *base, usize size)
{
    void *result = VirtualAlloc(base, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    return result;
}

usize platform_get_file_size(char const *filename)
{
    usize Result = 0;
    HANDLE FileHandle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        BOOL GetSizeResult = GetFileSizeEx(FileHandle, &FileSize);
        if (GetSizeResult)
        {
            Result = FileSize.QuadPart;
        }
        CloseHandle(FileHandle);
    }
    return Result;
}

uint32 platform_read_file_into_memory(char const *filename, void *data, usize size)
{
    DWORD BytesRead = 0;
    HANDLE FileHandle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        ReadFile(FileHandle, data, (DWORD) size, &BytesRead, NULL);
        CloseHandle(FileHandle);
    }
    return BytesRead;
}

void platform_dll_open(dll *dll, char const *path)
{
    dll->handle = LoadLibraryA(path);
}

void platform_dll_close(dll *dll)
{
    FreeLibrary(dll->handle);
    dll->handle = NULL;
}

bool32 platform_dll_is_valid(dll *dll)
{
    return dll->handle != NULL;
}

void *platform_dll_get_function(dll *dll, char const *function_name)
{
    void *result = (void *) GetProcAddress(dll->handle, function_name);
    return result;
}

timestamp_t platform_file_get_last_modified_time(char const *filename)
{
    timestamp_t result = 0;
    WIN32_FILE_ATTRIBUTE_DATA file_data;
    BOOL success = GetFileAttributesExA(filename, GetFileExInfoStandard, &file_data);
    if (success)
    {
        FILETIME file_time = file_data.ftLastWriteTime;
        result = (timestamp_t) ((uint64) file_time.dwLowDateTime) | (((uint64) file_time.dwHighDateTime) << 32);
    }
    return result;
}

int64 platform_clock_frequency_get(void)
{
    static int64 cache;
    if (cache == 0)
    {
        LARGE_INTEGER performance_counter_frequency;
        QueryPerformanceFrequency(&performance_counter_frequency);
        cache = performance_counter_frequency.QuadPart;
    }
    return cache;
}

timepoint platform_clock_now(void)
{
    LARGE_INTEGER PerformanceCounter;
    QueryPerformanceCounter(&PerformanceCounter);
    timepoint result = { (uint64) PerformanceCounter.QuadPart * 1000000 / platform_clock_frequency_get() };
    return result;
}
