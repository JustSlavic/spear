#include "primitive_meshes.h"


cpu_mesh mesh_square_create(void)
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

    memory_view vbo = memory_view_create(vbo_data, sizeof(vbo_data));
    memory_view ibo = memory_view_create(ibo_data, sizeof(ibo_data));
    vertex_buffer_layout vbl = {};
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);

    cpu_mesh result = {};
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}

cpu_mesh mesh_square_create_uv(void)
{
    static float32 vbo_data[] = {
        -1.0f, -1.0f,  0.0f,    0.0f,  0.0f,
         1.0f, -1.0f,  0.0f,    1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,    1.0f,  1.0f,
        -1.0f,  1.0f,  0.0f,    0.0f,  1.0f,
    };

    static uint32 ibo_data[] = {
        0, 1, 2, // first triangle
        2, 3, 0, // second triangle
    };

    memory_view vbo = memory_view_create(vbo_data, sizeof(vbo_data));
    memory_view ibo = memory_view_create(ibo_data, sizeof(ibo_data));
    vertex_buffer_layout vbl = {};
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 2);

    cpu_mesh result = {};
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}

cpu_mesh mesh_tetrahedron_create(void)
{
    float32 a = 1.0f / 3.0f;
    float32 b = sqrtf(8.0f / 9.0f);
    float32 c = sqrtf(2.0f / 9.0f);
    float32 d = sqrtf(2.0f / 3.0f);

    vector3 n0 = vector3_normalize(vector3_cross(vector3_create(  c, -d, 1+a), vector3_create(  c,  d, 1+a)));
    vector3 n1 = vector3_normalize(vector3_cross(vector3_create(  c,  d, 1+a), vector3_create( -b,  0, 1+a)));
    vector3 n2 = vector3_normalize(vector3_cross(vector3_create( -b,  0, 1+a), vector3_create(  c, -d, 1+a)));
    vector3 n3 = vector3_normalize(vector3_cross(vector3_create(b+c, -d,   0), vector3_create(b+c, -d,   0)));

    static float32 vbo_data[72];
    int i = 0;
#define V(value) vbo_data[i++] = (value);
    V( 0) V( 0) V( 1)  V(n0.x) V(n0.y) V(n0.z)
    V(-c) V( d) V(-a)  V(n0.x) V(n0.y) V(n0.z)
    V(-c) V(-d) V(-a)  V(n0.x) V(n0.y) V(n0.z)

    V( 0) V( 0) V( 1)  V(n1.x) V(n1.y) V(n1.z)
    V(-c) V(-d) V(-a)  V(n1.x) V(n1.y) V(n1.z)
    V( b) V( 0) V(-a)  V(n1.x) V(n1.y) V(n1.z)

    V( 0) V( 0) V( 1)  V(n2.x) V(n2.y) V(n2.z)
    V( b) V( 0) V(-a)  V(n2.x) V(n2.y) V(n2.z)
    V(-c) V( d) V(-a)  V(n2.x) V(n2.y) V(n2.z)

    V( b) V( 0) V(-a)  V(n3.x) V(n3.y) V(n3.z)
    V(-c) V(-d) V(-a)  V(n3.x) V(n3.y) V(n3.z)
    V(-c) V( d) V(-a)  V(n3.x) V(n3.y) V(n3.z)
#undef V

    memory_view vbo = memory_view_create(vbo_data, sizeof(vbo_data));
    vertex_buffer_layout vbl = {};
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);

    cpu_mesh result = {};
    result.vbo = vbo;
    result.vbl = vbl;

    return result;
}

