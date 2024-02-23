#include <base.h>
#include <input.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#define MAIN_WINDOW_CALLBACK(NAME) LRESULT CALLBACK NAME(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
typedef MAIN_WINDOW_CALLBACK(MainWindowCallbackType);


namespace gl
{
    bool create_window(HINSTANCE Instance, int32 ClientWidth, int32 ClientHeight, MainWindowCallbackType *WindowCallback, void *w);
}

namespace win32
{

int32 width(RECT rectangle)
{
    int32 result = rectangle.right - rectangle.left;
    return result;
}

int32 height(RECT rectangle)
{
    int32 result = rectangle.bottom - rectangle.top;
    return result;
}

struct window
{
    HWND handle;
    HDC device_context;
};

void create_opengl_window(HINSTANCE instance, int32 width, int32 height, MainWindowCallbackType cb, window *w)
{
    gl::create_window(instance, width, height, cb, w);
}

uint32 get_program_path(HINSTANCE instance, char *buffer, uint32 buffer_size)
{
    uint32 result_size = GetModuleFileNameA(instance, buffer, buffer_size);
    return result_size;
}

uint64 get_file_time(char const *file_path)
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

    static dll load(char const *file_path)
    {
        dll result;
        result.handle = LoadLibraryA(file_path);
        if (result.handle)
        {
            result.timestamp = get_file_time(file_path);
        }
        return result;
    }

    void unload()
    {
        if (handle)
        {
            FreeLibrary(handle);
            handle = NULL;
        }
    }

    [[nodiscard]]
    void *get_function(char const *name)
    {
        void *address = GetProcAddress(handle, name);
        return address;
    }
};

memory_buffer allocate_memory(void *base_address, usize size)
{
    memory_buffer result;
    result.data = (byte *) VirtualAlloc(base_address, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    result.size = size;
    return result;
}

keyboard_key map_button_from_virtual_key_code(uint32 vk)
{
    switch (vk)
    {
        case VK_ESCAPE: return KB_ESC; break;
        case VK_F1: return KB_F1; break;
        case VK_SPACE: return KB_SPACE; break;
        case VK_SHIFT: return KB_SHIFT; break;
        case VK_CONTROL: return KB_CTRL; break;
        case 'Q': return KB_Q; break;
        case 'W': return KB_W; break;
        case 'E': return KB_E; break;
        case 'R': return KB_R; break;
        case 'T': return KB_T; break;
        case 'Y': return KB_Y; break;
        case 'U': return KB_U; break;
        case 'I': return KB_I; break;
        case 'O': return KB_O; break;
        case 'P': return KB_P; break;
        case 'A': return KB_A; break;
        case 'S': return KB_S; break;
        case 'D': return KB_D; break;
        case 'F': return KB_F; break;
        case 'G': return KB_G; break;
        case 'H': return KB_H; break;
        case 'J': return KB_J; break;
        case 'K': return KB_K; break;
        case 'L': return KB_L; break;
        case 'Z': return KB_Z; break;
        case 'X': return KB_X; break;
        case 'C': return KB_C; break;
        case 'V': return KB_V; break;
        case 'B': return KB_B; break;
        case 'N': return KB_N; break;
        case 'M': return KB_M; break;
    }
    return KB_NONE;
}

struct wall_clock
{
    [[nodiscard]]
    static timepoint now()
    {
        LARGE_INTEGER PerformanceCounter;
        QueryPerformanceCounter(&PerformanceCounter);
        timepoint result = { (uint64) PerformanceCounter.QuadPart * 1000000 / frequency() };
        return result;
    }

    [[nodiscard]]
    static int64 frequency()
    {
        PERSIST int64 cache;
        if (cache == 0)
        {
            LARGE_INTEGER performance_counter_frequency;
            QueryPerformanceFrequency(&performance_counter_frequency);
            cache = performance_counter_frequency.QuadPart;
        }
        return cache;
    }
};

} // namespace win32
