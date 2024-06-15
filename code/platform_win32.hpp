#ifndef PLATFORM_WIN32_HPP
#define PLATFORM_WIN32_HPP

#include <base.h>



namespace gl
{




}

namespace win32 {

// int32 width(RECT rectangle)
// {
//     int32 result = rectangle.right - rectangle.left;
//     return result;
// }

// int32 height(RECT rectangle)
// {
//     int32 result = rectangle.bottom - rectangle.top;
//     return result;
// }

// struct window
// {
//     HWND handle;
//     HDC device_context;

//     void get_mouse_pos(int32 *x, int32 *y)
//     {
//         POINT pos;
//         GetCursorPos(&pos);
//         ScreenToClient(handle, &pos);
//         *x = pos.x;
//         *y = pos.y;
//     }
// };

// void create_opengl_window(HINSTANCE instance, int32 width, int32 height, MainWindowCallbackType cb, window *w)
// {
//     gl::create_window(instance, width, height, cb, w);
// }

// uint32 get_program_path(HINSTANCE instance, char *buffer, uint32 buffer_size)
// {
//     uint32 result_size = GetModuleFileNameA(instance, buffer, buffer_size);
//     return result_size;
// }



// memory_buffer load_file(memory_allocator allocator, char const *filename)
// {
//     memory_buffer result = memory_buffer::make();

//     HANDLE FileHandle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//     if (FileHandle == INVALID_HANDLE_VALUE)
//     {
//         return result;
//     }

//     LARGE_INTEGER FileSize;
//     BOOL GetSizeResult = GetFileSizeEx(FileHandle, &FileSize);
//     if (GetSizeResult == FALSE)
//     {
//         return result;
//     }

//     memory_buffer Memory = allocator.allocate_buffer(FileSize.QuadPart);

//     DWORD BytesRead;
//     BOOL ReadFileResult = ReadFile(FileHandle, Memory.data, (DWORD) FileSize.QuadPart, &BytesRead, NULL);

//     if (ReadFileResult == FALSE)
//     {
//         allocator.deallocate(Memory);
//         return result;
//     }

//     result.data = Memory.data;
//     result.size = FileSize.QuadPart;
//     return result;
// }







// struct wall_clock
// {
//     [[nodiscard]]
//     static timepoint now()
//     {
//         LARGE_INTEGER PerformanceCounter;
//         QueryPerformanceCounter(&PerformanceCounter);
//         timepoint result = { (uint64) PerformanceCounter.QuadPart * 1000000 / frequency() };
//         return result;
//     }

//     [[nodiscard]]
//     static int64 frequency()
//     {
//         PERSIST int64 cache;
//         if (cache == 0)
//         {
//             LARGE_INTEGER performance_counter_frequency;
//             QueryPerformanceFrequency(&performance_counter_frequency);
//             cache = performance_counter_frequency.QuadPart;
//         }
//         return cache;
//     }
// };

} // namespace win32

#endif // PLATFORM_WIN32_HPP