cpu_mesh mesh_cube_create(void)
{
    float32 a = 1.f;
    static float32 vbo_data[216];
    int i = 0;
#define V(value) vbo_data[i++] = (value);
    V(-a) V( a) V(-a)  V( 0) V( 0) V(-1)
    V(-a) V(-a) V(-a)  V( 0) V( 0) V(-1)
    V( a) V(-a) V(-a)  V( 0) V( 0) V(-1)

    V( a) V(-a) V(-a)  V( 0) V( 0) V(-1)
    V( a) V( a) V(-a)  V( 0) V( 0) V(-1)
    V(-a) V( a) V(-a)  V( 0) V( 0) V(-1)

    V(-a) V(-a) V( a)  V( 0) V(-1) V( 0)
    V(-a) V(-a) V(-a)  V( 0) V(-1) V( 0)
    V( a) V(-a) V(-a)  V( 0) V(-1) V( 0)

    V( a) V(-a) V(-a)  V( 0) V(-1) V( 0)
    V( a) V(-a) V( a)  V( 0) V(-1) V( 0)
    V(-a) V(-a) V( a)  V( 0) V(-1) V( 0)

    V(-a) V(-a) V( a)  V(-1) V( 0) V( 0)
    V(-a) V(-a) V(-a)  V(-1) V( 0) V( 0)
    V(-a) V( a) V(-a)  V(-1) V( 0) V( 0)

    V(-a) V( a) V(-a)  V(-1) V( 0) V( 0)
    V(-a) V( a) V( a)  V(-1) V( 0) V( 0)
    V(-a) V(-a) V( a)  V(-1) V( 0) V( 0)

    V(-a) V( a) V( a)  V( 0) V( 0) V( 1)
    V(-a) V(-a) V( a)  V( 0) V( 0) V( 1)
    V( a) V(-a) V( a)  V( 0) V( 0) V( 1)

    V( a) V(-a) V( a)  V( 0) V( 0) V( 1)
    V( a) V( a) V( a)  V( 0) V( 0) V( 1)
    V(-a) V( a) V( a)  V( 0) V( 0) V( 1)

    V(-a) V( a) V( a)  V( 0) V( 1) V( 0)
    V(-a) V( a) V(-a)  V( 0) V( 1) V( 0)
    V( a) V( a) V(-a)  V( 0) V( 1) V( 0)

    V( a) V( a) V(-a)  V( 0) V( 1) V( 0)
    V( a) V( a) V( a)  V( 0) V( 1) V( 0)
    V(-a) V( a) V( a)  V( 0) V( 1) V( 0)

    V( a) V(-a) V( a)  V( 1) V( 0) V( 0)
    V( a) V(-a) V(-a)  V( 1) V( 0) V( 0)
    V( a) V( a) V(-a)  V( 1) V( 0) V( 0)

    V( a) V( a) V(-a)  V( 1) V( 0) V( 0)
    V( a) V( a) V( a)  V( 1) V( 0) V( 0)
    V( a) V(-a) V( a)  V( 1) V( 0) V( 0)
#undef V

    memory_view vbo = memory_view_create(vbo_data, sizeof(vbo_data));
    vertex_buffer_layout vbl = {};
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);

    cpu_mesh result = {};
    result.vbo = vbo;
    result.vbl = vbl;

    return result;
}

