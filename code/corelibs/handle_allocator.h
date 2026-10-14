#ifndef SPEAR_CORELIBS_HANDLE_ALLOCATOR_H_
#define SPEAR_CORELIBS_HANDLE_ALLOCATOR_H_

#include "base.h"


typedef struct {
    uint32 count;
    uint32 capacity;

    uint32 mask;
    uint32 shift;

    uint32 *generation;
    uint32 *free_ring;
    uint64 head;
    uint64 tail;
} handle_allocator;


void handle_allocator_init(handle_allocator *ha, uint32 capacity, byte *memory, uint32 *size);
uint32 handle_allocator_allocate(handle_allocator *ha);
void handle_allocator_deallocate(handle_allocator *ha, uint32 handle);
bool handle_allocator_is_valid(handle_allocator *ha, uint32 handle);


#endif /* SPEAR_CORELIBS_HANDLE_ALLOCATOR_H_ */
