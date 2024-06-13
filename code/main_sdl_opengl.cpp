#include <base.h>
#include <time.hpp>
#include <input.hpp>
#include <game_interface.hpp>
#include <platform_sdl.hpp>
#include <hash.hpp>

#include <gfx/viewport.hpp>
#include <gfx/static_shaders.cpp>

#include <SDL2/SDL.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>


GLOBAL bool32 running;
GLOBAL int32 current_client_width;
GLOBAL int32 current_client_height;
GLOBAL bool32 viewport_changed;


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
                auto key = sdl::map_button_from_scancode(e.key.keysym.scancode);
                if (key != KB_NONE)
                {
                    process_button_state(&input->keyboard[key], e.type == SDL_KEYDOWN);
                }
            }
            break;
        }
    }
}

matrix4 make_look_at_matrix(vector3 eye, vector3 at, vector3 up)
{
    vector3 f = normalized(at - eye);
    vector3 s = normalized(cross(f, up));
    vector3 u = cross(s, f);

    matrix4 result = {};
    result._1 = V4(s, -inner(s, eye));
    result._2 = V4(u, -inner(u, eye));
    result._3 = V4(-f, inner(f, eye));
    result._4 = V4(0, 0, 0, 1);

    return result;
}

matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f)
{
    //     w/2
    //   +-----+
    //   |    /
    //   |   /
    // n |  /
    //   | / angle = fov/2
    //   |/  tg(fov / 2) = (w/2) / n
    //   +   => 2n / w = 1 / tg(fov / 2)

    float32 tf2 = (1.0f / tanf(0.5f * fov));

    matrix4 result = {};

    result._11 = tf2;
    result._22 = tf2 * aspect_ratio;
    result._33 = -(f + n) / (f - n);
    result._34 = -2.0f * f * n / (f - n);
    result._43 = -1.0f;

    return result;
}

struct cpu_mesh
{
    memory_buffer vbo;
    memory_buffer ibo;
    gfx::vertex_buffer_layout vbl;
};

struct gpu_mesh
{
    uint32 vbo;
    uint32 ibo;
    uint32 vao;
    uint32 count;
};

struct shader
{
    uint32 id;
    uint32 vs_id;
    uint32 fs_id;

    enum shader_type
    {
        vertex = GL_VERTEX_SHADER,
        fragment = GL_FRAGMENT_SHADER,
    };

    void uniform(char const *name, int32 n)
    {
        auto location = glGetUniformLocation(id, name);
        glUniform1i(location, n);
    }

    void uniform(char const *name, float f)
    {
        auto location = glGetUniformLocation(id, name);
        glUniform1f(location, f);
    }

    void uniform(char const *name, vector2 const& v)
    {
        auto location = glGetUniformLocation(id, name);
        glUniform2f(location, v.x, v.y);
    }

    void uniform(char const *name, vector3 const& v)
    {
        auto location = glGetUniformLocation(id, name);
        glUniform3f(location, v.x, v.y, v.z);
    }

    void uniform(char const *name, vector4 const& v)
    {
        auto location = glGetUniformLocation(id, name);
        glUniform4f(location, v.x, v.y, v.z, v.w);
    }

    void uniform(char const *name, matrix4 const& m)
    {
        auto location = glGetUniformLocation(id, name);
        glUniformMatrix4fv(location, 1, GL_TRUE, get_data(m));
    }
};

cpu_mesh make_square()
{
    static float32 vbo_data[] = {
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
    };

    static uint32 ibo_data[] = {
        0, 1, 2, // first triangle
        2, 3, 0, // second triangle
    };

    auto vbo = memory_buffer::from(vbo_data, sizeof(vbo_data));
    auto ibo = memory_buffer::from(ibo_data, sizeof(ibo_data));

    auto vbl = gfx::vertex_buffer_layout::make();
    vbl.push<float>(3);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}

cpu_mesh make_cube()
{
    static float32 vbo_data[] = {
        // bottom square
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        // top square
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
    };

    static uint32 ibo_data[] = {
        0, 1, 2,
        2, 3, 0,

        0, 3, 7,
        7, 4, 0,

        1, 5, 6,
        6, 2, 1,

        3, 2, 6,
        6, 7, 3,

        1, 0, 4,
        4, 5, 1,

        5, 4, 7,
        7, 6, 5,
    };

    auto vbo = memory_buffer::from(vbo_data, sizeof(vbo_data));
    auto ibo = memory_buffer::from(ibo_data, sizeof(ibo_data));

    auto vbl = gfx::vertex_buffer_layout::make();
    vbl.push<float>(3);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}

cpu_mesh make_square_uv()
{
    static float32 vbo_data[] = {
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f,
    };
    static uint32 ibo_data[] = {
        0, 1, 2, // first triangle
        2, 3, 0, // second triangle
    };

    auto vbo = memory_buffer::from(vbo_data, sizeof(vbo_data));
    auto ibo = memory_buffer::from(ibo_data, sizeof(ibo_data));

    auto vbl = gfx::vertex_buffer_layout::make();
    vbl.push<float>(2);
    vbl.push<float>(2);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}

