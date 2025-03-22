#ifndef PLATONIC_SOLIDS_HPP
#define PLATONIC_SOLIDS_HPP

#include <base.h>
#include "common_graphics.hpp"
#include <memory/allocator.hpp>


cpu_mesh make_platonic_tetrahedron();
cpu_mesh make_platonic_cube();
cpu_mesh make_platonic_octahedron();
cpu_mesh make_platonic_icosahedron();
cpu_mesh make_ico_sphere(memory_allocator t, memory_allocator a);

#endif
