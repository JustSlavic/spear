#include "engine.h"
#include "static_shaders.c"
#include "primitive_meshes.h"
#include <corelibs/file_formats/wavefront_obj.h>
#include <corelibs/file_formats/bmp.h>
#include <corelibs/file_formats/wav.h>

#include <math.h>


void spear_engine_init(engine *engine)
{
    engine->running = false;
    engine->viewport_changed = false;
    engine->current_client_width = 1200;
    engine->current_client_height = 800;

    usize allocator_size = MEGABYTES(5);
    usize temporary_size = MEGABYTES(5);
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
    printf("fov = %f\n", engine->fov);
    engine->aspect_ratio = 16.0f / 9.0f;
    engine->near_clip_distance = 0.05f;
    engine->near_clip_width = 2 * engine->near_clip_distance * tanf(0.5f * engine->fov);
    engine->near_clip_height = engine->near_clip_width / engine->aspect_ratio;
    engine->far_clip_distance = 10000.f;

    engine->game_context.near_clip_distance = engine->near_clip_distance;
    engine->game_context.near_clip_width = engine->near_clip_width;
    engine->game_context.near_clip_height = engine->near_clip_height;

    engine->renderer.view_matrix = matrix4_identity();
    engine->renderer.proj_matrix = matrix4_identity();
    renderer_setup_projection(&engine->renderer,
        engine->fov,
        engine->aspect_ratio,
        engine->near_clip_distance,
        engine->far_clip_distance);

    // Allocate sine sound buffer
    {
        // Frequency is 44100 samples per second
        // I want sine wave 300Hz (cycles per second)
        // That means 1 cycle will take 44100 samples
        // I will allocate 44100 * channel_count * sizeof(uint16)
        // for 1 second of sound
        // in this one second, I will write 300 sine cycles
        int channel_count = 2;
        int samples_per_second = 44100; // Hz

        int frame_size  = channel_count * sizeof(sound_sample_t);
        int frame_count = samples_per_second;

        int buffer_size = frame_count * frame_size;
        sound_sample_t *buffer = ALLOCATE_BUFFER_(engine->allocator, buffer_size);

        engine->sine_audio.size = buffer_size;
        engine->sine_audio.data = (uint8 *) buffer;

        int tone_hz = 300; // Hz
        int wave_period = samples_per_second / tone_hz;
        sound_sample_t *samples = (sound_sample_t *) engine->sine_audio.data;

        double tone_volume = 2000.0;
        double sine_time = 0;
        uint32 frame_index;
        for (frame_index = 0; frame_index < frame_count; frame_index++)
        {
            double t = sine_time;
            double sine_value = sin(t);
            int16 sample_value = (int16)(sine_value * tone_volume);
            *samples++ = sample_value;
            *samples++ = sample_value;
            sine_time += TWO_PI / wave_period;
            if (sine_time > TWO_PI)
                sine_time = sine_time - TWO_PI;
        }
    }
    engine->audio_latency = 1.0 / 20.0;

    {
        uint32 channel_count = 2;
        engine->master_audio.size = 44100 * channel_count * sizeof(sound_sample_t);
        engine->master_audio.data = ALLOCATE_BUFFER(engine->allocator, engine->master_audio.size);
        engine->master_audio.index_read = 0;
        engine->master_audio.index_write = 0;
    }
    {
        engine->sine_audio_262Hz.frequency = 262.0;
        engine->sine_audio_262Hz.volume = 1000.0;
        engine->sine_audio_262Hz.running_t = 0;
    }
    {
        engine->sine_audio_500Hz.frequency = 500.0;
        engine->sine_audio_500Hz.volume = 1000.0;
        engine->sine_audio_500Hz.running_t = 0;
    }

    spear_audio_init(&engine->master_audio);

    engine->sound_debug_position_count = 100;
    engine->sound_debug_positions_read = ALLOCATE_ARRAY(engine->allocator, float, engine->sound_debug_position_count);
    engine->sound_debug_positions_write = ALLOCATE_ARRAY(engine->allocator, float, engine->sound_debug_position_count);
    engine->sound_debug_positions_latency = ALLOCATE_ARRAY(engine->allocator, float, engine->sound_debug_position_count);
}