cpu_mesh mesh_octahedron_create(void)
{
    vector3 n0 = vector3_normalize(vector3_cross(vector3_create( 1, -1,  0), vector3_create( 1,  0, -1)));
    vector3 n1 = vector3_normalize(vector3_cross(vector3_create(-1,  0, -1), vector3_create(-1, -1,  0)));
    vector3 n2 = vector3_normalize(vector3_cross(vector3_create(-1,  1,  0), vector3_create(-1,  0, -1)));
    vector3 n3 = vector3_normalize(vector3_cross(vector3_create( 1,  0, -1), vector3_create( 1,  1,  0)));

    vector3 n4 = vector3_normalize(vector3_cross(vector3_create( 1,  0,  1), vector3_create( 1, -1,  0)));
    vector3 n5 = vector3_normalize(vector3_cross(vector3_create(-1, -1,  0), vector3_create(-1,  0,  1)));
    vector3 n6 = vector3_normalize(vector3_cross(vector3_create(-1,  0,  1), vector3_create(-1,  1,  0)));
    vector3 n7 = vector3_normalize(vector3_cross(vector3_create( 1,  1,  0), vector3_create( 1,  0,  1)));

    static float32 vbo_data[144];
    int i = 0;
#define V(value) vbo_data[i++] = (value);
        V( 1.f) V( 0.f) V( 0.f)  V(n0.x) V(n0.y) V(n0.z)
        V( 0.f) V( 1.f) V( 0.f)  V(n0.x) V(n0.y) V(n0.z)
        V( 0.f) V( 0.f) V( 1.f)  V(n0.x) V(n0.y) V(n0.z)

        V(-1.f) V( 0.f) V( 0.f)  V(n1.x) V(n1.y) V(n1.z)
        V( 0.f) V( 0.f) V( 1.f)  V(n1.x) V(n1.y) V(n1.z)
        V( 0.f) V( 1.f) V( 0.f)  V(n1.x) V(n1.y) V(n1.z)

        V(-1.f) V( 0.f) V( 0.f)  V(n2.x) V(n2.y) V(n2.z)
        V( 0.f) V(-1.f) V( 0.f)  V(n2.x) V(n2.y) V(n2.z)
        V( 0.f) V( 0.f) V( 1.f)  V(n2.x) V(n2.y) V(n2.z)

        V( 1.f) V( 0.f) V( 0.f)  V(n3.x) V(n3.y) V(n3.z)
        V( 0.f) V( 0.f) V( 1.f)  V(n3.x) V(n3.y) V(n3.z)
        V( 0.f) V(-1.f) V( 0.f)  V(n3.x) V(n3.y) V(n3.z)

        V( 1.f) V( 0.f) V( 0.f)  V(n4.x) V(n4.y) V(n4.z)
        V( 0.f) V( 0.f) V(-1.f)  V(n4.x) V(n4.y) V(n4.z)
        V( 0.f) V( 1.f) V( 0.f)  V(n4.x) V(n4.y) V(n4.z)

        V(-1.f) V( 0.f) V( 0.f)  V(n5.x) V(n5.y) V(n5.z)
        V( 0.f) V( 1.f) V( 0.f)  V(n5.x) V(n5.y) V(n5.z)
        V( 0.f) V( 0.f) V(-1.f)  V(n5.x) V(n5.y) V(n5.z)

        V(-1.f) V( 0.f) V( 0.f)  V(n6.x) V(n6.y) V(n6.z)
        V( 0.f) V( 0.f) V(-1.f)  V(n6.x) V(n6.y) V(n6.z)
        V( 0.f) V(-1.f) V( 0.f)  V(n6.x) V(n6.y) V(n6.z)

        V( 1.f) V( 0.f) V( 0.f)  V(n7.x) V(n7.y) V(n7.z)
        V( 0.f) V(-1.f) V( 0.f)  V(n7.x) V(n7.y) V(n7.z)
        V( 0.f) V( 0.f) V(-1.f)  V(n7.x) V(n7.y) V(n7.z)
#undef V

    memory_view vbo = memory_view_create(vbo_data, sizeof(vbo_data));
    vertex_buffer_layout vbl = {};
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);

    cpu_mesh result = {};
    result.vbo = vbo;
    result.vbl = vbl;

    return result;
}

