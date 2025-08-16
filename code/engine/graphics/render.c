#include "render.h"


// static matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f)
// {
//     matrix4 result = {};

//     result._11 = 2.0f * n / w;
//     result._22 = 2.0f * n / h;
//     result._33 = -(f + n) / (f - n);
//     result._34 = -2.0f * f * n / (f - n);
//     result._43 = -1.0f;

//     return result;
// }

static matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f)
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

// static matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f)
// {
//     matrix4 result = {};

//     result._11 = 2.0f / w;
//     result._22 = 2.0f / h;
//     result._33 = -2.0f / (f - n);
//     result._34 = -(f + n) / (f - n);
//     result._44 = 1.0f;

//     return result;
// }

// static matrix4 make_orthographic_matrix_ratio(float32 aspect_ratio, float32 n, float32 f)
// {
//     matrix4 result = {};

//     result._11 = 1.0f;
//     result._22 = 1.0f * aspect_ratio;
//     result._33 = -2.0f / (f - n);
//     result._34 = -(f + n) / (f - n);
//     result._44 = 1.0f;

//     return result;
// }

static matrix4 make_lookat_matrix_from_camera(vector3 p, vector3 forward, vector3 up)
{
    vector3 f = v3f_normalize(forward);
    vector3 s = v3f_normalize(v3f_cross(f, up));
    vector3 u = v3f_cross(s, f);

    matrix4 result = {};
    result._1 = v4f( s.x,  s.y,  s.z, -v3f_dot(s, p));
    result._2 = v4f( u.x,  u.y,  u.z, -v3f_dot(u, p));
    result._3 = v4f(-f.x, -f.y, -f.z,  v3f_dot(f, p));
    result._4 = v4f(   0,    0,    0,          1.f);

    return result;
}

// static matrix4 make_lookat_matrix_at(vector3 eye, vector3 at, vector3 up)
// {
//     vector3 f = v3f_normalize(sub3f(at, eye));
//     vector3 s = v3f_normalize(v3f_cross(f, up));
//     vector3 u = v3f_cross(s, f);

//     matrix4 result = {};
//     result._1 = v4f( s.x,  s.y,  s.z, -v3f_dot(s, eye));
//     result._2 = v4f( u.x,  u.y,  u.z, -v3f_dot(u, eye));
//     result._3 = v4f(-f.x, -f.y, -f.z,  v3f_dot(f, eye));
//     result._4 = v4f(   0,    0,    0,            1.f);

//     return result;
// }

void renderer_setup_projection(renderer *r, float fov, float aspect_ratio, float near, float far)
{
    r->proj_matrix = make_projection_matrix_fov(fov, aspect_ratio, near, far);
}

void renderer_setup_camera(renderer *r, vector3 pos, vector3 dir, vector3 up)
{
    r->view_matrix = make_lookat_matrix_from_camera(pos, dir, up);
}

static void renderer_draw_mesh_internal(renderer *r, matrix4 model, matrix4 view, matrix4 projection, gpu_mesh m, gpu_shader s, vector4 color)
{
    glUseProgram(s.id);

    render_shader_uniform_matrix4f(s, "u_model", (float32 *) &model);
    render_shader_uniform_matrix4f(s, "u_view", (float32 *) &view);
    render_shader_uniform_matrix4f(s, "u_projection", (float32 *) &projection);
    render_shader_uniform_vector4f(s, "u_color", (float32 *) &color);

    glBindVertexArray(m.vao);
    if (m.element_count > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ibo);
        glDrawElements(GL_TRIANGLES, m.element_count, GL_UNSIGNED_INT, NULL);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, m.vertex_count);
    }
}

void renderer_draw_mesh(renderer *r, matrix4 model, gpu_mesh m, gpu_shader s, vector4 color)
{
    matrix4 view = r->view_matrix;
    matrix4 proj = r->proj_matrix;
    renderer_draw_mesh_internal(r, model, view, proj, m, s, color);
}

void renderer_draw_mesh_ui(renderer *r, matrix4 model, gpu_mesh m, gpu_shader s, vector4 color)
{
    matrix4 view = m4f_identity();
    matrix4 proj = r->proj_matrix_ui;
    renderer_draw_mesh_internal(r, model, view, proj, m, s, color);
}

void renderer_draw_ui_frame(renderer *r, matrix4 model, gpu_mesh m, gpu_shader s, vector4 color, float width, float height)
{
    glUseProgram(s.id);

    matrix4 view = m4f_identity();
    matrix4 proj = r->proj_matrix_ui;

    render_shader_uniform_matrix4f(s, "u_model", (float32 *) &model);
    render_shader_uniform_matrix4f(s, "u_view", (float32 *) &view);
    render_shader_uniform_matrix4f(s, "u_projection", (float32 *) &proj);
    render_shader_uniform_vector4f(s, "u_color", (float32 *) &color);
    render_shader_uniform_float(s, "u_width", width);
    render_shader_uniform_float(s, "u_height", height);

    glBindVertexArray(m.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ibo);
    glDrawElements(GL_TRIANGLES, m.element_count, GL_UNSIGNED_INT, NULL);
}