void spear_engine_audio_mix(engine *engine)
{
    audio_buffer *audio = &engine->master_audio;

    uint32 channel_count = 2;
    uint32 frame_size = channel_count * sizeof(sound_sample_t);
    int samples_per_second = 44100; // Hz
    uint32 latency_frames = engine->audio_latency * samples_per_second;
    uint32 latency_bytes = latency_frames * channel_count * sizeof(sound_sample_t);

    uint32 R = audio->index_read;
    uint32 W = audio->index_write;
    uint32 S = audio->size;
    uint32 L = (audio->index_read + latency_bytes) % S;

    uint32 write_until_byte = W;

    if (R < L)
    {
        // |-------R-------L-------|S
        if (W < R)
        {
            // |---W---R-------L-------|S
            // We're gonna wait
            if (S - L > 2*latency_bytes)
            {
                W = L;
                write_until_byte = (L + latency_bytes) % S;
            }
        }
        else if (W < L)
        {
            // |-------R---W---L-------|S
            write_until_byte = (L + latency_bytes) % S;
        }
        else
        {
            // |-------R-------L---W---|S
            // We're gonna wait
        }
    }
    else
    {
        // |-------L-------R-------|S
        if (W < L)
        {
            // |---W---L-------R-------|S
            write_until_byte = (L + latency_bytes) % S;
        }
        else if (W < R)
        {
            // |-L----W--------------R-|S
            // We're gonna wait
        }
        else
        {
            // |-------L-------R---W---|S
            write_until_byte = (L + latency_bytes) % S;
        }
    }

    // Mix sine audio generated on the fly
    {
        uint32 chunk1_size = 0;
        uint32 chunk2_size = 0;

        if (write_until_byte < W)
        {
            chunk1_size = S - W;
            chunk2_size = write_until_byte;
        }
        else
        {
            chunk1_size = write_until_byte - W;
        }

        ASSERT(chunk1_size % frame_size == 0);
        ASSERT(chunk2_size % frame_size == 0);
        if (chunk1_size)
        {
            int16 *chunk1_data = (int16 *) (audio->data + audio->index_write);
            int16 *chunk1_buff = ALLOCATE_BUFFER_(engine->temporary, chunk1_size);
            spear_audio_sine_wave_generate(
                &engine->sine_audio_262Hz,
                chunk1_buff,
                chunk1_size,
                samples_per_second,
                channel_count);
            memcpy(chunk1_data, chunk1_buff, chunk1_size);
            spear_audio_sine_wave_generate(
                &engine->sine_audio_500Hz,
                chunk1_buff,
                chunk1_size,
                samples_per_second,
                channel_count);
            int sample_index;
            for (sample_index = 0; sample_index < chunk1_size / sizeof(sound_sample_t); sample_index++)
            {
                chunk1_data[sample_index] = (chunk1_data[sample_index] + chunk1_buff[sample_index]) / 2;
            }
        }
        if (chunk2_size)
        {
            int16 *chunk2_data = (int16 *) audio->data;
            int16 *chunk2_buff = ALLOCATE_BUFFER_(engine->temporary, chunk2_size);
            spear_audio_sine_wave_generate(
                &engine->sine_audio_262Hz,
                chunk2_buff,
                chunk2_size,
                samples_per_second,
                channel_count);
            memcpy(chunk2_data, chunk2_buff, chunk2_size);
            spear_audio_sine_wave_generate(
                &engine->sine_audio_500Hz,
                chunk2_buff,
                chunk2_size,
                samples_per_second,
                channel_count);
            int sample_index;
            for (sample_index = 0; sample_index < chunk2_size / sizeof(sound_sample_t); sample_index++)
            {
                chunk2_data[sample_index] = (chunk2_data[sample_index] + chunk2_buff[sample_index]) / 2;
            }
        }
    }

    audio->index_write = write_until_byte;
}

void spear_engine_init_graphics(engine *engine)
{
    renderer_init_api(&engine->renderer);
}