cpu_mesh mesh_icosahedron_create(void)
{
    float phi = (1.0f + sqrtf(5.0f)) * 0.5f;
    float a = 1.0f;
    float b = 1.0f / phi;

    static float32 vbo_data[360];
    int i = 0;
#define V(value) vbo_data[i++] = (value);
    V(-b) V( a) V( 0)  V(0) V(0) V(0)
    V( b) V( a) V( 0)  V(0) V(0) V(0)
    V( 0) V( b) V(-a)  V(0) V(0) V(0)

    V( b) V( a) V( 0)  V(0) V(0) V(0)
    V(-b) V( a) V( 0)  V(0) V(0) V(0)
    V( 0) V( b) V( a)  V(0) V(0) V(0)

    V(-a) V( 0) V( b)  V(0) V(0) V(0)
    V( 0) V(-b) V( a)  V(0) V(0) V(0)
    V( 0) V( b) V( a)  V(0) V(0) V(0)

    V( 0) V(-b) V( a)  V(0) V(0) V(0)
    V( a) V( 0) V( b)  V(0) V(0) V(0)
    V( 0) V( b) V( a)  V(0) V(0) V(0)

    V( a) V( 0) V(-b)  V(0) V(0) V(0)
    V( 0) V(-b) V(-a)  V(0) V(0) V(0)
    V( 0) V( b) V(-a)  V(0) V(0) V(0)

    V( 0) V(-b) V(-a)  V(0) V(0) V(0)
    V(-a) V( 0) V(-b)  V(0) V(0) V(0)
    V( 0) V( b) V(-a)  V(0) V(0) V(0)

    V(-b) V(-a) V( 0)  V(0) V(0) V(0)
    V( b) V(-a) V( 0)  V(0) V(0) V(0)
    V( 0) V(-b) V( a)  V(0) V(0) V(0)

    V( b) V(-a) V( 0)  V(0) V(0) V(0)
    V(-b) V(-a) V( 0)  V(0) V(0) V(0)
    V( 0) V(-b) V(-a)  V(0) V(0) V(0)

    V(-a) V( 0) V(-b)  V(0) V(0) V(0)
    V(-a) V( 0) V( b)  V(0) V(0) V(0)
    V(-b) V( a) V( 0)  V(0) V(0) V(0)

    V(-a) V( 0) V( b)  V(0) V(0) V(0)
    V(-a) V( 0) V(-b)  V(0) V(0) V(0)
    V(-b) V(-a) V( 0)  V(0) V(0) V(0)

    V( a) V( 0) V( b)  V(0) V(0) V(0)
    V( a) V( 0) V(-b)  V(0) V(0) V(0)
    V( b) V( a) V( 0)  V(0) V(0) V(0)

    V( a) V( 0) V(-b)  V(0) V(0) V(0)
    V( a) V( 0) V( b)  V(0) V(0) V(0)
    V( b) V(-a) V( 0)  V(0) V(0) V(0)

    V(-b) V( a) V( 0)  V(0) V(0) V(0)
    V(-a) V( 0) V( b)  V(0) V(0) V(0)
    V( 0) V( b) V( a)  V(0) V(0) V(0)

    V( a) V( 0) V( b)  V(0) V(0) V(0)
    V( b) V( a) V( 0)  V(0) V(0) V(0)
    V( 0) V( b) V( a)  V(0) V(0) V(0)

    V(-a) V( 0) V(-b)  V(0) V(0) V(0)
    V(-b) V( a) V( 0)  V(0) V(0) V(0)
    V( 0) V( b) V(-a)  V(0) V(0) V(0)

    V( b) V( a) V( 0)  V(0) V(0) V(0)
    V( a) V( 0) V(-b)  V(0) V(0) V(0)
    V( 0) V( b) V(-a)  V(0) V(0) V(0)

    V(-b) V(-a) V( 0)  V(0) V(0) V(0)
    V(-a) V( 0) V(-b)  V(0) V(0) V(0)
    V( 0) V(-b) V(-a)  V(0) V(0) V(0)

    V( a) V( 0) V(-b)  V(0) V(0) V(0)
    V( b) V(-a) V( 0)  V(0) V(0) V(0)
    V( 0) V(-b) V(-a)  V(0) V(0) V(0)

    V(-a) V( 0) V( b)  V(0) V(0) V(0)
    V(-b) V(-a) V( 0)  V(0) V(0) V(0)
    V( 0) V(-b) V( a)  V(0) V(0) V(0)

    V( b) V(-a) V( 0)  V(0) V(0) V(0)
    V( a) V( 0) V( b)  V(0) V(0) V(0)
    V( 0) V(-b) V( a)  V(0) V(0) V(0)
#undef V

    int vertex_index;
    for (vertex_index = 0; vertex_index < ARRAY_COUNT(vbo_data) / 6; vertex_index += 3)
    {
        int i0 = vertex_index * 6;
        int i1 = (vertex_index + 1) * 6;
        int i2 = (vertex_index + 2) * 6;

        vector3 v0 = vector3_create(vbo_data[i0], vbo_data[i0 + 1], vbo_data[i0 + 2]);
        vector3 v1 = vector3_create(vbo_data[i1], vbo_data[i1 + 1], vbo_data[i1 + 2]);
        vector3 v2 = vector3_create(vbo_data[i2], vbo_data[i2 + 1], vbo_data[i2 + 2]);
        vector3 normal = vector3_normalize(vector3_cross(vector3_sub(v0, v1), vector3_sub(v0, v2)));

        float norm_v0 = vector3_norm(v0);
        vbo_data[i0    ] = vbo_data[i0    ] / norm_v0;
        vbo_data[i0 + 1] = vbo_data[i0 + 1] / norm_v0;
        vbo_data[i0 + 2] = vbo_data[i0 + 2] / norm_v0;
        vbo_data[i0 + 3] = normal.x;
        vbo_data[i0 + 4] = normal.y;
        vbo_data[i0 + 5] = normal.z;

        float norm_v1 = vector3_norm(v1);
        vbo_data[i1    ] = vbo_data[i1    ] / norm_v1;
        vbo_data[i1 + 1] = vbo_data[i1 + 1] / norm_v1;
        vbo_data[i1 + 2] = vbo_data[i1 + 2] / norm_v1;
        vbo_data[i1 + 3] = normal.x;
        vbo_data[i1 + 4] = normal.y;
        vbo_data[i1 + 5] = normal.z;

        float norm_v2 = vector3_norm(v2);
        vbo_data[i2    ] = vbo_data[i2    ] / norm_v2;
        vbo_data[i2 + 1] = vbo_data[i2 + 1] / norm_v2;
        vbo_data[i2 + 2] = vbo_data[i2 + 2] / norm_v2;
        vbo_data[i2 + 3] = normal.x;
        vbo_data[i2 + 4] = normal.y;
        vbo_data[i2 + 5] = normal.z;
    }

    memory_view vbo = memory_view_create(vbo_data, sizeof(vbo_data));
    vertex_buffer_layout vbl = {};
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);

    cpu_mesh result = {};
    result.vbo = vbo;
    result.vbl = vbl;

    return result;
}