gpu_mesh load_mesh(cpu_mesh mesh)
{
    uint32 vbo_id = 0;
    {
        glGenBuffers(1, &vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, mesh.vbo.size, mesh.vbo.data, GL_STATIC_DRAW);
    }

    uint32 ibo_id = 0;
    {
        glGenBuffers(1, &ibo_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo.size, mesh.ibo.data, GL_STATIC_DRAW);
    }

    uint32 vao_id = 0;
    {
        glGenVertexArrays(1, &vao_id);
        glBindVertexArray(vao_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

        uint32 stride = 0;
        for (uint32 attrib_index = 0; attrib_index < mesh.vbl.size; attrib_index++)
        {
            stride += (mesh.vbl.elements[attrib_index].count * sizeof(float32));
        }

        printf("stride = %d\n", stride);

        usize offset = 0;
        for (uint32 attrib_index = 0; attrib_index < mesh.vbl.size; attrib_index++)
        {
            uint32 count = mesh.vbl.elements[attrib_index].count;
            glEnableVertexAttribArray(attrib_index);
            glVertexAttribPointer(
                attrib_index,      // Index
                count,             // Count
                GL_FLOAT,          // Type
                GL_FALSE,          // Normalized?
                stride,            // Stride
                (void *) offset);  // Offset

            offset += (count * sizeof(float32));
        }
    }

    gpu_mesh result;
    result.vbo = vbo_id;
    result.ibo = ibo_id;
    result.vao = vao_id;
    result.count = (uint32) (mesh.ibo.size / sizeof(uint32));

    printf("Mesh loaded: (vao = %d, vbo = %d, ibo = %d, count = %d)\n", vao_id, vbo_id, ibo_id, result.count);

    return result;
}

int compile_shader(char const *source_code, int shader_type)
{
    int id = glCreateShader(shader_type);
    glShaderSource(id, 1, (char const **) &source_code, (int const *) NULL);
    glCompileShader(id);

    int successful;
    glGetShaderiv(id, GL_COMPILE_STATUS, &successful);
    if (successful == GL_FALSE)
    {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, (int *) &length);

        auto message = malloc(length + 1);
        memset(message, 0, length + 1);

        glGetShaderInfoLog(id, length, &length, (char *) message);
        glDeleteShader(id);

        printf("Could not compile shader: \"%s\"\n", (char *)message);
        free(message);

        return 0;
    }

    return id;
}

shader compile_shaders(char const *vs_code, char const *fs_code)
{
    int vs = compile_shader(vs_code, GL_VERTEX_SHADER);
    int fs = compile_shader(fs_code, GL_FRAGMENT_SHADER);

    int id = glCreateProgram();
    glUseProgram(0);
    glAttachShader(id, vs);
    glAttachShader(id, fs);
    glLinkProgram(id);
    glDetachShader(id, vs);
    glDetachShader(id, fs);

    shader result;
    result.id = id;
    result.vs_id = vs;
    result.fs_id = fs;

    printf("Shaders compiled (id = %d, vs = %d, fs = %d)\n", id, vs, fs);

    return result;
}

#include <ecs/entity_manager.hpp>
#include <ecs/component.hpp>
#include <ecs/archetype.hpp>


void entity_system_1(ecs::event *evt, ecs::entity_id eid)
{
    printf("es: eid=%d\n", eid.id);
}

struct event_update : ecs::event {};


#define ECS_HASH hash_djb2


timepoint now()
{
    timepoint result = { SDL_GetTicks64() * 1000 };
    return result;
}


