#include "handle_allocator.h"


void handle_allocator_init(handle_allocator *ha, uint32 capacity, byte *memory, uint32 *size)
{
    uint32 alive_array_size = capacity * sizeof(*ha->alive);
    uint32 generation_array_size = capacity * sizeof(*ha->generation);
    uint32 free_ring_array_size = capacity * sizeof(*ha->free_ring);
    uint32 required_memory_size = alive_array_size + generation_array_size + free_ring_array_size;

    if (memory == NULL && size != NULL)
    {
        *size = required_memory_size;
        return;
    }

    if (memory != NULL && size != NULL)
    {
        if (*size != required_memory_size)
            return;

        ha->count = 0;
        ha->capacity = capacity;

        ha->mask = uint32_next_power_of_two(capacity | 1) - 1;
        ha->shift = __builtin_popcount(ha->mask);

        ha->alive = (bool32 *) (memory);
        ha->generation = (uint32 *) (memory + alive_array_size);
        ha->free_ring = (uint32 *) (memory + alive_array_size + generation_array_size);
        ha->head = 0;
        ha->tail = 0;

        int i;
        for (i = 1; i < capacity; i++)
        {
            ha->free_ring[ha->head] = i;
            ha->head += 1;
        }
        return;
    }

    ASSERT_MSG(false, "Handle allocator was called with wrong combination of arguments.");
}

uint32 handle_allocator_allocate(handle_allocator *ha)
{
    uint32 handle = 0;
    if (ha->tail < ha->head)
    {
        uint32 index = ha->free_ring[ha->tail % ha->capacity];
        if (!ha->alive[index])
        {
            uint32 generation = ha->generation[index];
            ha->alive[index] = true;
            ha->tail += 1;
            ha->count += 1;
            handle = (index & ha->mask) | (generation << ha->shift);
        }
        else
        {
            printf("Handle allocator tries to allocate already alive handle.");
        }
    }
    else
    {
        printf("Handle allocator reached maximum number of alive handles, and could not allocate more!\n");
    }
    return handle;
}

void handle_allocator_deallocate(handle_allocator *ha, uint32 handle)
{
    if (handle)
    {
        uint32 index = handle & ha->mask;
        uint32 generation = handle >> ha->shift;

        if (ha->alive[index])
        {
            if (generation < ha->generation[index])
            {
                ASSERT_MSG(false, "Handle allocator was asked to deallocate stale handle (%u:%u)",
                    generation, index);
                return;
            }

            if (generation == ha->generation[index])
            {
                ha->alive[index] = false;
                ha->generation[index] += 1;
                ha->free_ring[ha->head % ha->capacity] = index;
                ha->head += 1;
                ha->count -= 1;
            }
            else
            {
                ASSERT_MSG(false, "Handle allocator could not deallocate because something wrong happened! (handle.generation=%u; allocator.generation=%u)",
                    generation, ha->generation[index]);
            }
        }
        else
        {
            printf("Handle allocator was asked to deallocate dead handle (%u:%u).\n", generation, index);
        }
    }
    else
    {
        ASSERT_MSG(false, "Handle allocator deallocates invalid handle.");
    }
}

bool handle_allocator_is_valid(handle_allocator *ha, uint32 handle)
{
    bool result = false;
    if (handle)
    {
        uint32 index = handle & ha->mask;
        uint32 generation = handle >> ha->shift;
        result = (generation == ha->generation[index]);
    }
    return result;
}
