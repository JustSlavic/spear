#include "engine.h"
#include "static_shaders.c"
#include "primitive_meshes.h"
#include <corelibs/file_formats/bmp.h>
#include <corelibs/file_formats/png.h>
#include <corelibs/file_formats/wav.h>
#include <corelibs/file_formats/obj.h>
#include <corelibs/parse_primitives.h>

#include <math.h>

#include <gen/font_14x26.h>

#if !DLL_BUILD
#include <engine/game_interface.c>
#include <game_rpg/game.c>
#endif

#include "graphics/render.h"
#if OS_WINDOWS
#include "graphics/opengl_win32.h"
#endif


typedef enum
{
    SpearLoadFile_Success,

    SpearLoadFile_FileDoesNotExist,
    SpearLoadFile_NotEnoughMemory,
    SpearLoadFile_BufferUnderflow,
    SpearLoadFile_DecodeFailed,
} spear_load_file_result;

static spear_load_file_result
spear_engine_load_texture_file(spear_engine *engine,
                               char const *file_name,
                               bitmap *bitmap)
{
    usize file_size = platform_get_file_size(file_name);
    if (file_size == 0)
    {
        REPORT_ERROR("Could not get size of the file '%s', probably because file does not exist.", file_name);
        return SpearLoadFile_FileDoesNotExist;
    }
    void *file_data = ALLOCATE_BUFFER_(engine->temporary, file_size);
    if (file_data == NULL)
    {
        REPORT_ERROR("Failed to allocate %llu bytes needed for file '%s'", file_size, file_name);
        return SpearLoadFile_NotEnoughMemory;
    }
    uint32 bytes_read = platform_read_file_into_memory(file_name, file_data, file_size);
    if (bytes_read < file_size)
    {
        REPORT_ERROR("Reading file (%s) from the disk resulted in less bytes (%u) than file size (%llu)", file_name, bytes_read, file_size);
        return SpearLoadFile_BufferUnderflow;
    }

    uint32 width, height, bits_per_pixel, color_mode, is_top_down;
    int decode_result;
    void *image_data = NULL;
    usize image_size = 0;

    decode_result = bmp_check_signature(file_data, file_size);
    if (decode_result == BmpDecode_Success)
    {
        decode_result = bmp_extract_size(file_data, file_size, (uint32 *) &image_size);
        if (decode_result == BmpDecode_Success && image_size > 0)
        {
            image_data = ALLOCATE_BUFFER_(engine->allocator, image_size);
            if (image_data)
            {
                decode_result = bmp_decode(file_data, file_size, image_data, image_size,
                    &width, &height, &bits_per_pixel, &color_mode, &is_top_down);

                if (decode_result == BmpDecode_Success)
                {
                    printf("Loaded bmp file:\n    image_size = %llu;\n    width = %u;\n    height = %u;\n", image_size, width, height);
                    bitmap->data = image_data;
                    bitmap->size = image_size;
                    bitmap->width = width;
                    bitmap->height = height;
                    bitmap->bits_per_pixel = bits_per_pixel;
                    bitmap->color_mode = color_mode;

                    return SpearLoadFile_Success;
                }
                else
                {
                    REPORT_ERROR("Failed to decode bmp file '%s'", file_name);
                    DEALLOCATE(engine->allocator, image_data);
                    return SpearLoadFile_DecodeFailed;
                }
            }
            else
            {
                REPORT_ERROR("Failed to allocate %llu bytes for file '%s'", image_size, file_name);
                return SpearLoadFile_NotEnoughMemory;
            }
        }
        else
        {
            REPORT_ERROR("Failed to decode size from bmp file '%s'", file_name);
            return SpearLoadFile_DecodeFailed;
        }
    }

    decode_result = png_check_signature(file_data, file_size);
    if (decode_result == PngDecode_Success)
    {
        decode_result = png_extract_size(file_data, file_size, (uint32 *) &image_size);
        if (decode_result == PngDecode_Success && image_size > 0)
        {
            image_data = ALLOCATE_BUFFER_(engine->allocator, image_size);
            if (image_data)
            {
                decode_result = png_decode(file_data, file_size, image_data, image_size,
                    &width, &height, &bits_per_pixel, &color_mode, &is_top_down);

                if (decode_result == PngDecode_Success)
                {
                    printf("Loaded png file: image_size = %llu; width = %u; height = %u;\n", image_size, width, height);
                    bitmap->data = image_data;
                    bitmap->size = image_size;
                    bitmap->width = width;
                    bitmap->height = height;
                    bitmap->bits_per_pixel = bits_per_pixel;
                    bitmap->color_mode = color_mode;

                    return SpearLoadFile_Success;
                }
                else
                {
                    REPORT_ERROR("Failed to decode png file '%s'", file_name);
                    DEALLOCATE(engine->allocator, image_data);
                    return SpearLoadFile_DecodeFailed;
                }
            }
            else
            {
                REPORT_ERROR("Failed to allocate %llu bytes for file '%s'", image_size, file_name);
                return SpearLoadFile_NotEnoughMemory;
            }
        }
        else
        {
            REPORT_ERROR("Failed to decode size from png file '%s'", file_name);
            return SpearLoadFile_DecodeFailed;
        }
    }

    REPORT_ERROR("Failed to recognized file '%s', signatures for bmp and png files does not match", file_name);
    return SpearLoadFile_DecodeFailed;
}