viewport render_viewport_create(int32 width, int32 height, float32 desired_aspect_ratio)
{
    viewport result;

    float32 aspect_ratio = (float32) width / (float32) height;
    if (aspect_ratio < desired_aspect_ratio)
    {
        // Black strips on top and bottom of the screen
        result.width    = width;
        result.height   = (int32) (result.width / desired_aspect_ratio);
        result.offset_x = 0;
        result.offset_y = (height - result.height) / 2;
    }
    else if (aspect_ratio > desired_aspect_ratio)
    {
        // Black strips on left and right of the screen
        result.height   = height;
        result.width    = (int32) (result.height * desired_aspect_ratio);
        result.offset_x = (width - result.width) / 2;
        result.offset_y = 0;
    }
    else
    {
        // No black strips
        result.width    = width;
        result.height   = height;
        result.offset_x = 0;
        result.offset_y = 0;
    }

    return result;
}

void render_vertex_buffer_layout_push(vertex_buffer_layout *layout, uint32 element_size, uint32 element_count)
{
    ASSERT(layout->count < ARRAY_COUNT(layout->element_sizes));
    layout->element_sizes[layout->count] = element_size;
    layout->element_counts[layout->count] = element_count;
    layout->stride += (element_size * element_count);
    layout->count += 1;
}

gpu_mesh render_load_mesh_to_gpu(cpu_mesh mesh)
{
    uint32 vbo_id = 0;
    {
        glGenBuffers(1, &vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, mesh.vbo.size, mesh.vbo.data, GL_STATIC_DRAW);
    }

    uint32 ibo_id = 0;
    if (memory_view_is_valid(mesh.ibo))
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

        uint32 attrib_index;
        usize offset = 0;
        for (attrib_index = 0; attrib_index < mesh.vbl.count; attrib_index++)
        {
            uint32 count = mesh.vbl.element_counts[attrib_index];
            uint32 size = mesh.vbl.element_sizes[attrib_index];
            uint32 stride = mesh.vbl.stride;
            glEnableVertexAttribArray(attrib_index);
            glVertexAttribPointer(
                attrib_index,      // Index
                count,             // Count
                GL_FLOAT,          // Type
                GL_FALSE,          // Normalized?
                stride,            // Stride
                (void *) offset);  // Offset

            offset += (count * size);
        }
    }

    gpu_mesh result;
    result.vbo = vbo_id;
    result.ibo = ibo_id;
    result.vao = vao_id;
    result.vertex_count = mesh.vbo.size / mesh.vbl.stride;
    result.element_count = mesh.ibo.size / sizeof(uint32);

    printf("Mesh loaded: (vao = %d, vbo = %d, ibo = %d, vertex_count = %d, element_count = %d)\n", vao_id, vbo_id, ibo_id, result.vertex_count, result.element_count);
    return result;
}

int32 compile_shader(char const *source_code, int shader_type)
{
    int32 id = glCreateShader(shader_type);
    glShaderSource(id, 1, (char const **) &source_code, (int const *) NULL);
    glCompileShader(id);

    int successful;
    glGetShaderiv(id, GL_COMPILE_STATUS, &successful);
    if (successful == GL_FALSE)
    {
        GLsizei length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, (GLint *) &length);

        memory_allocator mallocator = memory_allocator_malloc_create();
        char *message = ALLOCATE_BUFFER(mallocator, length + 1);

        glGetShaderInfoLog(id, length, (GLsizei *) &length, message);
        glDeleteShader(id);

        printf("Could not compile shader: \"%s\"\n", message);

        DEALLOCATE(mallocator, message);
        return 0;
    }

    return id;
}

gpu_shader render_compile_shaders(char const *vs_code, char const *fs_code)
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

    gpu_shader result;
    result.id = id;
    result.vs_id = vs;
    result.fs_id = fs;

    printf("Shaders compiled (id = %d, vs = %d, fs = %d)\n", id, vs, fs);

    return result;
}

void render_shader_uniform_int(gpu_shader shader, char const *name, int32 n)
{
    int location = glGetUniformLocation(shader.id, name);
    glUniform1i(location, n);
}

void render_shader_uniform_float(gpu_shader shader, char const *name, float f)
{
    int location = glGetUniformLocation(shader.id, name);
    glUniform1f(location, f);
}

void render_shader_uniform_vector2f(gpu_shader shader, char const *name, float *v)
{
    int location = glGetUniformLocation(shader.id, name);
    glUniform2f(location, v[0], v[1]);
}

void render_shader_uniform_vector3f(gpu_shader shader, char const *name, float *v)
{
    int location = glGetUniformLocation(shader.id, name);
    glUniform3f(location, v[0], v[1], v[2]);
}

void render_shader_uniform_vector4f(gpu_shader shader, char const *name, float *v)
{
    int location = glGetUniformLocation(shader.id, name);
    glUniform4f(location, v[0], v[1], v[2], v[3]);
}

void render_shader_uniform_matrix4f(gpu_shader shader, char const *name, float *m)
{
    int location = glGetUniformLocation(shader.id, name);
    glUniformMatrix4fv(location, 1, GL_TRUE, m);
}