void spear_engine_create_meshes(engine *engine)
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

    // char const *utah_filename = "../data/utah_teapot.obj";
    // usize utah_size = platform_get_file_size(utah_filename);
    // printf("Utah size = %llu\n", utah_size);
    // void *utah_data = ALLOCATE_BUFFER_(engine->temporary, utah_size);
    // uint32 bytes_read = platform_read_file_into_memory(utah_filename, utah_data, utah_size);
    // ASSERT(bytes_read == utah_size);
    // wavefront_obj utah_teapot = wavefront_obj_parse(engine->temporary, utah_data, utah_size);
    // UNUSED(utah_teapot);
    // printf("Utah data = %p\n", utah_data);

    // Load texture file
    {
        char const *file_name = "../misc/test8x8.bmp";
        usize file_size = platform_get_file_size(file_name);
        void *file_data = ALLOCATE_BUFFER_(engine->temporary, file_size);
        uint32 bytes_read = platform_read_file_into_memory(file_name, file_data, file_size);
        ASSERT(bytes_read == file_size);

        void *image_data = NULL;
        usize image_size = 0;
        usize decode_result = bmp_extract_size(file_data, file_size, (uint32 *) &image_size);
        if (decode_result == BmpDecode_Success && image_size > 0)
        {
            image_data = ALLOCATE_BUFFER_(engine->allocator, image_size);
            if (image_data)
            {
                uint32 width, height, bits_per_pixel, color_mode, is_top_down;
                decode_result = bmp_decode(file_data, file_size, image_data, image_size,
                    &width, &height, &bits_per_pixel, &color_mode, &is_top_down);

                if (decode_result == BmpDecode_Success)
                {
                    printf("Loaded bmp file: image_size = %llu; width = %u; height = %u;\n", image_size, width, height);
                    engine->test_bmp = (bitmap)
                    {
                        .data = image_data,
                        .size = image_size,
                        .width = width,
                        .height = height,
                        .bits_per_pixel = bits_per_pixel,
                        .color_mode = color_mode,
                    };

                    engine->test_tx = load_texture(engine->test_bmp);
                    printf("Loaded texture (id=%d)\n", engine->test_tx.id);
                }
            }
        }
    }
    // Load sound file
    {
        char const *file_name = "../data/birds.wav";
        usize file_size = platform_get_file_size(file_name);
        if (file_size > 0)
        {
            void *file_data = ALLOCATE_BUFFER_(engine->temporary, file_size);
            uint32 bytes_read = platform_read_file_into_memory(file_name, file_data, file_size);
            ASSERT(bytes_read == file_size);

            void *sound_data = NULL;
            usize sound_size = 0;
            usize decode_result = wav_extract_size(file_data, file_size, &sound_size);
            if (decode_result == WavDecode_Success && sound_size > 0)
            {
                sound_data = ALLOCATE_BUFFER_(engine->allocator, sound_size);
                if (sound_data)
                {
                    uint32 channel_count, samples_per_second, bits_per_sample;
                    decode_result = wav_decode(file_data, file_size, sound_data, sound_size,
                        &channel_count, &samples_per_second, &bits_per_sample);

                    if (decode_result == WavDecode_Success)
                    {
                        printf("Loaded wav file:\n");
                        printf("    sound_size = %llu\n", sound_size);
                        printf("    channel_count = %u\n", channel_count);
                        printf("    samples_per_second = %u\n", samples_per_second);
                        printf("    bits_per_sample = %u\n", bits_per_sample);

                        engine->bird_audio.data = sound_data;
                        engine->bird_audio.size = sound_size;
                        engine->bird_audio.index_read = 0;
                        engine->bird_audio.index_write = 0;
                    }
                    else
                    {
                        printf("WAV decode failed: %s\n", wav_decode_result_to_cstring(decode_result));
                    }
                }
            }
            else
            {
                printf("WAV decode failed: %s\n", wav_decode_result_to_cstring(decode_result));
            }
        }
        else
        {
            printf("File '%s' does not exist\n", file_name);
        }
    }
}

void spear_engine_compile_shaders(engine *engine)
{
    engine->shader_single_color = render_compile_shaders(vs_single_color, fs_pass_color);
    engine->shader_textured = render_compile_shaders(vs_textured, fs_textured);
    engine->shader_ground = render_compile_shaders(vs_ground, fs_pass_color);
    engine->shader_framebuffer = render_compile_shaders(vs_framebuffer, fs_framebuffer);
    engine->shader_text = render_compile_shaders(vs_text, fs_text);
    engine->shader_phong = render_compile_shaders(vs_phong, fs_phong);
    engine->shader_sun = render_compile_shaders(vs_sun, fs_sun);
    engine->shader_ui_frame = render_compile_shaders(vs_frame, fs_pass_color);
}