static spear_load_file_result
spear_engine_load_audio_file(spear_engine *engine,
                             char const *file_name,
                             spear_audio_track *audio)
{
    wav_decode_result decode_result;

    usize file_size = platform_get_file_size(file_name);
    if (file_size == 0)
    {
        REPORT_ERROR("Could not get size of the file '%s', probably because file does not exist.", file_name);
        return SpearLoadFile_FileDoesNotExist;
    }

    void *file_data = ALLOCATE_BUFFER_(engine->temporary, file_size);
    if (file_data == NULL)
    {
        REPORT_ERROR("Failed to allocate %llu bytes needed for file '%s'", file_size, file_name);
        return SpearLoadFile_NotEnoughMemory;
    }

    uint32 bytes_read = platform_read_file_into_memory(file_name, file_data, file_size);
    if (bytes_read < file_size)
    {
        REPORT_ERROR("Reading file (%s) from the disk resulted in less bytes (%u) than file size (%llu)", file_name, bytes_read, file_size);
        return SpearLoadFile_BufferUnderflow;
    }

    usize sound_size = 0;
    decode_result = wav_extract_size(file_data, file_size, &sound_size);
    if (decode_result < WavDecode_Success)
    {
        REPORT_ERROR("Decoding audio data size is failed (%s)", wav_decode_result_to_cstring(decode_result));
        return SpearLoadFile_DecodeFailed;
    }

    void *sound_data = ALLOCATE_BUFFER_(engine->allocator, sound_size);
    if (sound_data == NULL)
    {
        REPORT_ERROR("Failed to allocate %llu bytes needed for audio data of audio file '%s'", file_size, file_name);
        return SpearLoadFile_NotEnoughMemory;
    }

    uint32 channel_count, samples_per_second, bits_per_sample;
    decode_result = wav_decode(file_data, file_size, sound_data, sound_size,
        &channel_count, &samples_per_second, &bits_per_sample);

    if (decode_result < WavDecode_Success)
    {
        REPORT_ERROR("Failed to decode audio file '%s'", file_name);
        DEALLOCATE(engine->allocator, sound_data);
        return SpearLoadFile_DecodeFailed;
    }

    printf("Loaded wav file:\n");
    printf("    sound_data = %p\n", sound_data);
    printf("    sound_size = %llu\n", sound_size);
    printf("    channel_count = %u\n", channel_count);
    printf("    samples_per_second = %u\n", samples_per_second);
    printf("    bits_per_sample = %u\n", bits_per_sample);

    audio->data = sound_data;
    audio->size = sound_size;
    audio->samples_per_second = samples_per_second;
    audio->channel_count = channel_count;
    audio->bits_per_sample = bits_per_sample;

    return SpearLoadFile_Success;
}

void
spear_engine_load_game_data(spear_engine *engine)
{
    spear_load_file_result load_result;

    {
        char const *filename = "../misc/test2.png";
        load_result = spear_engine_load_texture_file(engine, filename, &engine->test_bmp);
        if (load_result == SpearLoadFile_Success)
        {
            engine->test_tx = load_texture(engine->test_bmp);
            printf("Loaded texture (id=%d)\n", engine->test_tx.id);
        }
        else
        {
            REPORT_ERROR("Failed to load a texture from disk: filename='%s'", filename);
        }
    }
    {
        char const *filename = "../data/font_14x26.png";
        load_result = spear_engine_load_texture_file(engine, filename, &engine->font_bmp);
        if (load_result == SpearLoadFile_Success)
        {
            engine->font_atlas = load_texture(engine->font_bmp);
            printf("Loaded texture (id=%d)\n", engine->font_atlas.id);
        }
        else
        {
            REPORT_ERROR("Failed to load a texture from disk: filename='%s'", filename);
        }
    }

    load_result = spear_engine_load_audio_file(engine, "../data/rain_loop.wav", &engine->audio_buffer_rain);
    if (load_result == SpearLoadFile_Success)
    {
        uint32 sample_count_in_file = engine->audio_buffer_rain.size / sizeof(int16);
        engine->audio_rain = spear_audio_add_track(&engine->audio, (int16 *) engine->audio_buffer_rain.data, sample_count_in_file);
        spear_audio_play_once(&engine->audio, engine->audio_rain);
    }
    load_result = spear_engine_load_audio_file(engine, "../data/thunder.wav", &engine->audio_buffer_thunder);
    if (load_result == SpearLoadFile_Success)
    {
        uint32 sample_count_in_file = engine->audio_buffer_thunder.size / sizeof(int16);
        engine->audio_thunder = spear_audio_add_track(&engine->audio, (int16 *) engine->audio_buffer_thunder.data, sample_count_in_file);
        spear_audio_play_once(&engine->audio, engine->audio_thunder);
    }

    {
        char const *obj_filename = "../data/suzanne.obj";
        usize file_size = platform_get_file_size(obj_filename);
        printf("Obj size = %llu\n", file_size);
        void *file_data = ALLOCATE_BUFFER_(engine->temporary, file_size);
        uint32 bytes_read = platform_read_file_into_memory(obj_filename, file_data, file_size);
        ASSERT(bytes_read == file_size);

        obj_decode_result decode_result;

        float *vertices = NULL;
        uint32 vertex_buffer_size = 0;

        int32 *indices = NULL;
        uint32 index_buffer_size = 0;
#if 0
        obj_extract_size(file_data, file_size, &vertex_buffer_size, &index_buffer_size);
        printf("EXTRACTED SIZES: %u; %u\n", vertex_buffer_size, index_buffer_size);

        vertices = ALLOCATE_BUFFER_(engine->allocator, vertex_buffer_size);
        indices = ALLOCATE_BUFFER_(engine->allocator, index_buffer_size);

        decode_result = obj_decode(file_data, file_size,
           vertices, vertex_buffer_size,
           indices, index_buffer_size);
#else
        decode_result = obj_decode_no_index(file_data, file_size,
            (void **) &vertices, &vertex_buffer_size);
#endif

        UNUSED(decode_result);
        printf("DECODE RESULT = %s\n", obj_decode_result_to_cstring(decode_result));

        vertex_buffer_layout vbl = {};
        render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);
        render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);

        cpu_mesh suzanne =
        {
            .vbo = {
                .data = vertices,
                .size = vertex_buffer_size,
            },
            .ibo = {
                .data = indices,
                .size = index_buffer_size,
            },
            .vbl = vbl,
        };

        engine->mesh_suzanne = render_load_mesh_to_gpu(suzanne);
    }
}