cpu_mesh mesh_sphere_create(int n, int m)
{
    ASSERT(n > 2);
    ASSERT(m > 2);

    int i, j, vbo_i = 0, ibo_i = 0;
    int total_vertex_count = n * m + 2;
    int total_index_count = 3 * m + 6 * (n-1) * (m+1) + 3 * m;

    memory_allocator allocator = memory_allocator_malloc_create();
    float32 *vbo_data = ALLOCATE_ARRAY_(allocator, float, total_vertex_count * 6);
    uint32 *ibo_data = ALLOCATE_ARRAY_(allocator, uint32, total_index_count);

#define V(value) vbo_data[vbo_i++] = value;
#define I(value) ibo_data[ibo_i++] = value;

    V(0.0f) V(0.0f) V(1.0f)  V(0.0f) V(0.0f) V(1.0f) // top vertex

    for (i = 0; i < n; i++)
    {
        float32 a = PI * (i + 1) / (n + 1);
        for (j = 0; j < m; j++)
        {
            float32 b = 2.f * PI * j / m;

            float32 x = sinf(a) * cosf(b);
            float32 y = sinf(a) * sinf(b);
            float32 z = cosf(a);

            // position     normal
            V(x) V(y) V(z)  V(x) V(y) V(z)
        }
    }

    V(0.0f) V(0.0f) V(-1.0f)  V(0.0f) V(0.0f) V(-1.0f) // bottom vertex

    for (j = 0; j < m; j++)
    {
        I(0) I(j + 1) I((j + 1) % m + 1)
    }

    for (i = 1; i < n; i++)
    {
        for (j = 0; j < m + 1; j++)
        {
            // top row points: t0, t1, t2, ...
            // bottom row points: b0, b1, b2, ...

            // 2 triangles per quad
            // t0, b0, b1
            // b1, t1, t0

            I((i - 1) * m + j % m + 1) // t0
            I(i * m + j % m + 1)       // b0
            I(i * m + (j + 1) % m + 1) // b1

            I(i * m + (j + 1) % m + 1)       // b1
            I((i - 1) * m + (j + 1) % m + 1) // t1
            I((i - 1) * m + j % m + 1)       // t0
        }
    }

    for (j = 0; j < m; j++)
    {
        I(n * m + 1)
        I((n - 1) * m + j + 1)
        I((n - 1) * m + (j + 1) % m + 1)
    }
#undef V
#undef I

    ASSERT(vbo_i == total_vertex_count * 6);
    ASSERT(ibo_i == total_index_count);

    memory_view vbo = memory_view_create(vbo_data, sizeof(vbo_data[0]) * vbo_i);
    memory_view ibo = memory_view_create(ibo_data, sizeof(ibo_data[0]) * ibo_i);

    vertex_buffer_layout vbl = {};
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}

typedef struct triangle
{
    uint32 i0, i1, i2;
} triangle;

typedef struct subdivide
{
    vector3 *vertices;
    uint32 vertex_count;

    triangle *triangles;
    uint32 triangle_count;
} subdivide;