void spear_engine_load_game_dll(engine *engine, char const *filename)
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
            matrix4_mul(
                matrix4_translate(-1, 1, 0),
                matrix4_scale(2.f / viewport.width, -2.f/viewport.height, 1));
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

    spear_engine_audio_mix(engine);
}

static void spear_engine_draw_mesh_internal(engine *engine, render_command cmd)
{
    matrix4 model =
        matrix4_mul(
            matrix4_translate(cmd.mesh_position.x, cmd.mesh_position.y, cmd.mesh_position.z),
            matrix4_scale(cmd.mesh_scale.x, cmd.mesh_scale.y, cmd.mesh_scale.z)
        );
    gpu_mesh m = cmd.mesh_tag == RenderCommand_DrawMesh_Square ? engine->mesh_square :
                 cmd.mesh_tag == RenderCommand_DrawMesh_Cube ? engine->mesh_cube :
                 engine->mesh_cube;
    gpu_shader s = cmd.mesh_shader_tag == RenderCommand_DrawShader_SingleColor ? engine->shader_single_color :
                   cmd.mesh_shader_tag == RenderCommand_DrawShader_Ground ? engine->shader_ground :
                   cmd.mesh_shader_tag == RenderCommand_DrawShader_Phong ? engine->shader_phong :
                   engine->shader_single_color;

    renderer_draw_mesh(&engine->renderer, model, m, s, cmd.mesh_color);
}

static void spear_engine_draw_ui(engine *engine, render_command cmd)
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