void spear_engine_init(spear_engine *engine)
{
    engine->running = false;
    engine->current_client_width = 1200;
    engine->current_client_height = 800;

    usize allocator_size = MEGABYTES(20);
    usize temporary_size = MEGABYTES(20);
    usize game_memory_size = MEGABYTES(2);
    usize memory_size = allocator_size + temporary_size + game_memory_size;

    void *memory = platform_allocate_pages((void *) TERABYTES(1), memory_size);
    engine->memory.data = memory;
    engine->memory.size = memory_size;

    engine->allocator = memory_allocator_arena_create(memory, allocator_size, "engine");
    engine->temporary = memory_allocator_arena_create((byte *) memory + allocator_size, temporary_size, "temporary");
    engine->game_memory.data = (byte *) memory + allocator_size + temporary_size;
    engine->game_memory.size = game_memory_size;

    engine->game_dll = ALLOCATE(engine->allocator, struct dll);

    engine->fov = degrees_to_radians(60);
    engine->aspect_ratio = 16.0f / 9.0f;
    engine->near_clip_distance = 0.05f;
    engine->near_clip_width = 2 * engine->near_clip_distance * tanf(0.5f * engine->fov);
    engine->near_clip_height = engine->near_clip_width / engine->aspect_ratio;
    engine->far_clip_distance = 10000.f;

    engine->game_context.temporary_allocator = engine->temporary;
    engine->game_context.near_clip_distance = engine->near_clip_distance;
    engine->game_context.near_clip_width = engine->near_clip_width;
    engine->game_context.near_clip_height = engine->near_clip_height;

    engine->renderer.view_matrix = matrix4_identity();
    renderer_setup_projection(&engine->renderer,
        engine->fov,
        engine->aspect_ratio,
        engine->near_clip_distance,
        engine->far_clip_distance);

    spear_audio_init(&engine->audio);
    engine->audio.master_volume = 0.1;

    engine->sound_debug_position_count = 100;
    engine->sound_debug_positions_read = ALLOCATE_ARRAY(engine->allocator, float, engine->sound_debug_position_count);
    engine->sound_debug_positions_write = ALLOCATE_ARRAY(engine->allocator, float, engine->sound_debug_position_count);
    engine->sound_debug_positions_latency = ALLOCATE_ARRAY(engine->allocator, float, engine->sound_debug_position_count);
}

void spear_engine_init_graphics(spear_engine *engine)
{
    renderer_init_api(&engine->renderer);

    {
        // Prepare buffer for text
        uint32 vbo_id = 0;
        uint32 vao_id = 0;
        glGenBuffers(1, &vbo_id);
        glGenVertexArrays(1, &vao_id);
        glBindVertexArray(vao_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,        // Index
            2,        // Count
            GL_FLOAT, // Type
            GL_FALSE, // Normalized?
            4 * sizeof(float), // Stride
            (void *) (0));       // Offset

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,        // Index
            2,        // Count
            GL_FLOAT, // Type
            GL_FALSE, // Normalized?
            4 * sizeof(float), // Stride
            (void *) (2 * sizeof(float))); // Offset

        engine->text_buffer.vbo = vbo_id;
        engine->text_buffer.ibo = 0;
        engine->text_buffer.vao = vao_id;
        engine->text_buffer.vertex_count = 0;
        engine->text_buffer.element_count = 0;
    }

    {
        // Prepare framebuffer to draw from light source perspective
        int32 width = 1600;
        int32 height = 900;
        engine->light_framebuffer = renderer_framebuffer_create(width, height);
        engine->light_viewport = render_viewport_create(width, height, 16.f / 9.f);
    }

    {
        int32 width = 1024;
        int32 height = 1024;
        float32 aspect_ratio = (float32) width / (float32) height;
        engine->depth_buffer_viewport = render_viewport_create(width, height, aspect_ratio);

        uint32 framebuffer_id = 0;
        glGenFramebuffers(1, &framebuffer_id);

        uint32 texture_id = 0;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_id, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        engine->depth_buffer.framebuffer_id = framebuffer_id;
        engine->depth_buffer.color_texture_id = texture_id;
    }
}

