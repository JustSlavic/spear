#ifndef PLATFORM_LINUX_HPP
#define PLATFORM_LINUX_HPP

#include <base.hpp>
#include <sys/mman.h>


namespace linux
{


INLINE memory_block allocate_memory(void *base_address, usize size)
{
    memory_block result;
    result.memory = mmap(base_address, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    result.size = size;
    ASSERT_MSG(result.memory != MAP_FAILED, "mmap failed");
    return result;
}

INLINE memory_block allocate_memory(usize size)
{
    memory_block result = allocate_memory(NULL, size);
    return result;
}

INLINE void free_memory(memory_block block)
{
    int ec = munmap(block.memory, block.size);
    ASSERT_MSG(ec == 0, "munmap failed");
}


} // namespace linux


#endif // PLATFORM_LINUX_HPP
