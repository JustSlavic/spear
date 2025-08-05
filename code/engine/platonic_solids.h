#ifndef _SPEAR_ENGINE_PLATONIC_SOLIDS_H
#define _SPEAR_ENGINE_PLATONIC_SOLIDS_H

#include <base/base.h>
#include <engine/render.h>


cpu_mesh make_platonic_tetrahedron();
cpu_mesh make_platonic_cube(float a = 0.5773502691896258f); // Default side a = 1 / sqrt(3), so vertices touch unit sphere
cpu_mesh make_platonic_octahedron();
cpu_mesh make_platonic_icosahedron();
cpu_mesh make_ico_sphere(memory_allocator temp_allocator, memory_allocator allocator);


#endif // _SPEAR_ENGINE_PLATONIC_SOLIDS_H
