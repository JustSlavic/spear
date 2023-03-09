#ifndef RESOURCE_SYSTEM_HPP
#define RESOURCE_SYSTEM_HPP

#include <base.hpp>
#include <memory/allocator.hpp>


namespace rs
{


struct resource_token
{
    uint32 id;
};


struct resource
{
    void *data;
};


struct mesh_resource
{

};


struct texture_resource
{

};


struct shader_resource
{

};


struct resource_storage
{
    memory::allocator heap;
};


INLINE resource_token create_mesh_resource(resource_storage *storage, float32 *vbo)
{
    resource_token result = {};
    return result;
}


} // namespace rs


#endif // RESOURCE_SYSTEM_HPP
