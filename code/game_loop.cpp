#include "game_loop.hpp"

#include <memory/allocator.hpp>


void game_loop__initialize(game_loop_data *loop, platform::window *window)
{
    loop->memory = platform::allocate_pages((void *) TERABYTES(1), MEGABYTES(100));
    loop->allocator = memory_allocator::make_arena(loop->memory);
    loop->game_memory = loop->allocator.allocate_buffer(MEGABYTES(5));
    loop->temporary_allocator = loop->allocator.allocate_arena(MEGABYTES(10));

    loop->mesh_square = load_mesh(make_square());
    loop->mesh_square_uv = load_mesh(make_square_uv());
    loop->mesh_tetrahedron = load_mesh(make_platonic_tetrahedron());
    loop->mesh_cube = load_mesh(make_platonic_cube());
    loop->mesh_octahedron = load_mesh(make_platonic_octahedron());
    loop->mesh_icosahedron = load_mesh(make_platonic_icosahedron());
    loop->mesh_sphere = load_mesh(make_ico_sphere(loop->temporary_allocator, loop->allocator));

    loop->shader_color = compile_shaders(vs_single_color, fs_pass_color);
    loop->shader_ground = compile_shaders(vs_ground, fs_pass_color);
    loop->shader_framebuffer = compile_shaders(vs_framebuffer, fs_framebuffer);
    loop->shader_text = compile_shaders(vs_text, fs_text);
    loop->shader_phong = compile_shaders(vs_phong, fs_phong);
    loop->shader_sun = compile_shaders(vs_sun, fs_sun);

#if DEBUG
    loop->is_debug_graph_fps_active = false;
    loop->debug_graph_fps.memory = loop->allocator.allocate_buffer(sizeof(float32) * 512);
    loop->debug_graph_fps.graph = (float32 *) loop->debug_graph_fps.memory.data;
    loop->debug_graph_fps.index = 0;
    loop->debug_graph_fps.count = 512;
    loop->debug_graph_fps.max_value = 1000.f / 30.f;
#endif // DEBUG

    loop->ctx.temporary_allocator = loop->temporary_allocator;
    platform::get_monitor_resolution(window, &loop->screen_width, &loop->screen_height);

    loop->ctx.aspect_ratio = (float32) loop->screen_width / (float32) loop->screen_height;
    loop->ctx.near_clip_dist = 0.05f;
    loop->ctx.near_clip_width = 2 * loop->ctx.near_clip_dist * tanf(0.5f * to_radians(60));
    loop->ctx.near_clip_height = loop->ctx.near_clip_width / loop->ctx.aspect_ratio;
    loop->ctx.far_clip_dist = 10000.f;

    auto font_content = platform::load_file("font_14x26.png", &loop->allocator);
    auto font_bitmap = image::load_png(&loop->allocator, &loop->ctx.temporary_allocator, font_content);
    loop->font_texture = load_texture(font_bitmap);
    console::print("Font texture is id=%d\n", loop->font_texture.id);

    loop->frame_counter = 0;
}

void game_loop__update_viewport(game_loop_data *loop)
{
    if (loop->viewport_changed)
    {
        auto viewport = gfx::viewport::make(current_client_width, current_client_height, loop->ctx.aspect_ratio);
        glViewport(viewport.offset_x, viewport.offset_y, viewport.width, viewport.height);
        loop->viewport_changed = false;

        loop->ctx.viewport = viewport;

        loop->ctx.window_width = current_client_width;
        loop->ctx.window_height = current_client_height;
    }
}

void game_loop__iterate(game_loop_data *loop)
{
    loop->frame_counter += 1;
}
