#include <base.h>
#include <time.hpp>
#include <input.hpp>
#include <game_interface.hpp>
#include <platform.hpp>
#include <platform_sdl.hpp>
#include <hash.hpp>
#include <image/png.hpp>
#include <math.hpp>

#include <gfx/viewport.hpp>
#include <gfx/static_shaders.cpp>

#include <SDL2/SDL.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <common_graphics.hpp>
#include <gen/font_14x26.hpp>
#include <memory/serializer.hpp>
#include <string_view.hpp>

#include <platonic_solids.hpp>
#include <debug_graph.hpp>

GLOBAL bool32 running;
GLOBAL int32 current_client_width;
GLOBAL int32 current_client_height;
GLOBAL bool32 viewport_changed;


uint32 map_button_from_scancode_2(uint32 sk)
{
    switch (sk)
    {
    case SDL_SCANCODE_ESCAPE: return Keyboard_Esc;
    case SDL_SCANCODE_F1: return Keyboard_F1;
    case SDL_SCANCODE_F2: return Keyboard_F2;
    case SDL_SCANCODE_F3: return Keyboard_F3;
    case SDL_SCANCODE_F4: return Keyboard_F4;
    case SDL_SCANCODE_F5: return Keyboard_F5;
    case SDL_SCANCODE_F6: return Keyboard_F6;
    case SDL_SCANCODE_F7: return Keyboard_F7;
    case SDL_SCANCODE_F8: return Keyboard_F8;
    case SDL_SCANCODE_F9: return Keyboard_F9;
    case SDL_SCANCODE_F10: return Keyboard_F10;
    case SDL_SCANCODE_F11: return Keyboard_F11;
    case SDL_SCANCODE_F12: return Keyboard_F12;
    case SDL_SCANCODE_A: return Keyboard_A;
    case SDL_SCANCODE_B: return Keyboard_B;
    case SDL_SCANCODE_C: return Keyboard_C;
    case SDL_SCANCODE_D: return Keyboard_D;
    case SDL_SCANCODE_E: return Keyboard_E;
    case SDL_SCANCODE_F: return Keyboard_F;
    case SDL_SCANCODE_G: return Keyboard_G;
    case SDL_SCANCODE_H: return Keyboard_H;
    case SDL_SCANCODE_I: return Keyboard_I;
    case SDL_SCANCODE_J: return Keyboard_J;
    case SDL_SCANCODE_K: return Keyboard_K;
    case SDL_SCANCODE_L: return Keyboard_L;
    case SDL_SCANCODE_M: return Keyboard_M;
    case SDL_SCANCODE_N: return Keyboard_N;
    case SDL_SCANCODE_O: return Keyboard_O;
    case SDL_SCANCODE_P: return Keyboard_P;
    case SDL_SCANCODE_Q: return Keyboard_Q;
    case SDL_SCANCODE_R: return Keyboard_R;
    case SDL_SCANCODE_S: return Keyboard_S;
    case SDL_SCANCODE_T: return Keyboard_T;
    case SDL_SCANCODE_U: return Keyboard_U;
    case SDL_SCANCODE_V: return Keyboard_V;
    case SDL_SCANCODE_W: return Keyboard_W;
    case SDL_SCANCODE_X: return Keyboard_X;
    case SDL_SCANCODE_Y: return Keyboard_Y;
    case SDL_SCANCODE_Z: return Keyboard_Z;
    case SDL_SCANCODE_0: return Keyboard_0;
    case SDL_SCANCODE_1: return Keyboard_1;
    case SDL_SCANCODE_2: return Keyboard_2;
    case SDL_SCANCODE_3: return Keyboard_3;
    case SDL_SCANCODE_4: return Keyboard_4;
    case SDL_SCANCODE_5: return Keyboard_5;
    case SDL_SCANCODE_6: return Keyboard_6;
    case SDL_SCANCODE_7: return Keyboard_7;
    case SDL_SCANCODE_8: return Keyboard_8;
    case SDL_SCANCODE_9: return Keyboard_9;
    case SDL_SCANCODE_UP: return Keyboard_Up;
    case SDL_SCANCODE_DOWN: return Keyboard_Down;
    case SDL_SCANCODE_LEFT: return Keyboard_Left;
    case SDL_SCANCODE_RIGHT: return Keyboard_Right;
    }
    return 0;
}


