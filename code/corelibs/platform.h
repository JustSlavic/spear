#ifndef _SPEAR_PLATFORM_H
#define _SPEAR_PLATFORM_H

#include <corelibs/base.h>


typedef struct dll dll;

void *platform_allocate_pages(void *base, usize size);
usize platform_get_file_size(char const *filename);
uint32 platform_read_file_into_memory(char const *filename, void *memory, usize size);
void platform_dll_open(dll *dll, char const *path);
void platform_dll_close(dll *dll);
bool32 platform_dll_is_valid(dll *dll);
void *platform_dll_get_function(dll *dll, char const *function_name);

timestamp_t platform_file_get_last_modified_time(char const *filename);

timepoint platform_clock_now(void);


#endif // _SPEAR_PLATFORM_H