int main()
{
    current_client_width = 800;
    current_client_height = 600;

    auto global_memory = sdl::allocate_memory((void *) TERABYTES(1), MEGABYTES(50));
    auto global_arena  = memory_allocator::make_arena(global_memory);

    auto game_memory = global_arena.allocate_buffer(MEGABYTES(5));
    auto string_id_allocator = global_arena.allocate_arena(KILOBYTES(10));
    auto string_id_storage = string_id::initialize(string_id_allocator);

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

    GLubyte const *v = glGetString(GL_VERSION);
    printf("glGetString(GL_VERSION) -> %s\n", v);

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
    ctx.far_clip_dist = 100.f;
    ctx.debug_load_file = NULL;

    // ======================================================================

    auto cpu_square = make_square();
    auto gpu_square = load_mesh(cpu_square);

    auto cpu_cube = make_cube();
    auto gpu_cube = load_mesh(cpu_cube);

    auto shader_color = compile_shaders(vs_single_color, fs_pass_color);
    auto shader_ground = compile_shaders(vs_ground, fs_pass_color);

    // ======================================================================

    auto view = matrix4::identity();
    auto projection = make_projection_matrix_fov(to_radians(60), ctx.aspect_ratio, ctx.near_clip_dist, ctx.far_clip_dist);

    // ======================================================================

    auto projection_ui = matrix4::identity();
    // auto ui_render_target = driver.create_render_target(&ctx);

    // driver.use_render_target(ui_render_target);
    // driver.clear_color(1.f, 1.f, 0.f, 1.f);
    // driver.clear();

    // ======================================================================

    // ecs::component comps[] = {
    //     ECS_COMPONENT("eid", ecs::entity_id),
    //     ECS_COMPONENT("abc", int),
    //     ECS_COMPONENT("def", int),
    // };
    // ecs::archetype arch = ecs::make_archetype(comps, ARRAY_COUNT(comps));

    // auto eid1 = manager.create_entity();
    // auto comp_values = temporary_arena.allocate_buffer(sizeof(ecs::entity_id) + sizeof(int) * 2);
    // *(ecs::entity_id *)(comp_values.data) = eid1;
    // *(int *)(comp_values.data + sizeof(ecs::entity_id)) = 32;
    // *(int *)(comp_values.data + sizeof(ecs::entity_id) + sizeof(int)) = 42;
    // ecs::archetype_initializer init = { { comps[0], comps[1], comps[2] }, ARRAY_COUNT(comps), comp_values };
    // arch.allocate(init);

    // for (uint32 i = 0; i < arch.chunk.size / sizeof(int); i++)
    // {
    //     printf("%d ", *(int *)(arch.chunk.data + i*sizeof(int)));
    // }

    // // manager.register_event<event_update>(ECS_HASH("event_update"));
    // // manager.register_system<ECS_HASH("event_update")>(entity_system_1);
    // manager.send_event_immediate(event_update{});

    // ======================================================================

    initialize_memory(&ctx, game_memory);

    input_state input = {};

    float32 last_frame_dt = 0.016f;
    timepoint last_timepoint = now();

    running = true;
    while (running)
    {
        reset_transitions(input.keyboard.buttons, KB_KEY_COUNT);
        reset_transitions(input.mouse.buttons, MOUSE_KEY_COUNT);
        process_pending_messages(&input);
        sdl::get_mouse_pos(&input.mouse.x, &input.mouse.y);
        input.dt = last_frame_dt;
        input.time = last_timepoint;

        if (viewport_changed)
        {
            SDL_GetWindowSize(window, &current_client_width, &current_client_height);
            auto viewport = gfx::viewport::make(current_client_width, current_client_height, ctx.aspect_ratio);
            glViewport(viewport.offset_x, viewport.offset_y, viewport.width, viewport.height);
            viewport_changed = false;

            ctx.viewport = viewport;
            ctx.window_width = current_client_width;
            ctx.window_height = current_client_height;

            projection_ui =
                matrix4::translate(-1, 1, 0) *
                matrix4::scale(2.0f/viewport.width, -2.0f/viewport.height, 1);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        update_and_render(&ctx, game_memory, &input);

        for (auto cmd : ctx.exec_commands)
        {
            switch (cmd.kind)
            {
                case exec_command::exit:
                {
                    running = false;
                }
                break;
            }
        }
        ctx.exec_commands.clear();

        for (auto cmd : ctx.rend_commands)
        {
            if (cmd.kind == rend_command::setup_camera)
            {
                view = make_look_at_matrix(cmd.position, cmd.position + cmd.forward, cmd.up);
            }
            else if (cmd.kind == rend_command::render_square)
            {
                glUseProgram(shader_color.id);

                shader_color.uniform("u_model", cmd.model);
                shader_color.uniform("u_view", view);
                shader_color.uniform("u_projection", projection);
                shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(gpu_square.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_square.ibo);
                glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.kind == rend_command::render_cube)
            {
                shader *s = NULL;
                if (cmd.shader == SHADER_COLOR)
                    s = &shader_color;
                else if (cmd.shader == SHADER_GROUND)
                    s = &shader_ground;
                else
                    ASSERT_FAIL();

                glUseProgram(s->id);

                s->uniform("u_model", cmd.model);
                s->uniform("u_view", view);
                s->uniform("u_projection", projection);
                s->uniform("u_color", cmd.color);

                glBindVertexArray(gpu_cube.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_cube.ibo);
                glDrawElements(GL_TRIANGLES, gpu_cube.count, GL_UNSIGNED_INT, NULL);
            }
            else if (cmd.kind == rend_command::render_ui)
            {
                glUseProgram(shader_color.id);

                shader_color.uniform("u_model", cmd.model);
                shader_color.uniform("u_view", matrix4::identity());
                shader_color.uniform("u_projection", projection_ui);
                shader_color.uniform("u_color", cmd.color);

                glBindVertexArray(gpu_square.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_square.ibo);
                glDrawElements(GL_TRIANGLES, gpu_square.count, GL_UNSIGNED_INT, NULL);
            }
        }
        ctx.rend_commands.clear();

        SDL_GL_SwapWindow(window);

        timepoint end_of_frame = now();
        last_frame_dt = get_seconds(end_of_frame - last_timepoint);
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
#include <crc.cpp>
#include <os/platform_posix.cpp>
#include <ecs/entity_manager.cpp>
#include <collision.cpp>
#include <context.cpp>

#include <memory_bucket.cpp>
#include <ecs/archetype.cpp>