void process_pending_messages(input_state *input)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type)
        {
            case SDL_QUIT:
                running = false;
            break;

            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED ||
                    e.window.event == SDL_WINDOWEVENT_SHOWN ||
                    e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    viewport_changed = true;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                auto button = (e.button.button == 1) ? MOUSE_LEFT :
                              (e.button.button == 2) ? MOUSE_MIDDLE :
                              (e.button.button == 3) ? MOUSE_RIGHT : MOUSE_NONE;
                process_button_state(&input->mouse[button], e.button.state == SDL_PRESSED);
            }
            break;

            case SDL_KEYUP:
            case SDL_KEYDOWN:
            {
                uint32 k = map_button_from_scancode_2(e.key.keysym.scancode);
                if (k != Button_None)
                {
                    process_button_state(&input->keyboard_and_mouse.buttons[k], e.type == SDL_KEYDOWN);
                }
            }
            break;
        }
    }
}

#define ECS_HASH hash_djb2

timepoint now()
{
    timepoint result = { SDL_GetTicks64() * 1000 };
    return result;
}

void draw_platonic_solid(gpu_mesh mesh, shader s, vector4 c, matrix4 m, matrix4 v, matrix4 p)
{
    glUseProgram(s.id);

    s.uniform("u_model", m);
    s.uniform("u_view", v);
    s.uniform("u_projection", p);
    s.uniform("u_color", c);

    glBindVertexArray(mesh.vao);
    if (mesh.ibo)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
        glDrawElements(GL_TRIANGLES, mesh.count, GL_UNSIGNED_INT, NULL);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, mesh.count);
    }
}

