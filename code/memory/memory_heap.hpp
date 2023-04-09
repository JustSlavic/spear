#ifndef MEMORY_MEMORY_HEAP_HPP
#define MEMORY_MEMORY_HEAP_HPP

#include <base.hpp>
#include "allocator.hpp"


namespace memory {


struct memory_heap
{
    #define HEAP_HEADER_OFFSET_MASK   (((usize) -1) >> 1)
    #define HEAP_HEADER_OCCUPIED_MASK (((usize)  1) << (8*sizeof(usize) - 1))

    using header = usize;
    using footer = usize;
    using guard  = usize;

    static usize get_offset(usize *data)
    {
        usize result = (*data) & HEAP_HEADER_OFFSET_MASK;
        return result;
    }

    static void set_offset(usize *data, usize chunk_size)
    {
        (*data) = ((*data) & HEAP_HEADER_OCCUPIED_MASK) | (chunk_size & HEAP_HEADER_OFFSET_MASK);
    }

    static bool32 is_occupied(usize *data)
    {
        bool result = (*data) & HEAP_HEADER_OCCUPIED_MASK;
        return result;
    }

    static void set_occupied(usize *data, bool occupied)
    {
        (*data) = ((*data) & HEAP_HEADER_OFFSET_MASK) | (((usize) occupied) << (8*sizeof(usize) - 1));
    }

    #undef HEAP_HEADER_OFFSET_MASK
    #undef HEAP_HEADER_OCCUPIED_MASK

    ALLOCATOR_BASE;
    header *first_header;
    footer *last_footer;

