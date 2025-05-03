#include "game_loop.hpp"

#include <memory/allocator.hpp>


void init_loop(game_loop_data *data)
{
    data->memory = platform::allocate_pages((void *) TERABYTES(1), MEGABYTES(100));
    data->allocator = memory_allocator::make_arena(data->memory);
    data->game_memory = data->allocator.allocate_buffer(MEGABYTES(5));
    data->temporary_allocator = data->allocator.allocate_arena(MEGABYTES(10));

    data->mesh_square = load_mesh(make_square());
    data->mesh_square_uv = load_mesh(make_square_uv());
    data->mesh_tetrahedron = load_mesh(make_platonic_tetrahedron());
    data->mesh_cube = load_mesh(make_platonic_cube());
    data->mesh_octahedron = load_mesh(make_platonic_octahedron());
    data->mesh_icosahedron = load_mesh(make_platonic_icosahedron());
    data->mesh_sphere = load_mesh(make_ico_sphere(data->temporary_allocator, data->allocator));

    data->shader_color = compile_shaders(vs_single_color, fs_pass_color);
    data->shader_ground = compile_shaders(vs_ground, fs_pass_color);
    data->shader_framebuffer = compile_shaders(vs_framebuffer, fs_framebuffer);
    data->shader_text = compile_shaders(vs_text, fs_text);
    data->shader_phong = compile_shaders(vs_phong, fs_phong);
    data->shader_sun = compile_shaders(vs_sun, fs_sun);

#if DEBUG
    data->is_debug_graph_fps_active = false;
    data->debug_graph_fps.memory = data->allocator.allocate_buffer(sizeof(float32) * 512);
    data->debug_graph_fps.graph = (float32 *) data->debug_graph_fps.memory.data;
    data->debug_graph_fps.index = 0;
    data->debug_graph_fps.count = 512;
    data->debug_graph_fps.max_value = 1000.f / 30.f;
#endif // DEBUG

    data->frame_counter = 0;
}


void iter_loop(game_loop_data *data)
{
    data->frame_counter += 1;
}