int main()
{
    current_client_width = 1200;
    current_client_height = 675;

    auto global_memory = sdl::allocate_memory((void *) TERABYTES(1), MEGABYTES(50));
    auto global_arena  = memory_allocator::make_arena(global_memory);

    auto game_memory = ALLOCATE_BUFFER(global_arena, MEGABYTES(5));
    auto temporary_allocator = global_arena.allocate_arena(MEGABYTES(5));

    // ======================================================================

    int err = SDL_Init(SDL_INIT_VIDEO);
    if (err < 0)
    {
        printf("Could not initialize SDL2: \"%s\"\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    auto window = SDL_CreateWindow(
        "window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        current_client_width, current_client_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (window == NULL)
    {
        printf("Could not create SDL Window: \"%s\"\n", SDL_GetError());
        exit(2);
    }

    auto render_context = SDL_GL_CreateContext(window);
    if (render_context == NULL)
    {
        printf("Could not create OpenGL context: \"%s\"\n", SDL_GetError());
        exit(4);
    }

    SDL_GL_MakeCurrent(window, render_context);

    SDL_version compiled;
    SDL_VERSION(&compiled);
    printf("Compiled against SDL2 v.%d.%d.%d\n", (int)compiled.major, (int)compiled.minor, (int)compiled.patch);

    SDL_version linked;
    SDL_GetVersion(&linked);
    printf("Linked against SDL2 v.%d.%d.%d\n", (int)linked.major, (int)linked.minor, (int)linked.patch);

    printf("glGetString(GL_SHADING_LANGUAGE_VERSION) -> %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    GLubyte const *gl_version = glGetString(GL_VERSION);
    printf("glGetString(GL_VERSION) -> %s\n", gl_version);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    SDL_GL_SetSwapInterval(1);

    // ======================================================================

    context ctx;
    memset(&ctx, 0, sizeof(ctx));

    ctx.aspect_ratio = 16.0f / 9.0f;
    ctx.near_clip_dist = 0.05f;
    ctx.near_clip_width = 2 * ctx.near_clip_dist * tanf(0.5f * to_radians(60));
    ctx.near_clip_height = ctx.near_clip_width / ctx.aspect_ratio;
    ctx.far_clip_dist = 10000.f;
    ctx.debug_load_file = NULL;
    ctx.temporary_allocator = temporary_allocator;

    // ======================================================================

    auto view_matrix = matrix4::identity();
    auto proj_matrix = make_projection_matrix_fov(to_radians(60), ctx.aspect_ratio, ctx.near_clip_dist, ctx.far_clip_dist);
    auto proj_matrix_ui = matrix4::identity();

    // ======================================================================

    auto cpu_square = make_square();
    auto gpu_square = load_mesh(cpu_square);

    auto cpu_cube = make_cube();
    auto gpu_cube = load_mesh(cpu_cube);

    auto cpu_sphere = make_sphere();
    auto gpu_sphere = load_mesh(cpu_sphere);

    auto cpu_tetrahedron = make_platonic_tetrahedron();
    auto gpu_tetrahedron = load_mesh(cpu_tetrahedron);

    auto cpu_platonic_cube = make_platonic_cube();
    auto gpu_platonic_cube = load_mesh(cpu_platonic_cube);

    auto cpu_octahedron = make_platonic_octahedron();
    auto gpu_octahedron = load_mesh(cpu_octahedron);

    auto cpu_icosahedron = make_platonic_icosahedron();
    auto gpu_icosahedron = load_mesh(cpu_icosahedron);

    auto cpu_ico_sphere = make_ico_sphere(temporary_allocator, global_arena);
    auto gpu_ico_sphere = load_mesh(cpu_ico_sphere);

    auto cpu_square_uv = make_square_uv();
    auto gpu_square_uv = load_mesh(cpu_square_uv);

    auto shader_color = compile_shaders(vs_single_color, fs_pass_color);
    auto shader_ground = compile_shaders(vs_ground, fs_pass_color);
    auto shader_framebuffer = compile_shaders(vs_framebuffer, fs_framebuffer);
    auto shader_text = compile_shaders(vs_text, fs_text);
    auto shader_phong = compile_shaders(vs_phong, fs_phong);
    auto shader_sun = compile_shaders(vs_sun, fs_sun);

    auto font_content = platform::load_file("font_14x26.png", &global_arena);
    auto font_bitmap = image::load_png(&global_arena, &temporary_allocator, font_content);
    auto font_texture = load_texture(font_bitmap);
    console::print("Font texture is id=%d\n", font_texture.id);

    // ======================================================================

    auto ui_framebuffer = create_framebuffer(current_client_width, current_client_height);

    // ======================================================================
#if DEBUG
    bool32 debug_draw_fps_graph_active = false;
    debug_graph debug_fps_graph = {};
    debug_fps_graph.memory = ALLOCATE_BUFFER(global_arena, sizeof(float32) * 512);
    debug_fps_graph.graph = (float32 *) debug_fps_graph.memory.data;
    debug_fps_graph.index = 0;
    debug_fps_graph.count = 512;
    debug_fps_graph.max_value = 1000.f / 30.f;
#endif // DEBUG
    // ======================================================================

#define FLUID_N 50
#define FLUID_SIZE ((FLUID_N)+2)*((FLUID_N)+2)
#define Ix(x, y) ((x)*((FLUID_N)+2) + (y))
    float32 s[FLUID_SIZE] = {};
    float32 u_data[FLUID_SIZE] = {};
    float32 u_data_[FLUID_SIZE] = {};
    float32 v_data[FLUID_SIZE] = {};
    float32 v_data_[FLUID_SIZE] = {};
    float32 rho_data[FLUID_SIZE] = {};
    float32 rho_data_[FLUID_SIZE] = {};

    float32 *u    = u_data;
    float32 *u0   = u_data_;
    float32 *v    = v_data;
    float32 *v0   = v_data_;
    float32 *rho  = rho_data;
    float32 *rho0 = rho_data_;

    s[Ix(FLUID_N/2, FLUID_N/2)] = 100.0;

    // ======================================================================

    initialize_memory(&ctx, game_memory);

    input_state input = {};

    duration last_frame_dt = duration::microseconds(16);
    timepoint last_timepoint = now();

    running = true;
    while (running)
    {
        reset_transitions(input.keyboard_and_mouse.buttons, Button_Count);
        process_pending_messages(&input);
        input.keyboard_and_mouse.scroll = 0;
        sdl::get_mouse_pos(&input.keyboard_and_mouse.x, &input.keyboard_and_mouse.y);
        input.dt = get_seconds(last_frame_dt);
        input.time = get_seconds(last_timepoint);

        if (viewport_changed)
        {
            SDL_GetWindowSize(window, &current_client_width, &current_client_height);
            auto viewport = gfx::viewport::make(current_client_width, current_client_height, ctx.aspect_ratio);
            glViewport(viewport.offset_x, viewport.offset_y, viewport.width, viewport.height);
            viewport_changed = false;

            ctx.viewport = viewport;

            printf("Viewport size = %d x %d\n", viewport.width, viewport.height);

            ctx.window_width = current_client_width;
            ctx.window_height = current_client_height;

            proj_matrix_ui = matrix4::translate(-1, 1, 0)
                           * matrix4::scale(2.0f/viewport.width, -2.0f/viewport.height, 1);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, ui_framebuffer.framebuffer_id);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        update_and_render(&ctx, game_memory, &input);

        for (auto cmd : ctx.exec_commands)
        {
            switch (cmd.tag)
            {
                case ExecutionCommand_ExitGame:
                {
                    running = false;
                }
                break;

                case ExecutionCommand_DebugDraw_Off:
                    debug_draw_fps_graph_active = false;
                break;

                case ExecutionCommand_DebugDraw_Fps:
                    TOGGLE(debug_draw_fps_graph_active);
                break;

                default:
                    ASSERT_FAIL("Unsupported");
            }
        }
        ctx.exec_commands.clear();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        for (auto cmd : ctx.rend_commands)
        {
            if (cmd.tag == RenderCommand_SetupCamera)
            {
                view_matrix = make_lookat_matrix(cmd.position, cmd.position + cmd.forward, cmd.up);
            }
            else if (cmd.tag == RenderCommand_RenderMesh)
            {
                gpu_mesh *mesh = NULL;
                if (cmd.mesh_tag == RenderMesh_Square) mesh = &gpu_square;
                if (cmd.mesh_tag == RenderMesh_Cube) mesh = &gpu_cube;
                if (cmd.mesh_tag == RenderMesh_Sphere) mesh = &gpu_ico_sphere;

                shader *shader = NULL;
                if (cmd.shader_tag == RenderShader_SingleColor) shader = &shader_color;
                if (cmd.shader_tag == RenderShader_Ground) shader = &shader_ground;
                if (cmd.shader_tag == RenderShader_Phong) shader = &shader_phong;
                if (cmd.shader_tag == RenderShader_Sun) shader = &shader_sun;

                if (mesh && shader)
                {
                    auto tm = transform::translate(cmd.position) *
                              transform::scale(cmd.scale);

                    tm.sx = apply_unit_quaternion(cmd.orientation, tm.sx);
                    tm.sy = apply_unit_quaternion(cmd.orientation, tm.sy);
                    tm.sz = apply_unit_quaternion(cmd.orientation, tm.sz);

                    glUseProgram(shader->id);

                    shader->uniform("u_model", to_matrix4(tm));
                    shader->uniform("u_view", view_matrix);
                    shader->uniform("u_projection", proj_matrix);
                    shader->uniform("u_color", cmd.color);

                    glBindVertexArray(mesh->vao);
                    if (mesh->ibo)
                    {
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
                        glDrawElements(GL_TRIANGLES, mesh->count, GL_UNSIGNED_INT, NULL);
                    }
                    else
                    {
                        glDrawArrays(GL_TRIANGLES, 0, mesh->count);
                    }
                }
            }
            else if (cmd.tag == RenderCommand_RenderSquare)
            {
                glUseProgram(shader_color.id);

                shader_color.uniform("u_model", cmd.model);
                shader_color.uniform("u_view", view_matrix);
                shader_color.uniform("u_projection", proj_matrix);
                shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(gpu_square.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_square.ibo);
                glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.tag == RenderCommand_RenderCube)
            {
                shader *s = NULL;
                if (cmd.shader_tag == RenderShader_SingleColor) s = &shader_color;
                if (cmd.shader_tag == RenderShader_Ground) s = &shader_ground;
                if (cmd.shader_tag == RenderShader_Phong) s = &shader_phong;
                if (cmd.shader_tag == RenderShader_Sun) s = &shader_sun;

                glUseProgram(s->id);

                s->uniform("u_model", cmd.model);
                s->uniform("u_view", view_matrix);
                s->uniform("u_projection", proj_matrix);
                s->uniform("u_color", cmd.color);

                glBindVertexArray(gpu_cube.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_cube.ibo);
                glDrawElements(GL_TRIANGLES, gpu_cube.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.tag == RenderCommand_RenderSphere)
            {
                auto tm = transform::translate(cmd.position) *
                          transform::scale(cmd.scale);

                tm.sx = apply_unit_quaternion(cmd.orientation, tm.sx);
                tm.sy = apply_unit_quaternion(cmd.orientation, tm.sy);
                tm.sz = apply_unit_quaternion(cmd.orientation, tm.sz);

                // draw_platonic_solid(
                //     gpu_platonic_cube,
                //     shader_phong,
                //     cmd.color,
                //     to_matrix4(tm), view_matrix, proj_matrix);

                draw_platonic_solid(
                    gpu_ico_sphere,
                    shader_phong,
                    cmd.color,
                    to_matrix4(tm), view_matrix, proj_matrix);
            }
            else if (cmd.tag == RenderCommand_RenderUi)
            {
                ASSERT_FAIL("RenderUI command in wrong array!");
            }
        }
        ctx.rend_commands.clear();

#if 0
        {
            // Add sources
            for (int i = 0; i < FLUID_SIZE; i++)
            {
                rho0[i] += input.dt * s[i];
            }

            // Diffusion
            float32 diff = 0.1;
            float32 a = input.dt * diff * FLUID_N * FLUID_N;
            for (int k = 0; k < 20; k++)
            {
                for (int i = 1; i <= FLUID_N; i++)
                {
                    for (int j = 1; j <= FLUID_N; j++)
                    {
                        rho[Ix(i, j)] =
                            (rho0[Ix(i, j)] + a * (rho[Ix(i - 1, j)] +
                                                   rho[Ix(i + 1, j)] +
                                                   rho[Ix(i, j - 1)] +
                                                   rho[Ix(i, j + 1)])) / (1 + 4*a);
                    }
                }
            }

            // printf("rho[%d, %d] = %5.2f\n", FLUID_N/2, FLUID_N/2, rho[Ix(FLUID_N/2, FLUID_N/2)]);
            // printf("----------\n");
            // for (int i = 0; i < FLUID_N + 2; i++)
            // {
            //     for (int j = 0; j < FLUID_N + 2; j++)
            //     {
            //         printf("%5.2f ", rho[Ix(i, j)]);
            //     }
            //     printf("\n");
            // }

            int n = FLUID_N+2;
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    // `2.0 * i/n` is the part that moves squares in the grid
                    // `- 1.0` is the part that moves left bottom square
                    //         from the center to the left bottom corner
                    //         of the screen
                    // `1.0/n` is the part that moves left bottom square
                    //         so that its left bottom corner alignes
                    //         with the left bottom corner of the screen
                    auto m =
                        matrix4::translate(2.0/n * i - 1.0 + 1.0/n,
                                           2.0/n * j - 1.0 + 1.0/n,
                                           0.f) *
                        matrix4::scale(1.0/n);

                    glUseProgram(shader_color.id);
                    shader_color.uniform("u_model", m);
                    shader_color.uniform("u_view", matrix4::identity());
                    shader_color.uniform("u_projection", matrix4::identity());
                    shader_color.uniform("u_color",
                        V4(1.0, 1.0, 1.0, 1e-1 * rho[Ix(i, j)]));

                    glBindVertexArray(gpu_square.vao);
                    glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);
                }
            }
            // Swap
            {
                float *tmp = rho;
                rho = rho0;
                rho0 = tmp;
            }
        }
#endif

        glBindFramebuffer(GL_FRAMEBUFFER, ui_framebuffer.framebuffer_id);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        for (auto cmd : ctx.rend_commands_ui)
        {
            if (cmd.tag == RenderCommand_RenderUi)
            {
                glUseProgram(shader_color.id);

                shader_color.uniform("u_model", cmd.model);
                shader_color.uniform("u_view", matrix4::identity());
                shader_color.uniform("u_projection", proj_matrix_ui);
                shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(gpu_square.vao);
                glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.tag == RenderCommand_RenderBanner)
            {
                auto pNDC = proj_matrix * view_matrix * matrix4::translate(cmd.position) * V4(0, 0, 0, 1);
                pNDC /= pNDC.w;
                auto pUI = matrix4::scale(0.5f * ctx.viewport.width, - 0.5f * ctx.viewport.height, 1)
                         * matrix4::translate(1, -1, 0)
                         * pNDC;
                auto m = matrix4::translate(pUI.xyz) * cmd.model;

                glUseProgram(shader_color.id);

                shader_color.uniform("u_model", m);
                shader_color.uniform("u_view", matrix4::identity());
                shader_color.uniform("u_projection", proj_matrix_ui);
                shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(gpu_square.vao);
                glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.tag == RenderCommand_RenderText)
            {
                if (font_texture.id == 0)
                {
                    PERSIST bool32 log_once = 1;
                    if (log_once)
                    {
                        console::print("Error: font texture isn't loaded!\n");
                        log_once = 0;
                    }
                }
                else
                {
                    glUseProgram(shader_text.id);
                    shader_text.uniform("u_model", cmd.model);
                    shader_text.uniform("u_projection", proj_matrix_ui);
                    shader_text.uniform("u_color", cmd.color);

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, font_texture.id);

                    float32 posx = 0.f;
                    float32 posy = 0.f;
                    uint32 count = 0;

                    string_view strview = string_view::from(cmd.cstr);
                    auto temp_memory = ALLOCATE_ALIGNED_BUFFER(temporary_allocator, strview.size * 24 * sizeof(float32), alignof(float32));
                    auto seri_buffer = serializer::from(temp_memory.data, temp_memory.size);

                    char c = 0;
                    for (char const *str = cmd.cstr; (c = *str) != 0; str++)
                    {
                        glyph g = get_glyph(c);

                        float32 px = (float32) posx - g.origin_x;
                        float32 py = (float32) posy - g.origin_y;
                        float32 w  = (float32) g.width;
                        float32 h  = (float32) g.height;

                        float32 uv_x = (float32) g.x / font_14x26.width;
                        float32 uv_y = (float32) g.y / font_14x26.height;
                        float32 uv_x1 = (float32) (g.x + g.width) / font_14x26.width;
                        float32 uv_y1 = (float32) (g.y + g.height) / font_14x26.height;

                        float32 vbo_data[] = {
                             px,     py,       uv_x,  uv_y,
                             px + w, py,       uv_x1, uv_y,
                             px    , py + h,   uv_x,  uv_y1,

                             px + w, py,       uv_x1, uv_y,
                             px + w, py + h,   uv_x1, uv_y1,
                             px,     py + h,   uv_x,  uv_y1,
                        };

                        seri_buffer.push(vbo_data, sizeof(vbo_data));
                        posx += g.width; // 2 pixels between characters
                        count += 6;
                    }

                    glBindVertexArray(gpu_square_uv.vao);

                    glBindBuffer(GL_ARRAY_BUFFER, gpu_square_uv.vbo);
                    glBufferData(GL_ARRAY_BUFFER, seri_buffer.size, seri_buffer.data, GL_STATIC_DRAW);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);

                    glDrawArrays(GL_TRIANGLES, 0, count);
                }
            }
            else
            {
                ASSERT_FAIL("Render command in wrong array!");
            }
        }
        ctx.rend_commands_ui.clear();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Draw UI on top of everything
        {
            glDisable(GL_DEPTH_TEST);

            glUseProgram(shader_framebuffer.id);
            shader_framebuffer.uniform("u_framebuffer", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ui_framebuffer.color_texture_id);

            glBindVertexArray(gpu_square.vao);
            glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);

            glEnable(GL_DEPTH_TEST);
        }

#if DEBUG
        if (debug_draw_fps_graph_active)
        {
            debug_fps_graph.graph[(debug_fps_graph.index++) % debug_fps_graph.count] = input.dt * 1000.f;
            draw_debug_graph(&debug_fps_graph, &gpu_square, &shader_color);
        }
#endif

        SDL_GL_SwapWindow(window);

        temporary_allocator.reset();

        timepoint end_of_frame = now();
        last_frame_dt = end_of_frame - last_timepoint;
        last_timepoint = end_of_frame;
    }

    SDL_GL_DeleteContext(render_context);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

#include <memory/allocator.cpp>
#include <string_id.cpp>
#include <game/game.cpp>
#include <image/png.cpp>
#include <crc.cpp>
#include <os/platform_posix.cpp>
#include <ecs/entity_manager.cpp>
#include <collision.cpp>
#include <context.cpp>

#include <memory_bucket.cpp>
#include <ecs/archetype.cpp>
#include <platonic_solids.cpp>
#include <debug_graph.cpp>