subdivide subdivide_triangles(memory_allocator allocator, subdivide input)
{
    uint i, j;
    uint32 edge_count = 3 * input.triangle_count;

    subdivide result = {};
    // Every triangle has 3 edges, but every edge is edge for two triangles,
    // therefore divide by 2.
    result.vertex_count = input.vertex_count + edge_count / 2;
    result.vertices = ALLOCATE_ARRAY(allocator, vector3, result.vertex_count);
    result.triangle_count = 4 * input.triangle_count;
    result.triangles = ALLOCATE_ARRAY(allocator, triangle, result.triangle_count);

    int vertex_i = 0;
    int triangle_i = 0;

    // I will need index map, which will map pairs of indices, to the single index
    // of vertex, which will divide selected edge
    usize index_map_dim = input.vertex_count;
    uint32 *index_map = ALLOCATE_ARRAY(allocator, uint32, index_map_dim * index_map_dim);

    // Copy all current vertices
    for (i = 0; i < input.vertex_count; i++)
    {
        result.vertices[vertex_i++] = input.vertices[i];
    }

    // Push new vertices for all edges and record its indices into index map
    for (i = 0; i < input.triangle_count; i++)
    {
        uint32 iis[4] =
        {
            input.triangles[i].i0,
            input.triangles[i].i1,
            input.triangles[i].i2,
            input.triangles[i].i0,
        };
        for (j = 0; j < 3; j++)
        {
            uint32 i0 = iis[j];
            uint32 i1 = iis[j + 1];
            usize index = index_map[i0 * index_map_dim + i1];
            if (index == 0)
            {
                index = vertex_i;
                vector3 v01 = vector3_scale(0.5f, vector3_add(result.vertices[i0], result.vertices[i1]));
                result.vertices[vertex_i++] = v01;
                index_map[i0 * index_map_dim + i1] = (uint32) index;
                index_map[i1 * index_map_dim + i0] = (uint32) index;
            }
        }
    }

#define T(A, B, C) (triangle){ .i0 = A, .i1 = B, .i2 = C }

    // Now when we have vertices and index_map
    // we can create new index buffer object (ibo)
    uint triangle_index;
    for (triangle_index = 0; triangle_index < input.triangle_count; triangle_index++)
    {
        /*
                 0
                / \
              01   20
              /     \
             1---12--2
        */

        uint32 i0 = input.triangles[triangle_index].i0;
        uint32 i1 = input.triangles[triangle_index].i1;
        uint32 i2 = input.triangles[triangle_index].i2;

        uint32 i01 = index_map[i0 * index_map_dim + i1];
        uint32 i12 = index_map[i1 * index_map_dim + i2];
        uint32 i20 = index_map[i2 * index_map_dim + i0];

        result.triangles[triangle_i++] = T(i0, i01, i20);
        result.triangles[triangle_i++] = T(i01, i1, i12);
        result.triangles[triangle_i++] = T(i20, i12, i2);
        result.triangles[triangle_i++] = T(i20, i01, i12);
    }

    return result;
}