void spear_engine_create_meshes(spear_engine *engine)
{
    engine->mesh_square = render_load_mesh_to_gpu(mesh_square_create());
    engine->mesh_square_uv = render_load_mesh_to_gpu(mesh_square_create_uv());
    engine->mesh_tetrahedron = render_load_mesh_to_gpu(mesh_tetrahedron_create());
    engine->mesh_cube = render_load_mesh_to_gpu(mesh_cube_create());
    engine->mesh_octahedron = render_load_mesh_to_gpu(mesh_octahedron_create());
    engine->mesh_icosahedron = render_load_mesh_to_gpu(mesh_icosahedron_create());
    engine->mesh_sphere = render_load_mesh_to_gpu(mesh_sphere_create(10, 10));
    engine->mesh_ico_sphere = render_load_mesh_to_gpu(mesh_ico_sphere_create(engine->allocator, engine->temporary));
    engine->mesh_ui_frame = render_load_mesh_to_gpu(mesh_ui_frame_create());
}

void spear_engine_compile_shaders(spear_engine *engine)
{
    engine->shader_single_color = render_compile_shaders(vs_single_color, fs_pass_color);
    engine->shader_textured = render_compile_shaders(vs_textured, fs_textured);
    engine->shader_framebuffer = render_compile_shaders(vs_framebuffer, fs_framebuffer);
    engine->shader_text = render_compile_shaders(vs_text, fs_text);
    engine->shader_phong = render_compile_shaders(vs_phong, fs_phong);
    engine->shader_depth_map = render_compile_shaders(vs_depth_map, fs_depth_map);
    engine->shader_sun = render_compile_shaders(vs_sun, fs_sun);
    engine->shader_ui_frame = render_compile_shaders(vs_frame, fs_pass_color);
}

