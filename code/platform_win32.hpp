#ifndef PLATFORM_WIN32_HPP
#define PLATFORM_WIN32_HPP

#include <base.hpp>
#include <platform.hpp>
#include <memory/memory.hpp>
#include <memory/allocator.hpp>
#include <time.hpp>

#include <windows.h>


#define MAIN_WINDOW_CALLBACK(NAME) LRESULT CALLBACK NAME(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
typedef MAIN_WINDOW_CALLBACK(MainWindowCallbackType);

namespace gfx::gl {
bool32 create_opengl_window(HINSTANCE Instance, int32 ClientWidth, int32 ClientHeight, MainWindowCallbackType *WindowCallback, void *w);
}

namespace win32 {

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

struct window
{
    HWND handle;
    HDC device_context;
};

bool32 create_opengl_window(HINSTANCE Instance, int32 ClientWidth, int32 ClientHeight, MainWindowCallbackType *WindowCallback, window *w)
{
    bool32 result = gfx::gl::create_opengl_window(Instance, ClientWidth, ClientHeight, WindowCallback, w);
    return result;
}

bool32 create_simple_window(HINSTANCE Instance, int32 ClientWidth, int32 ClientHeight, MainWindowCallbackType *WindowCallback, window *w)
{
    auto *result = (window *) w;

    int32 PrimaryMonitorWidth  = GetSystemMetrics(SM_CXSCREEN);
    int32 PrimaryMonitorHeight = GetSystemMetrics(SM_CYSCREEN);

    HBRUSH BlackBrush = CreateSolidBrush(RGB(0, 0, 0));

    WNDCLASSA WindowClass {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "WindowClass";
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hbrBackground = BlackBrush;

    ATOM WindowClassAtom = RegisterClassA(&WindowClass);
    if (!WindowClassAtom)
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! Could not register window class.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return false;
    }

    RECT WindowRectangle = { 0, 0, ClientWidth, ClientHeight };
    if (!AdjustWindowRect(&WindowRectangle, WS_OVERLAPPEDWINDOW, false))
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! AdjustWindowRect failed.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return false;
    }

    HWND window = CreateWindowExA(
        0, WindowClass.lpszClassName,
        "window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        (PrimaryMonitorWidth - width(WindowRectangle)) / 2,
        (PrimaryMonitorHeight - height(WindowRectangle)) / 2,
        width(WindowRectangle),
        height(WindowRectangle),
        0, 0, Instance, NULL);

    if (!window)
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "System error! Could not create a window.", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return false;
    }

    result->handle = window;
    result->device_context = GetDC(window);

    return true;
}

void get_mouse_pos(window *wnd, int32 *x, int32 *y)
{
    POINT pos;
    GetCursorPos(&pos);
    ScreenToClient(wnd->handle, &pos);
    *x = pos.x;
    *y = pos.y;
}

uint64 get_processor_cycles()
{
    return __rdtsc();
}

[[nodiscard]]
int64 get_wall_clock_frequency()
{
    PERSIST int64 cache;
    if (cache == 0)
    {
        LARGE_INTEGER performance_counter_frequency;
        // @note: this always succeeds on WinXP and higher
        QueryPerformanceFrequency(&performance_counter_frequency);
        cache = performance_counter_frequency.QuadPart;
    }
    return cache;
}

[[nodiscard]]
timepoint get_wall_clock()
{
    LARGE_INTEGER PerformanceCounter;
    // @note: this always succeeds on WinXP and higher
    QueryPerformanceCounter(&PerformanceCounter);
    return { (uint64) PerformanceCounter.QuadPart };
}


float64 get_seconds(timepoint t)
{
    float64 result = t.counts / (float64) get_wall_clock_frequency();
    return result;
}


float64 get_seconds(duration d)
{
    float64 result = d.counts / (float64) get_wall_clock_frequency();
    return result;
}


uint32 get_program_path(HINSTANCE instance, char *buffer, uint32 buffer_size)
{
    uint32 result_size = GetModuleFileNameA(instance, buffer, buffer_size);
    return result_size;
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

INLINE memory_block load_file(memory::allocator *allocator, char const *filename)
{
    memory_block result = {};

    HANDLE FileHandle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle == INVALID_HANDLE_VALUE)
    {
        return result;
    }

    LARGE_INTEGER FileSize;
    BOOL GetSizeResult = GetFileSizeEx(FileHandle, &FileSize);
    if (GetSizeResult == FALSE)
    {
        return result;
    }

    void *Memory = ALLOCATE_BUFFER(allocator, FileSize.QuadPart);

    DWORD BytesRead;
    BOOL ReadFileResult = ReadFile(FileHandle, Memory, (DWORD) FileSize.QuadPart, &BytesRead, NULL);

    if (ReadFileResult == FALSE)
    {
        DEALLOCATE(allocator, Memory);
        return result;
    }

    result.memory = Memory;
    result.size   = FileSize.QuadPart;
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