cpu_mesh mesh_ico_sphere_create(memory_allocator allocator, memory_allocator temp_allocator)
{
    uint i;

    // Create initial subdivide
    subdivide init;
    {
        float phi = (1.0f + sqrtf(5.0f)) * 0.5f;
        float a = 1.0f;
        float b = 1.0f / phi;

        uint32 vertex_count = 0;
        vector3 *vertices = ALLOCATE_ARRAY(temp_allocator, vector3, 12);
        vertices[vertex_count++] = vector3_create( b,  a,  0);
        vertices[vertex_count++] = vector3_create( 0,  b,  a);
        vertices[vertex_count++] = vector3_create( a,  0,  b);
        vertices[vertex_count++] = vector3_create(-b,  a,  0);
        vertices[vertex_count++] = vector3_create( 0, -b,  a);
        vertices[vertex_count++] = vector3_create( a,  0, -b);
        vertices[vertex_count++] = vector3_create( b, -a,  0);
        vertices[vertex_count++] = vector3_create( 0,  b, -a);
        vertices[vertex_count++] = vector3_create(-a,  0,  b);
        vertices[vertex_count++] = vector3_create(-b, -a,  0);
        vertices[vertex_count++] = vector3_create( 0, -b, -a);
        vertices[vertex_count++] = vector3_create(-a,  0, -b);
        ASSERT(vertex_count = 12);

        uint32 triangle_count = 0;
        triangle *triangles = ALLOCATE_ARRAY(temp_allocator, triangle, 20);
        triangles[triangle_count++] = T(3, 0, 7);
        triangles[triangle_count++] = T(0, 3, 1);
        triangles[triangle_count++] = T(8, 4, 1);
        triangles[triangle_count++] = T(4, 2, 1);
        triangles[triangle_count++] = T(5, 10, 7);
        triangles[triangle_count++] = T(10, 11, 7);
        triangles[triangle_count++] = T(9, 6, 4);
        triangles[triangle_count++] = T(6, 9, 10);
        triangles[triangle_count++] = T(11, 8, 3);
        triangles[triangle_count++] = T(8, 11, 9);
        triangles[triangle_count++] = T(2, 5, 0);
        triangles[triangle_count++] = T(5, 2, 6);
        triangles[triangle_count++] = T(3, 8, 1);
        triangles[triangle_count++] = T(2, 0, 1);
        triangles[triangle_count++] = T(11, 3, 7);
        triangles[triangle_count++] = T(0, 5, 7);
        triangles[triangle_count++] = T(9, 11, 10);
        triangles[triangle_count++] = T(5, 6, 10);
        triangles[triangle_count++] = T(8, 9, 4);
        triangles[triangle_count++] = T(6, 2, 4);
        ASSERT(triangle_count = 20);

        init.vertices = vertices;
        init.vertex_count = vertex_count;
        init.triangles = triangles;
        init.triangle_count = triangle_count;
    }

    subdivide subdiv0 = subdivide_triangles(temp_allocator, init);
    subdivide subdiv1 = subdivide_triangles(temp_allocator, subdiv0);
    subdivide subdiv = subdivide_triangles(temp_allocator, subdiv1);

    // Allocate space for position and normal
    usize vbo_size = subdiv.vertex_count * sizeof(vector3) * 2;
    float32 *vbo_data = ALLOCATE_BUFFER(allocator, vbo_size);
    uint32 vbo_count = 0;
    for (i = 0; i < subdiv.vertex_count; i++)
    {
        float32 normal = vector3_norm(subdiv.vertices[i]);

        vbo_data[vbo_count++] = subdiv.vertices[i].x / normal;
        vbo_data[vbo_count++] = subdiv.vertices[i].y / normal;
        vbo_data[vbo_count++] = subdiv.vertices[i].z / normal;
        vbo_data[vbo_count++] = subdiv.vertices[i].x / normal;
        vbo_data[vbo_count++] = subdiv.vertices[i].y / normal;
        vbo_data[vbo_count++] = subdiv.vertices[i].z / normal;
    }

    usize ibo_size = subdiv.triangle_count * sizeof(triangle);
    uint32 *ibo_data = ALLOCATE_BUFFER(allocator, ibo_size);
    uint32 ibo_count = 0;
    for (i = 0; i < subdiv.triangle_count; i++)
    {
        ibo_data[ibo_count++] = subdiv.triangles[i].i0;
        ibo_data[ibo_count++] = subdiv.triangles[i].i1;
        ibo_data[ibo_count++] = subdiv.triangles[i].i2;
    }

    vertex_buffer_layout vbl = {};
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 3);

    cpu_mesh result;
    result.vbo = memory_view_create(vbo_data, vbo_size);
    result.ibo = memory_view_create(ibo_data, ibo_size);
    result.vbl = vbl;

    return result;
}

#undef T

cpu_mesh mesh_ui_frame_create(void)
{
    // 3--------2
    // |\      /|
    // | 7----6 |
    // | |    | |
    // | 4----5 |
    // |/      \|
    // 0--------1

    static float32 vbo_data[] = {
        -1.f, -1.f,  0.f,  0.f,
         1.f, -1.f,  0.f,  0.f,
         1.f,  1.f,  0.f,  0.f,
        -1.f,  1.f,  0.f,  0.f,
        -1.f, -1.f,  1.f,  1.f,
         1.f, -1.f, -1.f,  1.f,
         1.f,  1.f, -1.f, -1.f,
        -1.f,  1.f,  1.f, -1.f,
    };

    static uint32 ibo_data[] = {
        0, 1, 4,
        4, 1, 5,
        1, 2, 5,
        5, 2, 6,
        2, 3, 6,
        6, 3, 7,
        3, 0, 7,
        7, 0, 4,
    };

    memory_view vbo = memory_view_create(vbo_data, sizeof(vbo_data));
    memory_view ibo = memory_view_create(ibo_data, sizeof(ibo_data));
    vertex_buffer_layout vbl = {};
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 2);
    render_vertex_buffer_layout_push(&vbl, sizeof(float), 2);

    cpu_mesh result = {};
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}