void spear_engine_load_game_dll(spear_engine *engine, char const *filename)
{
#if DLL_BUILD
    timestamp_t dll_last_modified_time = platform_file_get_last_modified_time(filename);
    if (engine->game_dll_timestamp < dll_last_modified_time)
    {
        if (platform_dll_is_valid(engine->game_dll))
        {
            platform_dll_close(engine->game_dll);
        }
        platform_dll_open(engine->game_dll, filename);
        engine->initialize_memory = (initialize_memory_t *) platform_dll_get_function(engine->game_dll, "initialize_memory");
        engine->update_and_render = (update_and_render_t *) platform_dll_get_function(engine->game_dll, "update_and_render");
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
    // #error "Not supported"
    engine->initialize_memory = initialize_memory;
    engine->update_and_render = update_and_render;
#endif
}

void spear_engine_input_mouse_pos_set(spear_engine *engine, spear_input *input, int mouse_x, int mouse_y)
{
    input->keyboard_and_mouse.window_mouse.x = mouse_x;
    input->keyboard_and_mouse.window_mouse.y = mouse_y;
    input->keyboard_and_mouse.viewport_mouse.x = mouse_x - engine->viewport.offset_x;
    input->keyboard_and_mouse.viewport_mouse.y = mouse_y - engine->viewport.offset_y;
}

void spear_engine_update_viewport(spear_engine *engine, int width, int height)
{
    viewport viewport = render_viewport_create(width, height, engine->aspect_ratio);
    glViewport(viewport.offset_x, viewport.offset_y, viewport.width, viewport.height);

    engine->viewport = viewport;
    engine->current_client_width = width;
    engine->current_client_height = height;

    engine->game_context.viewport_offset_x = viewport.offset_x;
    engine->game_context.viewport_offset_y = viewport.offset_y;
    engine->game_context.viewport_width = viewport.width;
    engine->game_context.viewport_height = viewport.height;

    engine->renderer.proj_matrix_ui =
        matrix4_mul(
            matrix4_translate(-1, 1, 0),
            matrix4_scale(2.f / viewport.width, -2.f/viewport.height, 1));
}

void spear_engine_game_init(spear_engine *engine)
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

void spear_engine_game_update(spear_engine *engine, spear_input *input)
{
    if (engine->update_and_render)
    {
        engine->update_and_render(&engine->game_context, engine->game_memory, input);
    }
    else
    {
        printf("Could not run 'update_and_render' function.\n");
    }

    uint engine_command_index;
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

void spear_engine_game_sound(spear_engine *engine, spear_sound_output_buffer *output)
{
    spear_audio_mix(&engine->audio, output);
}

static void spear_engine_draw_mesh(spear_engine *engine, render_command cmd)
{
    matrix4 model =
        matrix4_mul(
            matrix4_translate(cmd.mesh_position.x, cmd.mesh_position.y, cmd.mesh_position.z),
            matrix4_scale(cmd.mesh_scale.x, cmd.mesh_scale.y, cmd.mesh_scale.z)
        );
    gpu_mesh m = cmd.mesh_tag == RenderCommand_DrawMesh_Square ? engine->mesh_square :
                 cmd.mesh_tag == RenderCommand_DrawMesh_Cube ? engine->mesh_cube :
                 cmd.mesh_tag == RenderCommand_DrawMesh_Suzanne ? engine->mesh_suzanne :
                 engine->mesh_cube;
    gpu_shader s = cmd.mesh_shader_tag == RenderCommand_DrawShader_SingleColor ? engine->shader_single_color :
                   cmd.mesh_shader_tag == RenderCommand_DrawShader_Phong ? engine->shader_phong :
                   engine->shader_single_color;

    renderer_draw_mesh(&engine->renderer, model, m, s, cmd.mesh_color);
}

static void spear_engine_draw_ui(spear_engine *engine, render_command cmd)
{
    if (cmd.mesh_tag == RenderCommand_DrawMesh_UiFrame)
    {
        matrix4 model = matrix4_mul(
            transform_to_matrix4(cmd.ui_tm),
            matrix4_mul(
                matrix4_translate(cmd.ui_offset.x, cmd.ui_offset.y, 0),
                matrix4_scale(0.5f * cmd.ui_width, 0.5f * cmd.ui_height, 1)));

        if (cmd.ui_width < 50)
            printf("!!! cmd.ui_width = %f\n", cmd.ui_width);

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
        matrix4 model = matrix4_mul(
            transform_to_matrix4(cmd.ui_tm),
            matrix4_scale(0.5f * cmd.ui_width, 0.5f * cmd.ui_height, 1));
        renderer_draw_mesh_ui(&engine->renderer,
            model,
            engine->mesh_square,
            engine->shader_single_color,
            cmd.ui_color);
    }
}

void spear_engine_draw_ui_text(spear_engine *engine, render_command cmd)
{
    int vertex_count = 0;
    {
        // Prepare buffer
        uint32 buffer_index = 0;
        uint32 buffer_size = cmd.text.size * 24 * sizeof(float32);
        float32 *buffer = ALLOCATE_BUFFER(engine->temporary, buffer_size);

        float32 posx = 0.f;
        float32 posy = 0.f;
        int i;
        for (i = 0; i < cmd.text.size; i++)
        {
            glyph g = get_glyph(cmd.text.data[i]);
            float32 px = (float32) posx;
            float32 py = (float32) posy;
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

            memcpy(buffer + buffer_index, vbo_data, 24 * sizeof(float32));
            buffer_index += 24;

            posx += g.width + 1;
            vertex_count += 6;
        }

        glBindVertexArray(engine->text_buffer.vao);
        glBindBuffer(GL_ARRAY_BUFFER, engine->text_buffer.vbo);
        glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    {
        // Draw triangles
        matrix4 model_translate = matrix4_translate(cmd.ui_position.x, cmd.ui_position.y, 0.f);
        matrix4 model_scale = matrix4_scale(1.f, 1.f, 1.f);
        matrix4 model = matrix4_mul(model_translate, model_scale);
        matrix4 projection = engine->renderer.proj_matrix_ui;
        vector4 color = vector4_create(1.f, 1.f, 1.f, 1.f);

        gpu_shader *shader = &engine->shader_text;
        glUseProgram(shader->id);
        render_shader_uniform_matrix4f(*shader, "u_model", (float *) &model);
        render_shader_uniform_matrix4f(*shader, "u_projection", (float *) &projection);
        render_shader_uniform_vector4f(*shader, "u_color", (float *) &color);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, engine->font_atlas.id);

        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(engine->text_buffer.vao);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
        glEnable(GL_DEPTH_TEST);
    }
}

void spear_engine_draw_scene(spear_engine *engine)
{
    uint render_command_index;
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
#if 0
                // Draw background color, so it's easier to see the viewport inside a bigger window.
                glDisable(GL_DEPTH_TEST);
                renderer_draw_mesh_ui(&engine->renderer,
                    matrix4_scale(10000.f, 10000.f, 1.f),
                    engine->mesh_square,
                    engine->shader_single_color,
                    vector4_create(0.1f, 0.1f, 0.1f, 1.f));
                glEnable(GL_DEPTH_TEST);
#endif
            }
            break;

            case RenderCommand_Wireframe:
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            break;

            case RenderCommand_DrawMesh:
            {
                spear_engine_draw_mesh(engine, cmd);
            }
            break;

            case RenderCommand_DrawUi:
            {
                spear_engine_draw_ui(engine, cmd);
            }
            break;

            case RenderCommand_UiText:
            {
                spear_engine_draw_ui_text(engine, cmd);
            }
            break;

            case RenderCommand_Invalid:
                ASSERT_MSG(0, "Engine: RenderCommand_Invalid (= 0) in the command queue.\n");
            break;
        }
    }
}

