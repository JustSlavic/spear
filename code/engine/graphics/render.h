#ifndef _SPEAR_ENGINE_GRAPHICS_RENDERER_H
#define _SPEAR_ENGINE_GRAPHICS_RENDERER_H

#include <corelibs/base.h>
#include <corelibs/math.h>


typedef struct
{
    int32 offset_x;
    int32 offset_y;
    int32 width;
    int32 height;
} viewport;

typedef struct
{
    uint32 element_sizes[8];
    uint32 element_counts[8];
    uint32 count;
    uint32 stride;
} vertex_buffer_layout;

typedef struct
{
    memory_view vbo;
    memory_view ibo;
    vertex_buffer_layout vbl;
} cpu_mesh;

typedef struct
{
    uint32 vbo;
    uint32 ibo;
    uint32 vao;
    uint32 vertex_count;
    uint32 element_count;
} gpu_mesh;

typedef struct
{
    uint32 id;
    uint32 vs_id;
    uint32 fs_id;
} gpu_shader;

typedef struct
{
    uint32 id;
} gpu_texture;

typedef struct renderer
{
    matrix4 view_matrix;
    matrix4 proj_matrix;
    matrix4 proj_matrix_ui;
} renderer;

void renderer_init_api(renderer *r);
void renderer_setup_projection(renderer *r, float fov, float aspect_ratio, float near, float far);
void renderer_setup_camera(renderer *r, vector3 pos, vector3 dir, vector3 up);
void renderer_draw_mesh(renderer *r, matrix4 model, gpu_mesh m, gpu_shader s, vector4 color);
void renderer_draw_mesh_ui(renderer *r, matrix4 model, gpu_mesh m, gpu_shader s, vector4 color);
void renderer_draw_mesh_textured(renderer *r, matrix4 model, gpu_mesh m, gpu_shader s, gpu_texture t);
void renderer_draw_ui_frame(renderer *r, matrix4 model, gpu_mesh m, gpu_shader s, vector4 color, float width, float height);

viewport render_viewport_create(int32 width, int32 height, float32 desired_aspect_ratio);
void render_vertex_buffer_layout_push(vertex_buffer_layout *layout, uint32 element_size, uint32 element_count);
gpu_mesh render_load_mesh_to_gpu(cpu_mesh mesh);
gpu_shader render_compile_shaders(char const *vs_code, char const *fs_code);
gpu_texture load_texture(bitmap bitmap);

void render_shader_uniform_float(gpu_shader shader, char const *name, float);
void render_shader_uniform_vector4f(gpu_shader shader, char const *name, float *);
void render_shader_uniform_matrix4f(gpu_shader shader, char const *name, float *);


#endif // _SPEAR_ENGINE_GRAPHICS_RENDERER_H
