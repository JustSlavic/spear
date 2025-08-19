#include "engine.h"
#include "static_shaders.c"
#include "primitive_meshes.h"

void spear_engine_init(engine *engine)
{
    engine->running = false;
    engine->viewport_changed = false;
    engine->current_client_width = 1200;
    engine->current_client_height = 800;

    usize allocator_size = MEGABYTES(1);
    usize temporary_size = MEGABYTES(1);
    usize game_memory_size = MEGABYTES(2);
    usize memory_size = allocator_size + temporary_size + game_memory_size;

    void *memory = platform_allocate_pages((void *) TERABYTES(1), memory_size);
    engine->memory.data = memory;
    engine->memory.size = memory_size;

    engine->allocator = memory_allocator_arena_create(memory, allocator_size);
    engine->temporary = memory_allocator_arena_create(memory + allocator_size, temporary_size);
    engine->game_memory.data = memory + allocator_size + temporary_size;
    engine->game_memory.size = game_memory_size;

    engine->game_dll = ALLOCATE(engine->allocator, dll);

    engine->fov = degrees_to_radians(60);
    printf("fov = %f\n", engine->fov);
    engine->aspect_ratio = 16.0f / 9.0f;
    engine->near_clip_distance = 0.05f;
    engine->near_clip_width = 2 * engine->near_clip_distance * tanf(0.5f * engine->fov);
    engine->near_clip_height = engine->near_clip_width / engine->aspect_ratio;
    engine->far_clip_distance = 10000.f;

    engine->game_context.near_clip_distance = engine->near_clip_distance;
    engine->game_context.near_clip_width = engine->near_clip_width;
    engine->game_context.near_clip_height = engine->near_clip_height;

    renderer_setup_projection(&engine->renderer,
        engine->fov,
        engine->aspect_ratio,
        engine->near_clip_distance,
        engine->far_clip_distance);
}

void spear_engine_create_meshes(engine *engine)
{
    engine->mesh_square = render_load_mesh_to_gpu(mesh_square_create());
    engine->mesh_tetrahedron = render_load_mesh_to_gpu(mesh_tetrahedron_create());
    engine->mesh_cube = render_load_mesh_to_gpu(mesh_cube_create());
    engine->mesh_octahedron = render_load_mesh_to_gpu(mesh_octahedron_create());
    engine->mesh_icosahedron = render_load_mesh_to_gpu(mesh_icosahedron_create());
    engine->mesh_sphere = render_load_mesh_to_gpu(mesh_sphere_create(10, 10));
    engine->mesh_ico_sphere = render_load_mesh_to_gpu(mesh_ico_sphere_create(engine->allocator, engine->temporary));
    engine->mesh_ui_frame = render_load_mesh_to_gpu(mesh_ui_frame_create());
}

void spear_engine_compile_shaders(engine *engine)
{
    engine->shader_single_color = render_compile_shaders(vs_single_color, fs_pass_color);
    engine->shader_ground = render_compile_shaders(vs_ground, fs_pass_color);
    engine->shader_framebuffer = render_compile_shaders(vs_framebuffer, fs_framebuffer);
    engine->shader_text = render_compile_shaders(vs_text, fs_text);
    engine->shader_phong = render_compile_shaders(vs_phong, fs_phong);
    engine->shader_sun = render_compile_shaders(vs_sun, fs_sun);
    engine->shader_ui_frame = render_compile_shaders(vs_frame, fs_pass_color);
}

void spear_engine_load_game_dll(engine *engine)
{
#if DLL_BUILD
    timestamp_t dll_last_modified_time = platform_file_get_last_modified_time("spear_game.so");
    if (engine->game_dll_timestamp < dll_last_modified_time)
    {
        if (platform_dll_is_valid(engine->game_dll))
        {
            platform_dll_close(engine->game_dll);
        }
        platform_dll_open(engine->game_dll, "spear_game.so");
        engine->initialize_memory = platform_dll_get_function(engine->game_dll, "initialize_memory");
        engine->update_and_render = platform_dll_get_function(engine->game_dll, "update_and_render");
        if (engine->initialize_memory && engine->update_and_render)
        {
            engine->game_dll_timestamp = dll_last_modified_time;
            printf("Successfully loaded game dll\n");
        }
        else
        {
            printf("Could not find function in dll\n");
        }
    }
#else
    #error "Not supported"
#endif
}

void spear_engine_input_reset_transitions(engine *engine)
{
    input_button_reset_transitions(engine->input.keyboard_and_mouse.buttons, Button_Count);
    engine->input.keyboard_and_mouse.mouse_scroll = 0;
}

void spear_engine_input_mouse_pos_set(engine *engine, int mouse_x, int mouse_y)
{
    engine->input.keyboard_and_mouse.window_mouse.x = mouse_x;
    engine->input.keyboard_and_mouse.window_mouse.y = mouse_y;
    engine->input.keyboard_and_mouse.viewport_mouse.x = mouse_x - engine->viewport.offset_x;
    engine->input.keyboard_and_mouse.viewport_mouse.y = mouse_y - engine->viewport.offset_y;
}