void spear_engine_game_render(spear_engine *engine)
{
    if (1)
    {
        // Render to depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, engine->depth_buffer.framebuffer_id);
        glViewport(
            engine->depth_buffer_viewport.offset_x,
            engine->depth_buffer_viewport.offset_y,
            engine->depth_buffer_viewport.width,
            engine->depth_buffer_viewport.height);
        glClear(GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Setup projection and view matrices
        light_source_info *light_info = &engine->game_context.light_source;
        matrix4 projection_matrix = make_orthographic_matrix(10.f, 10.f,
            10.f, 20.f);
        matrix4 view_matrix = make_lookat_matrix_from_camera(
            light_info->position,
            vector3_sub(vector3_create(0.f, 0.f, 0.f), light_info->position),
            vector3_create(0.f, 0.f, 1.f));

        // Setup shader
        gpu_shader shader = engine->shader_depth_map;
        glUseProgram(shader.id);
        render_shader_uniform_matrix4f(shader, "u_projection", (float32 *) &projection_matrix);
        render_shader_uniform_matrix4f(shader, "u_view", (float32 *) &view_matrix);

        // Draw scene
        uint render_command_index;
        for (render_command_index = 0;
             render_command_index < engine->game_context.render_commands_count;
             render_command_index++)
        {
            render_command cmd = engine->game_context.render_commands[render_command_index];
            switch (cmd.tag)
            {
                case RenderCommand_DrawMesh:
                {
                    matrix4 model_translate = matrix4_translate(cmd.mesh_position.x, cmd.mesh_position.y, cmd.mesh_position.z);
                    matrix4 model_scale = matrix4_scale(cmd.mesh_scale.x, cmd.mesh_scale.y, cmd.mesh_scale.z);
                    matrix4 model_matrix = matrix4_mul(model_translate, model_scale);

                    gpu_mesh mesh = cmd.mesh_tag == RenderCommand_DrawMesh_Square ? engine->mesh_square :
                                    cmd.mesh_tag == RenderCommand_DrawMesh_Cube ? engine->mesh_cube :
                                    cmd.mesh_tag == RenderCommand_DrawMesh_Suzanne ? engine->mesh_suzanne :
                                    engine->mesh_cube;

                    render_shader_uniform_matrix4f(shader, "u_model", (float32 *) &model_matrix);

                    glBindVertexArray(mesh.vao);
                    if (mesh.element_count > 0)
                    {
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
                        glDrawElements(GL_TRIANGLES, mesh.element_count, GL_UNSIGNED_INT, NULL);
                    }
                    else
                    {
                        glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count);
                    }
                }
                break;

                case RenderCommand_SetupCamera:
                case RenderCommand_Wireframe:
                case RenderCommand_DrawUi:
                case RenderCommand_UiText:
                break;

                case RenderCommand_Invalid:
                    ASSERT_MSG(0, "Engine: RenderCommand_Invalid (= 0) in the command queue.\n");
                break;
            }
        }
    }

    if (1)
    {
        // Draw the scene to the framebuffer
        gpu_framebuffer fb = engine->light_framebuffer;
        glBindFramebuffer(GL_FRAMEBUFFER, fb.framebuffer_id);
        glViewport(engine->light_viewport.offset_x, engine->light_viewport.offset_y,
            engine->light_viewport.width, engine->light_viewport.height);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        {
            // Draw meshes for casting shadows
            matrix4 projection_matrix = make_projection_matrix_fov(
                engine->fov,
                engine->aspect_ratio,
                engine->near_clip_distance,
                engine->far_clip_distance);

            light_source_info *light_info = &engine->game_context.light_source;
            matrix4 view_matrix = make_lookat_matrix_from_camera(
                light_info->position,
                vector3_sub(vector3_create(0.f, 0.f, 0.f), light_info->position),
                vector3_create(0.f, 0.f, 1.f));

            // Draw only meshes
            uint render_command_index;
            for (render_command_index = 0;
                 render_command_index < engine->game_context.render_commands_count;
                 render_command_index++)
            {
                render_command cmd = engine->game_context.render_commands[render_command_index];
                switch (cmd.tag)
                {
                    case RenderCommand_DrawMesh:
                    {
                        matrix4 model_translate = matrix4_translate(cmd.mesh_position.x, cmd.mesh_position.y, cmd.mesh_position.z);
                        matrix4 model_scale = matrix4_scale(cmd.mesh_scale.x, cmd.mesh_scale.y, cmd.mesh_scale.z);
                        matrix4 model_matrix = matrix4_mul(model_translate, model_scale);

                        gpu_mesh mesh = cmd.mesh_tag == RenderCommand_DrawMesh_Square ? engine->mesh_square :
                                        cmd.mesh_tag == RenderCommand_DrawMesh_Cube ? engine->mesh_cube :
                                        cmd.mesh_tag == RenderCommand_DrawMesh_Suzanne ? engine->mesh_suzanne :
                                        engine->mesh_cube;
                        gpu_shader shader = cmd.mesh_shader_tag == RenderCommand_DrawShader_SingleColor ? engine->shader_single_color :
                                            cmd.mesh_shader_tag == RenderCommand_DrawShader_Phong ? engine->shader_phong :
                                            engine->shader_single_color;

                        glUseProgram(shader.id);

                        render_shader_uniform_matrix4f(shader, "u_model", (float32 *) &model_matrix);
                        render_shader_uniform_matrix4f(shader, "u_view", (float32 *) &view_matrix);
                        render_shader_uniform_matrix4f(shader, "u_projection", (float32 *) &projection_matrix);
                        render_shader_uniform_vector4f(shader, "u_color", (float32 *) &cmd.mesh_color);

                        glBindVertexArray(mesh.vao);
                        if (mesh.element_count > 0)
                        {
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
                            glDrawElements(GL_TRIANGLES, mesh.element_count, GL_UNSIGNED_INT, NULL);
                        }
                        else
                        {
                            glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count);
                        }
                    }
                    break;

                    case RenderCommand_SetupCamera:
                    case RenderCommand_Wireframe:
                    case RenderCommand_DrawUi:
                    case RenderCommand_UiText:
                    break;

                    case RenderCommand_Invalid:
                        ASSERT_MSG(0, "Engine: RenderCommand_Invalid (= 0) in the command queue.\n");
                    break;
                }
            }
        }
    }

    if (1)
    {
        // Draw the regular scene
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(engine->viewport.offset_x, engine->viewport.offset_y,
            engine->viewport.width, engine->viewport.height);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        spear_engine_draw_scene(engine);
    }

    // Draw framebuffer contents on top of everything
    if (1)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(
            engine->viewport.offset_x + engine->viewport.width * 0.5f,
            engine->viewport.offset_y + engine->viewport.height * 0.5f,
            engine->viewport.width * 0.5f,
            engine->viewport.height * 0.5f);

        // glClearColor(0.f, 0.f, 0.f, 0.f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        glUseProgram(engine->shader_framebuffer.id);
        render_shader_uniform_int(engine->shader_framebuffer, "u_framebuffer", 0);
        render_shader_uniform_int(engine->shader_framebuffer, "u_is_depth", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, engine->light_framebuffer.color_texture_id);

        glBindVertexArray(engine->mesh_square_uv.vao);
        glDrawElements(GL_TRIANGLES, engine->mesh_square_uv.element_count, GL_UNSIGNED_INT, NULL);

        glEnable(GL_DEPTH_TEST);
    }

    // Draw depth map contents on top of everything
    if (1)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(
            engine->viewport.offset_x + engine->viewport.width * 0.5f,
            engine->viewport.offset_y,
            engine->viewport.width * 0.5f,
            engine->viewport.height * 0.5f);

        // glClearColor(0.f, 0.f, 0.f, 0.f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        glUseProgram(engine->shader_framebuffer.id);
        render_shader_uniform_int(engine->shader_framebuffer, "u_framebuffer", 0);
        render_shader_uniform_int(engine->shader_framebuffer, "u_is_depth", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, engine->depth_buffer.color_texture_id);

        glBindVertexArray(engine->mesh_square_uv.vao);
        glDrawElements(GL_TRIANGLES, engine->mesh_square_uv.element_count, GL_UNSIGNED_INT, NULL);

        glEnable(GL_DEPTH_TEST);
    }

    engine->game_context.render_commands_count = 0;