void spear_engine_game_render(engine *engine)
{
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
                // glDisable(GL_DEPTH_TEST);
                // renderer_draw_mesh_ui(&engine->renderer,
                //     matrix4_scale(10000.f, 10000.f, 1.f),
                //     engine->mesh_square,
                //     engine->shader_single_color,
                //     v4f(0.1f, 0.1f, 0.1f, 1.f));
                // glEnable(GL_DEPTH_TEST);
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

#if 0
    // Draw texture debug
    {
        matrix4 model = matrix4_translate(0.f, 0.f, 2.f);
        glUseProgram(engine->shader_textured.id);
        render_shader_uniform_matrix4f(engine->shader_textured, "u_model", (float *) &model);
        render_shader_uniform_matrix4f(engine->shader_textured, "u_view", (float *) &engine->renderer.view_matrix);
        render_shader_uniform_matrix4f(engine->shader_textured, "u_projection", (float *) &engine->renderer.proj_matrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, engine->test_tx.id);

        glBindVertexArray(engine->mesh_square_uv.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->mesh_square_uv.ibo);
        glDrawElements(GL_TRIANGLES, engine->mesh_square_uv.element_count, GL_UNSIGNED_INT, NULL);
    }
#endif
    // Draw audio debug
    {
        uint32 channel_count = 2;
        uint32 latency_frames = engine->audio_latency * 44100;
        uint32 latency_bytes = latency_frames * channel_count * sizeof(sound_sample_t);

        uint32 index_read = engine->master_audio.index_read;
        uint32 index_write = engine->master_audio.index_write;

        float screen_position_read = (float) index_read / engine->master_audio.size * engine->current_client_width;
        float screen_position_write = (float) index_write / engine->master_audio.size * engine->current_client_width;
        float screen_position_latency = (float) (index_read + latency_bytes) / engine->master_audio.size * engine->current_client_width;

        uint32 index = engine->sound_debug_position_running_index++;
        if (engine->sound_debug_position_running_index >= engine->sound_debug_position_count)
            engine->sound_debug_position_running_index = 0;

        engine->sound_debug_positions_read[index] = screen_position_read;
        engine->sound_debug_positions_write[index] = screen_position_write;
        engine->sound_debug_positions_latency[index] = screen_position_latency;

        uint32 i;
        for (i = 0; i < engine->sound_debug_position_count; i++)
        {
            {
                matrix4 model = matrix4_mul(
                    matrix4_translate(
                        engine->sound_debug_positions_read[i],
                        (float) i * engine->viewport.height / engine->sound_debug_position_count,
                        0.f),
                    matrix4_scale(1.f, 50.f, 1.f));
                matrix4 view = matrix4_identity();
                vector4 color = vector4_create(1.f, 0.f, 0.f, 1.f);
                glUseProgram(engine->shader_single_color.id);
                render_shader_uniform_matrix4f(engine->shader_single_color, "u_model", (float *) &model);
                render_shader_uniform_matrix4f(engine->shader_single_color, "u_view", (float *) &view);
                render_shader_uniform_matrix4f(engine->shader_single_color, "u_projection", (float *) &engine->renderer.proj_matrix_ui);
                render_shader_uniform_vector4f(engine->shader_single_color, "u_color", (float *) &color);

                // glActiveTexture(GL_TEXTURE0);
                // glBindTexture(GL_TEXTURE_2D, engine->test_tx.id);

                glBindVertexArray(engine->mesh_square.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->mesh_square.ibo);
                glDrawElements(GL_TRIANGLES, engine->mesh_square.element_count, GL_UNSIGNED_INT, NULL);
            }
            {
                vector4 color = vector4_create(0.f, 1.f, 0.f, 1.f);
                float W0 = engine->sound_debug_positions_write[i];
                float W1 = engine->sound_debug_positions_write[(i + 1) % engine->sound_debug_position_count];

                float p = 0.5f * (W0 + W1);
                float len = fabs(engine->sound_debug_positions_write[(i + 1) % engine->sound_debug_position_count] - engine->sound_debug_positions_write[i]);

                matrix4 model = matrix4_mul(
                    matrix4_translate(
                        p,
                        (float) i * engine->viewport.height / engine->sound_debug_position_count,
                        0.f),
                    matrix4_scale(len*0.5f, 1.f, 1.f));
                matrix4 view = matrix4_identity();
                glUseProgram(engine->shader_single_color.id);
                render_shader_uniform_matrix4f(engine->shader_single_color, "u_model", (float *) &model);
                render_shader_uniform_matrix4f(engine->shader_single_color, "u_view", (float *) &view);
                render_shader_uniform_matrix4f(engine->shader_single_color, "u_projection", (float *) &engine->renderer.proj_matrix_ui);
                render_shader_uniform_vector4f(engine->shader_single_color, "u_color", (float *) &color);

                // glActiveTexture(GL_TEXTURE0);
                // glBindTexture(GL_TEXTURE_2D, engine->test_tx.id);

                glBindVertexArray(engine->mesh_square.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->mesh_square.ibo);
                glDrawElements(GL_TRIANGLES, engine->mesh_square.element_count, GL_UNSIGNED_INT, NULL);
            }
            {
                float p = 0.5f * (engine->sound_debug_positions_read[i] + engine->sound_debug_positions_latency[i]);
                float len = fabs(engine->sound_debug_positions_read[i] - engine->sound_debug_positions_latency[i]);
                matrix4 model = matrix4_mul(
                    matrix4_translate(
                        p,
                        (float) i * engine->viewport.height / engine->sound_debug_position_count,
                        0.f),
                    matrix4_scale(0.5f * len, 1.f, 1.f));
                matrix4 view = matrix4_identity();
                vector4 color = vector4_create(1.f, 1.f, 0.f, 1.f);
                glUseProgram(engine->shader_single_color.id);
                render_shader_uniform_matrix4f(engine->shader_single_color, "u_model", (float *) &model);
                render_shader_uniform_matrix4f(engine->shader_single_color, "u_view", (float *) &view);
                render_shader_uniform_matrix4f(engine->shader_single_color, "u_projection", (float *) &engine->renderer.proj_matrix_ui);
                render_shader_uniform_vector4f(engine->shader_single_color, "u_color", (float *) &color);

                // glActiveTexture(GL_TEXTURE0);
                // glBindTexture(GL_TEXTURE_2D, engine->test_tx.id);

                glBindVertexArray(engine->mesh_square.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->mesh_square.ibo);
                glDrawElements(GL_TRIANGLES, engine->mesh_square.element_count, GL_UNSIGNED_INT, NULL);
            }
        }
    }
}