void spear_engine_update_viewport(engine *engine, int width, int height)
{
    if (engine->viewport_changed)
    {
        viewport viewport = render_viewport_create(width, height, engine->aspect_ratio);
        glViewport(viewport.offset_x, viewport.offset_y, viewport.width, viewport.height);

        engine->viewport = viewport;
        engine->viewport_changed = false;
        engine->current_client_width = width;
        engine->current_client_height = height;

        engine->game_context.viewport_offset_x = viewport.offset_x;
        engine->game_context.viewport_offset_y = viewport.offset_y;
        engine->game_context.viewport_width = viewport.width;
        engine->game_context.viewport_height = viewport.height;

        engine->renderer.proj_matrix_ui =
            m4f_mul(
                m4f_translate(-1, 1, 0),
                m4f_scale(2.f / viewport.width, -2.f/viewport.height, 1));
    }
}

void spear_engine_game_init(engine *engine)
{
    if (engine->initialize_memory)
    {
        engine->initialize_memory(&engine->game_context, engine->game_memory);
    }
    else
    {
        printf("Could not run 'initialize_memory' function.\n");
    }
}

void spear_engine_game_update(engine *engine)
{
    if (engine->update_and_render)
    {
        engine->update_and_render(&engine->game_context, engine->game_memory, &engine->input);
    }
    else
    {
        printf("Could not run 'update_and_render' function.\n");
    }

    int engine_command_index;
    for (engine_command_index = 0;
         engine_command_index < engine->game_context.engine_commands_count;
         engine_command_index++)
    {
        engine_command cmd = engine->game_context.engine_commands[engine_command_index];
        switch (cmd.tag)
        {
            case EngineCommand_Exit:
                engine->running = false;
            break;

            case EngineCommand_Invalid:
            ASSERT_MSG(false, "Engine: EngineCommand_Invalid (= 0) in the command queue.\n");
            break;
        }
    }
    engine->game_context.engine_commands_count = 0;
}

static void spear_engine_draw_mesh_internal(engine *engine, render_command cmd)
{
    matrix4 model =
        m4f_mul(
            m4f_translate(cmd.mesh_position.x, cmd.mesh_position.y, cmd.mesh_position.z),
            m4f_scale(cmd.mesh_scale.x, cmd.mesh_scale.y, cmd.mesh_scale.z)
        );
    gpu_mesh m = cmd.mesh_tag == RenderCommand_DrawMesh_Square ? engine->mesh_square :
                 cmd.mesh_tag == RenderCommand_DrawMesh_Cube ? engine->mesh_cube :
                 engine->mesh_cube;
    gpu_shader s = cmd.mesh_shader_tag == RenderCommand_DrawShader_SingleColor ? engine->shader_single_color :
                   cmd.mesh_shader_tag == RenderCommand_DrawShader_Ground ? engine->shader_ground :
                   engine->shader_single_color;

    renderer_draw_mesh(&engine->renderer, model, m, s, cmd.mesh_color);
}

static void spear_engine_draw_ui(engine *engine, render_command cmd)
{
    if (cmd.mesh_tag == RenderCommand_DrawMesh_UiFrame)
    {
        matrix4 model = m4f_mul(
            tm_to_m4f(cmd.ui_tm),
            m4f_mul(
                m4f_translate(cmd.ui_offset.x, cmd.ui_offset.y, 0),
                m4f_scale(0.5f * cmd.ui_width, 0.5f * cmd.ui_height, 1)));
        renderer_draw_ui_frame(&engine->renderer,
            model,
            engine->mesh_ui_frame,
            engine->shader_ui_frame,
            cmd.ui_color,
            cmd.ui_frame_width,
            cmd.ui_frame_width);
    }
    else
    {
        matrix4 model = m4f_mul(
            tm_to_m4f(cmd.ui_tm),
            m4f_scale(0.5f * cmd.ui_width, 0.5f * cmd.ui_height, 1));
        renderer_draw_mesh_ui(&engine->renderer,
            model,
            engine->mesh_square,
            engine->shader_single_color,
            cmd.ui_color);
    }

}

void spear_engine_game_render(engine *engine)
{
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int render_command_index;
    for (render_command_index = 0;
        render_command_index < engine->game_context.render_commands_count;
        render_command_index++)
    {
        render_command cmd = engine->game_context.render_commands[render_command_index];
        switch (cmd.tag)
        {
            case RenderCommand_SetupCamera:
            {
                renderer_setup_camera(&engine->renderer,
                    cmd.camera_position, cmd.camera_forward, cmd.camera_up);
                glDisable(GL_DEPTH_TEST);
                renderer_draw_mesh_ui(&engine->renderer,
                    m4f_scale(10000.f, 10000.f, 1.f),
                    engine->mesh_square,
                    engine->shader_single_color,
                    v4f(0.1f, 0.1f, 0.1f, 1.f));
                glEnable(GL_DEPTH_TEST);
            }
            break;

            case RenderCommand_DrawMesh:
            {
                spear_engine_draw_mesh_internal(engine, cmd);
            }
            break;

            case RenderCommand_DrawUi:
            {
                spear_engine_draw_ui(engine, cmd);
            }
            break;

            case RenderCommand_Invalid:
                ASSERT_MSG(0, "Engine: RenderCommand_Invalid (= 0) in the command queue.\n");
            break;
        }
    }
    engine->game_context.render_commands_count = 0;
}
