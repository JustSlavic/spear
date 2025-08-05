#ifndef _SPEAR_ENGINE_PRIMITIVE_MESHES_H
#define _SPEAR_ENGINE_PRIMITIVE_MESHES_H

#include <corelibs/base.h>
#include <corelibs/memory/allocator.h>
#include <engine/graphics/render.h>


cpu_mesh mesh_square_create(void);
cpu_mesh mesh_tetrahedron_create(void);
cpu_mesh mesh_cube_create(void);
cpu_mesh mesh_octahedron_create(void);
cpu_mesh mesh_icosahedron_create(void);
cpu_mesh mesh_sphere_create(int n, int m);
cpu_mesh mesh_ico_sphere_create(memory_allocator allocator, memory_allocator temp_allocator);


#endif // _SPEAR_ENGINE_PRIMITIVE_MESHES_H