    guard *first_guard;
    guard *last_guard;
};


void initialize_memory_heap(allocator *a, void *memory, usize size)
{
    auto *heap = (memory_heap *) a;

    heap->type = allocator::heap;
    heap->memory = memory;
    heap->size = size;
    heap->used = 0;

    ASSERT(size > (2*sizeof(memory_heap::guard) + sizeof(memory_heap::header) + sizeof(memory_heap::footer)));

    usize first_guard_padding = get_padding(memory, alignof(memory_heap::guard));
    heap->first_guard = (memory_heap::guard *) ((byte *) memory + first_guard_padding);

    byte *last_guard_address = ((byte *) memory + size - sizeof(memory_heap::guard));
    usize last_guard_padding = get_padding(last_guard_address, alignof(memory_heap::guard));
    last_guard_address -= (sizeof(memory_heap::guard) - last_guard_padding);
    heap->last_guard = (memory_heap::guard *) last_guard_address;

    heap->used += sizeof(memory_heap::guard) * 2 + last_guard_padding;

    *heap->first_guard = 0;
    *heap->last_guard = 0;

    uintptr_t first_header_address = (uintptr_t) ((byte *) heap->first_guard + sizeof(memory_heap::guard));
    usize offset = (uintptr_t) last_guard_address - first_header_address;

    heap->first_header = (memory_heap::header *) first_header_address;
    memory_heap::set_occupied(heap->first_header, false);
    memory_heap::set_offset(heap->first_header, offset);

    heap->last_footer = (memory_heap::footer *) ((byte *) last_guard_address - sizeof(memory_heap::footer));
    memory_heap::set_occupied(heap->last_footer, false);
    memory_heap::set_offset(heap->last_footer, offset);

    heap->used += sizeof(memory_heap::header) + sizeof(memory_heap::footer);
}

void initialize_memory_heap(allocator *a, memory_block block)
{
    initialize_memory_heap(a, block.memory, block.size);
}

void heap_reset(allocator *a)
{
    initialize_memory_heap(a, a->memory, a->size);
}

void *heap_allocate(allocator *a, usize requested_size, usize alignment)
{
    auto *heap = (memory_heap *) a;
    void *result = NULL;

    // The minimal allocation size is 8 bytes (have to be for alignment purposes)
    if (requested_size < sizeof(usize))
        requested_size = sizeof(usize);

    auto *header = heap->first_header;
    loop
    {
        if (header == heap->last_guard) break;

        bool32 is_occupied = memory_heap::is_occupied(header);
        usize offset = memory_heap::get_offset(header);
        usize chunk_size = offset - 2*sizeof(memory_heap::guard);

        // [header][memory][footer][guard or offset]
        // | <--    offset    ---> |
        //         |      | <- chunk_size

        if (!is_occupied && (chunk_size >= requested_size))
        {
            if (chunk_size >= (requested_size + 4*sizeof(memory_heap::guard)))
            {
                // [header].................................................................[footer]
                //         [allocated memory][new footer][new header]...the rest of chunk...
                //
                // The rest of chunk is at minimum sizeof(memory_heap::guard);
                // + also one additional 'sizeof' for padding, so minimal allocation size is 8 bytes,

                auto *new_footer = (memory_heap::footer *) ((byte *) header + sizeof(memory_heap::header) + requested_size);
                usize new_footer_padding = get_padding(new_footer, alignof(memory_heap::footer));
                new_footer = (memory_heap::footer *) ((byte *) new_footer + new_footer_padding);
                auto *new_header = (memory_heap::header *) ((byte *) new_footer + sizeof(memory_heap::footer));

                usize offset_1 = requested_size + new_footer_padding + sizeof(memory_heap::footer) + sizeof(memory_heap::header);
                usize offset_2 = offset - offset_1;

                memory_heap::set_offset(header, offset_1);
                memory_heap::set_occupied(header, true);
                *new_footer = *header;

                auto *old_footer = (memory_heap::footer *) ((byte *) header + offset - sizeof(memory_heap::footer));

                memory_heap::set_offset(new_header, offset_2);
                memory_heap::set_occupied(new_header, false);
                *old_footer = *new_header;

                result = (byte *) header + sizeof(memory_heap::header);

                heap->used += requested_size + new_footer_padding + sizeof(memory_heap::footer) + sizeof(memory_heap::header);
            }
            else
            {
                // Allocate in this chunk without dividing it more
                auto *footer = (memory_heap::footer *) (header + offset - sizeof(memory_heap::footer));

                memory_heap::set_occupied(header, true);
                memory_heap::set_occupied(footer, true);

                result = (byte *) header + sizeof(memory_heap::header);

                heap->used += requested_size;
            }

            break;
        }

        header = (memory_heap::header *) ((byte *) header + offset);
    }


    return result;
}

void heap_deallocate(allocator *a, void *memory)
{
    auto *heap = (memory_heap *) a;
    auto *header = (memory_heap::header *) ((byte *) memory - sizeof(memory_heap::header));
    usize offset = memory_heap::get_offset(header);
    auto *footer = (memory_heap::footer *) ((byte *) header + offset - sizeof(memory_heap::footer));
    heap->used -= (offset - sizeof(memory_heap::header) - sizeof(memory_heap::footer));

    auto *left_footer = (memory_heap::footer *) ((byte *) header - sizeof(memory_heap::footer));
    if (*left_footer != 0)
    {
        if (!memory_heap::is_occupied(left_footer))
        {
            usize left_offset = memory_heap::get_offset(left_footer);
            auto *left_header = (memory_heap::header *) ((byte *) left_footer - left_offset + sizeof(memory_heap::header));
            header = left_header;

            offset = left_offset + offset;
            memory_heap::set_offset(header, offset);
            memory_heap::set_offset(footer, offset);

            heap->used -= (sizeof(memory_heap::footer) + sizeof(memory_heap::header));
        }
    }

    auto *right_header = (memory_heap::header *) ((byte *) footer + sizeof(memory_heap::header));
    if (*right_header != 0)
    {
        if (!memory_heap::is_occupied(right_header))
        {
            usize right_offset = memory_heap::get_offset(right_header);
            auto *right_footer = (memory_heap::footer *) ((byte *) right_header + right_offset - sizeof(memory_heap::footer));
            footer = right_footer;

            offset = offset + right_offset;
            memory_heap::set_offset(header, offset);
            memory_heap::set_offset(footer, offset);

            heap->used -= (sizeof(memory_heap::footer) + sizeof(memory_heap::header));
        }
    }

    memory_heap::set_occupied(header, false);
    memory_heap::set_occupied(footer, false);
}

// @todo
// void *reallocate(allocator *a, void *memory, usize size);


} // namespace memory

#endif // MEMORY_MEMORY_HEAP_HPP
