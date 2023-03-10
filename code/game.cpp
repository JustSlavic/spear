#include <game.hpp>
#include <game_state.hpp>

#include <math/rectangle2.hpp>
#include <math/vector4.hpp>
#include <math/matrix4.hpp>


//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
//
INITIALIZE_MEMORY_FUNCTION(initialize_memory)
{
    using namespace math;

    ASSERT(sizeof(game_state) < game_memory.size);
    game_state *gs = (game_state *) game_memory.memory;

    // @note: let zero-indexed entity be 'null entity' representing lack of entity
    gs->entity_count = 1;

    gs->camera_position = V3(0, 0, -10);

    float32 vbo_init[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
    };

    uint32 ibo_init[] = {
        0, 1, 2, // first triangle
        2, 3, 0, // second triangle
    };

    gfx::vertex_buffer_layout vbl = {};
    gfx::push_layout_element(&vbl, 3);

    auto vbo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(vbo_init));
    memory::copy(vbo.memory, vbo_init, sizeof(vbo_init));

    auto ibo = ALLOCATE_BLOCK_(&context->temporary_allocator, sizeof(ibo_init));
    memory::copy(ibo.memory, ibo_init, sizeof(ibo_init));

    gs->rectangle_mesh = create_mesh_resource(&context->resource_storage, vbo, ibo, vbl);
    gs->rectangle_shader = create_shader_resource(&context->resource_storage, STRID("rectangle.shader"));

    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            // comment to find
            auto *entity = push_entity(gs);
            entity->position = V2(x, y);
            entity->velocity = V2(-0.01 * x, 0.01 * x - 0.01 * y * y);
            entity->width = 0.1f;
            entity->height = 0.05f;
            entity->rotation = 0.0f;
            entity->rotational_velocity = 0.01f * x * y - 0.025f;
        }
    }
}

//
// Arguments:
// - execution_context *context;
// - memory_block game_memory;
// - input_devices input;
// - float32 dt;
//
UPDATE_AND_RENDER_FUNCTION(update_and_render)
{
    using namespace math;

    game_state *gs = (game_state *) game_memory.memory;

    if (get_press_count(input->keyboard_device[keyboard::esc]))
    {
        push_execution_command(context, exit_command());
    }

    vector3 camera_velocity = vector3::zero();
    if (get_hold_count(input->keyboard_device[keyboard::w]))
    {
        camera_velocity.y += 1;
    }
    if (get_hold_count(input->keyboard_device[keyboard::a]))
    {
        camera_velocity.x += 1;
    }
    if (get_hold_count(input->keyboard_device[keyboard::s]))
    {
        camera_velocity.y -= 1;
    }
    if (get_hold_count(input->keyboard_device[keyboard::d]))
    {
        camera_velocity.x -= 1;
    }

    float32 camera_speed = 0.1f;
    gs->camera_position += camera_velocity * camera_speed * dt;

    // Setup camera
    {
        gfx::render_command::command_setup_camera setup_camera;
        setup_camera.camera_position = gs->camera_position;
        setup_camera.look_at_position = V3(gs->camera_position.x, gs->camera_position.y, 0);
        setup_camera.camera_up_direction = V3(0, 1, 0);

        push_setup_camera_command(context, setup_camera);
    }

    // Background
    {
        gfx::render_command::command_draw_background draw_background;
        draw_background.mesh = gs->rectangle_mesh;
        draw_background.shader = gs->rectangle_shader;
        draw_background.color = math::make_vector4(1.0, 1.0, 1.0, 1.0);
        push_draw_background_command(context, draw_background);
    }

#if 0
    auto gravity = V2(0, -9.8); // m/s^2
#else
    auto gravity = V2(0, 0); // m/s^2
#endif

    for (int entity_index = 1; entity_index < gs->entity_count; entity_index++)
    {
        auto *entity = gs->entities + entity_index;

        entity->rotation = entity->rotation + entity->rotational_velocity * dt;
        entity->velocity = entity->velocity + gravity * dt;
        entity->position = entity->position + entity->velocity * dt;

        gfx::render_command::command_draw_mesh_with_color draw_mesh;
        draw_mesh.mesh_token = gs->rectangle_mesh;
        draw_mesh.shader_token = gs->rectangle_shader;
        draw_mesh.model =
            math::translated(V3(entity->position.x, entity->position.y, 0),
            math::rotated_z(entity->rotation,
            math::scaled(V3(entity->width, entity->height, 1),
                math::matrix4::identity())));
        draw_mesh.color = V4(0.1f, 0.32f, 0.72f, 1.0f);

        push_draw_mesh_with_color_command(context, draw_mesh);
    }
}


#include <memory/allocator.cpp>
#include <string_id.cpp>