#if 0
    // Draw texture debug
    {
        matrix4 model = matrix4_translate(0.f, 0.f, 2.f);
        glUseProgram(engine->shader_textured.id);
        render_shader_uniform_matrix4f(engine->shader_textured, "u_model", (float *) &model);
        render_shader_uniform_matrix4f(engine->shader_textured, "u_view", (float *) &engine->renderer.view_matrix);
        render_shader_uniform_matrix4f(engine->shader_textured, "u_projection", (float *) &engine->renderer.proj_matrix);
        render_shader_uniform_int(engine->shader_textured, "is_upside_down", true);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, engine->font_atlas.id);

        glBindVertexArray(engine->mesh_square_uv.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->mesh_square_uv.ibo);
        glDrawElements(GL_TRIANGLES, engine->mesh_square_uv.element_count, GL_UNSIGNED_INT, NULL);
    }
#endif
#if 0
    // Draw audio debug
    // if (false)
    // {
    //     uint32 channel_count = 2;
    //     uint32 latency_frames = engine->audio.latency * engine->audio.frames_per_second;
    //     uint32 latency_bytes = latency_frames * channel_count * sizeof(sound_sample_t);

    //     uint32 index_read = engine->audio.R;
    //     uint32 index_write = engine->audio.W;

    //     float screen_position_read = (float) index_read / engine->audio.playback_buffer_size * engine->current_client_width;
    //     float screen_position_write = (float) index_write / engine->audio.playback_buffer_size * engine->current_client_width;
    //     float screen_position_latency = (float) (index_read + latency_bytes) / engine->audio.playback_buffer_size * engine->current_client_width;

    //     uint32 index = engine->sound_debug_position_running_index++;
    //     if (engine->sound_debug_position_running_index >= engine->sound_debug_position_count)
    //         engine->sound_debug_position_running_index = 0;

    //     engine->sound_debug_positions_read[index] = screen_position_read;
    //     engine->sound_debug_positions_write[index] = screen_position_write;
    //     engine->sound_debug_positions_latency[index] = screen_position_latency;

    //     uint32 i;
    //     for (i = 0; i < engine->sound_debug_position_count; i++)
    //     {
    //         {
    //             matrix4 model = matrix4_mul(
    //                 matrix4_translate(
    //                     engine->sound_debug_positions_read[i],
    //                     (float) i * engine->viewport.height / engine->sound_debug_position_count,
    //                     0.f),
    //                 matrix4_scale(1.f, 50.f, 1.f));
    //             matrix4 view = matrix4_identity();
    //             vector4 color = vector4_create(1.f, 0.f, 0.f, 1.f);
    //             glUseProgram(engine->shader_single_color.id);
    //             render_shader_uniform_matrix4f(engine->shader_single_color, "u_model", (float *) &model);
    //             render_shader_uniform_matrix4f(engine->shader_single_color, "u_view", (float *) &view);
    //             render_shader_uniform_matrix4f(engine->shader_single_color, "u_projection", (float *) &engine->renderer.proj_matrix_ui);
    //             render_shader_uniform_vector4f(engine->shader_single_color, "u_color", (float *) &color);

    //             // glActiveTexture(GL_TEXTURE0);
    //             // glBindTexture(GL_TEXTURE_2D, engine->test_tx.id);

    //             glBindVertexArray(engine->mesh_square.vao);
    //             glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->mesh_square.ibo);
    //             glDrawElements(GL_TRIANGLES, engine->mesh_square.element_count, GL_UNSIGNED_INT, NULL);
    //         }
    //         {
    //             vector4 color = vector4_create(0.f, 1.f, 0.f, 1.f);
    //             float W0 = engine->sound_debug_positions_write[i];
    //             float W1 = engine->sound_debug_positions_write[(i + 1) % engine->sound_debug_position_count];

    //             float p = 0.5f * (W0 + W1);
    //             if (p < W0) continue;
    //             float len = fabs(engine->sound_debug_positions_write[(i + 1) % engine->sound_debug_position_count] - engine->sound_debug_positions_write[i]);

    //             matrix4 model = matrix4_mul(
    //                 matrix4_translate(
    //                     p,
    //                     (float) i * engine->viewport.height / engine->sound_debug_position_count,
    //                     0.f),
    //                 matrix4_scale(len*0.5f, 1.f, 1.f));
    //             matrix4 view = matrix4_identity();
    //             glUseProgram(engine->shader_single_color.id);
    //             render_shader_uniform_matrix4f(engine->shader_single_color, "u_model", (float *) &model);
    //             render_shader_uniform_matrix4f(engine->shader_single_color, "u_view", (float *) &view);
    //             render_shader_uniform_matrix4f(engine->shader_single_color, "u_projection", (float *) &engine->renderer.proj_matrix_ui);
    //             render_shader_uniform_vector4f(engine->shader_single_color, "u_color", (float *) &color);

    //             // glActiveTexture(GL_TEXTURE0);
    //             // glBindTexture(GL_TEXTURE_2D, engine->test_tx.id);

    //             glBindVertexArray(engine->mesh_square.vao);
    //             glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->mesh_square.ibo);
    //             glDrawElements(GL_TRIANGLES, engine->mesh_square.element_count, GL_UNSIGNED_INT, NULL);
    //         }
    //         {
    //             float p = 0.5f * (engine->sound_debug_positions_read[i] + engine->sound_debug_positions_latency[i]);
    //             float len = fabs(engine->sound_debug_positions_read[i] - engine->sound_debug_positions_latency[i]);
    //             matrix4 model = matrix4_mul(
    //                 matrix4_translate(
    //                     p,
    //                     (float) i * engine->viewport.height / engine->sound_debug_position_count,
    //                     0.f),
    //                 matrix4_scale(0.5f * len, 1.f, 1.f));
    //             matrix4 view = matrix4_identity();
    //             vector4 color = vector4_create(1.f, 1.f, 0.f, 1.f);
    //             glUseProgram(engine->shader_single_color.id);
    //             render_shader_uniform_matrix4f(engine->shader_single_color, "u_model", (float *) &model);
    //             render_shader_uniform_matrix4f(engine->shader_single_color, "u_view", (float *) &view);
    //             render_shader_uniform_matrix4f(engine->shader_single_color, "u_projection", (float *) &engine->renderer.proj_matrix_ui);
    //             render_shader_uniform_vector4f(engine->shader_single_color, "u_color", (float *) &color);

    //             // glActiveTexture(GL_TEXTURE0);
    //             // glBindTexture(GL_TEXTURE_2D, engine->test_tx.id);

    //             glBindVertexArray(engine->mesh_square.vao);
    //             glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->mesh_square.ibo);
    //             glDrawElements(GL_TRIANGLES, engine->mesh_square.element_count, GL_UNSIGNED_INT, NULL);
    //         }
    //     }
    // }
#endif
}
