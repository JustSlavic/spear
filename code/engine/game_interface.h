#ifndef _SPEAR_ENGINE_GAME_INTERFACE_H
#define _SPEAR_ENGINE_GAME_INTERFACE_H

#include <corelibs/base.h>
#include <gamelibs/input.h>
#include <gamelibs/camera.h>


typedef enum engine_command_tag
{
    EngineCommand_Invalid = 0,

    EngineCommand_Exit,
} engine_command_tag;

typedef struct engine_command
{
    engine_command_tag tag;
} engine_command;

typedef enum render_command_tag
{
    RenderCommand_Invalid = 0,

    RenderCommand_SetupCamera,
    RenderCommand_DrawMesh,
    RenderCommand_DrawUi,
} render_command_tag;

typedef enum render_command_draw_mesh_tag
{
    RenderCommand_DrawMesh_Invalid = 0,

    RenderCommand_DrawMesh_Square,
    RenderCommand_DrawMesh_Cube,
    RenderCommand_DrawMesh_UiFrame,
} render_command_draw_mesh_tag;

typedef enum render_command_draw_shader_tag
{
    RenderCommand_DrawShader_Invalid = 0,

    RenderCommand_DrawShader_SingleColor,
    RenderCommand_DrawShader_Ground,
    RenderCommand_DrawShader_Phong,
} render_command_draw_shader_tag;

typedef struct render_command
{
    render_command_tag tag;
    union
    {
        struct // RenderCommand_SetupCamera
        {
            vector3 camera_position;
            vector3 camera_forward;
            vector3 camera_up;
        };
        struct // RenderCommand_DrawMesh
        {
            render_command_draw_mesh_tag mesh_tag;
            render_command_draw_shader_tag mesh_shader_tag;
            vector3 mesh_position;
            vector3 mesh_scale;
            vector4 mesh_color;
        };
        struct // RenderCommand_DrawUi
        {
            render_command_draw_mesh_tag ui_mesh_tag;
            render_command_draw_shader_tag ui_shader_tag;

            transform ui_tm;
            float ui_width;
            float ui_height;
            vector4 ui_color;
            // Optional parameters
            float ui_frame_width;
            vector2 ui_offset;
        };
    };
} render_command;

typedef struct context
{
    engine_command engine_commands[16];
    uint32 engine_commands_count;

    render_command render_commands[256];
    uint32 render_commands_count;

    float32 near_clip_distance;
    float32 near_clip_width;
    float32 near_clip_height;

    int32 viewport_offset_x;
    int32 viewport_offset_y;
    int32 viewport_width;
    int32 viewport_height;

    void *game_state;
} context;

void context_engine_command_push(context *ctx, engine_command cmd);
void context_render_command_push(context *ctx, render_command cmd);

void context_engine_command_push_exit(context *ctx);
void context_render_command_push_camera(context *ctx, vector3 position, vector3 forward, vector3 up);
void context_render_command_push_square(context *ctx, render_command_draw_shader_tag shader_tag, vector3 position, quaternion orientation, float scale_x, float scale_y);
void context_render_command_push_cube(context *ctx, render_command_draw_shader_tag shader_tag, vector3 position, vector3 scale, vector4 color);
void context_render_command_push_ui(context *ctx, transform tm, float width, float height, vector4 color, float frame_width, vector4 frame_color);

#define INITIALIZE_MEMORY_FUNCTION_T(NAME) void NAME(context *ctx, memory_view game_memory)
typedef INITIALIZE_MEMORY_FUNCTION_T(initialize_memory_t);
#define INITIALIZE_MEMORY_FUNCTION(...) INITIALIZE_MEMORY_FUNCTION_T(initialize_memory)

#define UPDATE_AND_RENDER_FUNCTION_T(NAME) void NAME(context *ctx, memory_view game_memory, spear_input *input)
typedef UPDATE_AND_RENDER_FUNCTION_T(update_and_render_t);
#define UPDATE_AND_RENDER_FUNCTION(...) UPDATE_AND_RENDER_FUNCTION_T(update_and_render)


DLL_EXPORT INITIALIZE_MEMORY_FUNCTION();
DLL_EXPORT UPDATE_AND_RENDER_FUNCTION();


#endif // _SPEAR_ENGINE_GAME_INTERFACE_H
