#include "platonic_solids.hpp"


cpu_mesh make_platonic_tetrahedron()
{
    float32 a = 1.0f / 3.0f;
    float32 b = sqrtf(8.0f / 9.0f);
    float32 c = sqrtf(2.0f / 9.0f);
    float32 d = sqrtf(2.0f / 3.0f);

    vector3 n0 = normalized(cross(V3(c, -d, 1+a), V3(c, d, 1+a)));
    vector3 n1 = normalized(cross(V3(c, d, 1+a), V3(-b, 0, 1+a)));
    vector3 n2 = normalized(cross(V3(-b, 0, 1+a), V3(c, -d, 1+a)));
    vector3 n3 = normalized(cross(V3(b+c, -d, 0), V3(b+c, -d, 0)));

    static float32 vbo_data[] =
    {
         0,  0,  1, n0.x, n0.y, n0.z,
        -c,  d, -a, n0.x, n0.y, n0.z,
        -c, -d, -a, n0.x, n0.y, n0.z,

         0,  0,  1, n1.x, n1.y, n1.z,
        -c, -d, -a, n1.x, n1.y, n1.z,
         b,  0, -a, n1.x, n1.y, n1.z,

         0,  0,  1, n2.x, n2.y, n2.z,
         b,  0, -a, n2.x, n2.y, n2.z,
        -c,  d, -a, n2.x, n2.y, n2.z,

         b,  0, -a, n3.x, n3.y, n3.z,
        -c, -d, -a, n3.x, n3.y, n3.z,
        -c,  d, -a, n3.x, n3.y, n3.z,
    };

    auto vbo = memory_buffer::from(vbo_data, sizeof(vbo_data));
    auto vbl = vertex_buffer_layout::make();
    vbl.push<float>(3);
    vbl.push<float>(3);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = memory_buffer{};
    result.vbl = vbl;

    return result;
}

cpu_mesh make_platonic_cube()
{
    float32 a = 0.5773502691896258f; // 1 / sqrt(3)

    static float32 vbo_data[] = {
        -a,  a, -a,  0,  0, -1,
        -a, -a, -a,  0,  0, -1,
         a, -a, -a,  0,  0, -1,

         a, -a, -a,  0,  0, -1,
         a,  a, -a,  0,  0, -1,
        -a,  a, -a,  0,  0, -1,

        -a, -a,  a,  0, -1,  0,
        -a, -a, -a,  0, -1,  0,
         a, -a, -a,  0, -1,  0,

         a, -a, -a,  0, -1,  0,
         a, -a,  a,  0, -1,  0,
        -a, -a,  a,  0, -1,  0,

        -a, -a,  a, -1,  0,  0,
        -a, -a, -a, -1,  0,  0,
        -a,  a, -a, -1,  0,  0,

        -a,  a, -a, -1,  0,  0,
        -a,  a,  a, -1,  0,  0,
        -a, -a,  a, -1,  0,  0,

        -a,  a,  a,  0,  0,  1,
        -a, -a,  a,  0,  0,  1,
         a, -a,  a,  0,  0,  1,

         a, -a,  a,  0,  0,  1,
         a,  a,  a,  0,  0,  1,
        -a,  a,  a,  0,  0,  1,

        -a,  a,  a,  0,  1,  0,
        -a,  a, -a,  0,  1,  0,
         a,  a, -a,  0,  1,  0,

         a,  a, -a,  0,  1,  0,
         a,  a,  a,  0,  1,  0,
        -a,  a,  a,  0,  1,  0,

         a, -a,  a,  1,  0,  0,
         a, -a, -a,  1,  0,  0,
         a,  a, -a,  1,  0,  0,

         a,  a, -a,  1,  0,  0,
         a,  a,  a,  1,  0,  0,
         a, -a,  a,  1,  0,  0,
    };

    auto vbo = memory_buffer::from(vbo_data, sizeof(vbo_data));
    auto vbl = vertex_buffer_layout::make();
    vbl.push<float>(3);
    vbl.push<float>(3);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = memory_buffer{};
    result.vbl = vbl;

    return result;
}

cpu_mesh make_platonic_octahedron()
{
    vector3 n0 = normalized(cross(V3( 1, -1,  0), V3( 1,  0, -1)));
    vector3 n1 = normalized(cross(V3(-1,  0, -1), V3(-1, -1,  0)));
    vector3 n2 = normalized(cross(V3(-1,  1,  0), V3(-1,  0, -1)));
    vector3 n3 = normalized(cross(V3( 1,  0, -1), V3( 1,  1,  0)));

    vector3 n4 = normalized(cross(V3( 1,  0,  1), V3( 1, -1,  0)));
    vector3 n5 = normalized(cross(V3(-1, -1,  0), V3(-1,  0,  1)));
    vector3 n6 = normalized(cross(V3(-1,  0,  1), V3(-1,  1,  0)));
    vector3 n7 = normalized(cross(V3( 1,  1,  0), V3( 1,  0,  1)));

    static float32 vbo_data[] =
    {
         1.f,  0.f,  0.f, n0.x, n0.y, n0.z,
         0.f,  1.f,  0.f, n0.x, n0.y, n0.z,
         0.f,  0.f,  1.f, n0.x, n0.y, n0.z,

        -1.f,  0.f,  0.f, n1.x, n1.y, n1.z,
         0.f,  0.f,  1.f, n1.x, n1.y, n1.z,
         0.f,  1.f,  0.f, n1.x, n1.y, n1.z,

        -1.f,  0.f,  0.f, n2.x, n2.y, n2.z,
         0.f, -1.f,  0.f, n2.x, n2.y, n2.z,
         0.f,  0.f,  1.f, n2.x, n2.y, n2.z,

         1.f,  0.f,  0.f, n3.x, n3.y, n3.z,
         0.f,  0.f,  1.f, n3.x, n3.y, n3.z,
         0.f, -1.f,  0.f, n3.x, n3.y, n3.z,

         1.f,  0.f,  0.f, n4.x, n4.y, n4.z,
         0.f,  0.f, -1.f, n4.x, n4.y, n4.z,
         0.f,  1.f,  0.f, n4.x, n4.y, n4.z,

        -1.f,  0.f,  0.f, n5.x, n5.y, n5.z,
         0.f,  1.f,  0.f, n5.x, n5.y, n5.z,
         0.f,  0.f, -1.f, n5.x, n5.y, n5.z,

        -1.f,  0.f,  0.f, n6.x, n6.y, n6.z,
         0.f,  0.f, -1.f, n6.x, n6.y, n6.z,
         0.f, -1.f,  0.f, n6.x, n6.y, n6.z,

         1.f,  0.f,  0.f, n7.x, n7.y, n7.z,
         0.f, -1.f,  0.f, n7.x, n7.y, n7.z,
         0.f,  0.f, -1.f, n7.x, n7.y, n7.z,
    };

    auto vbo = memory_buffer::from(vbo_data, sizeof(vbo_data));

    auto vbl = vertex_buffer_layout::make();
    vbl.push<float>(3);
    vbl.push<float>(3);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = memory_buffer{};
    result.vbl = vbl;

    return result;
}

cpu_mesh make_platonic_icosahedron()
{
    float phi = (1.0f + sqrtf(5.0f)) * 0.5f;
    float a = 1.0f;
    float b = 1.0f / phi;

    static float32 vbo_data[] =
    {
        -b,  a,  0,  0, 0, 0,
         b,  a,  0,  0, 0, 0,
         0,  b, -a,  0, 0, 0,

         b,  a,  0,  0, 0, 0,
        -b,  a,  0,  0, 0, 0,
         0,  b,  a,  0, 0, 0,

        -a,  0,  b,  0, 0, 0,
         0, -b,  a,  0, 0, 0,
         0,  b,  a,  0, 0, 0,

         0, -b,  a,  0, 0, 0,
         a,  0,  b,  0, 0, 0,
         0,  b,  a,  0, 0, 0,

         a,  0, -b,  0, 0, 0,
         0, -b, -a,  0, 0, 0,
         0,  b, -a,  0, 0, 0,

         0, -b, -a,  0, 0, 0,
        -a,  0, -b,  0, 0, 0,
         0,  b, -a,  0, 0, 0,

        -b, -a,  0,  0, 0, 0,
         b, -a,  0,  0, 0, 0,
         0, -b,  a,  0, 0, 0,

         b, -a,  0,  0, 0, 0,
        -b, -a,  0,  0, 0, 0,
         0, -b, -a,  0, 0, 0,

        -a,  0, -b,  0, 0, 0,
        -a,  0,  b,  0, 0, 0,
        -b,  a,  0,  0, 0, 0,

        -a,  0,  b,  0, 0, 0,
        -a,  0, -b,  0, 0, 0,
        -b, -a,  0,  0, 0, 0,

         a,  0,  b,  0, 0, 0,
         a,  0, -b,  0, 0, 0,
         b,  a,  0,  0, 0, 0,

         a,  0, -b,  0, 0, 0,
         a,  0,  b,  0, 0, 0,
         b, -a,  0,  0, 0, 0,

        -b,  a,  0,  0, 0, 0,
        -a,  0,  b,  0, 0, 0,
         0,  b,  a,  0, 0, 0,

         a,  0,  b,  0, 0, 0,
         b,  a,  0,  0, 0, 0,
         0,  b,  a,  0, 0, 0,

        -a,  0, -b,  0, 0, 0,
        -b,  a,  0,  0, 0, 0,
         0,  b, -a,  0, 0, 0,

         b,  a,  0,  0, 0, 0,
         a,  0, -b,  0, 0, 0,
         0,  b, -a,  0, 0, 0,

        -b, -a,  0,  0, 0, 0,
        -a,  0, -b,  0, 0, 0,
         0, -b, -a,  0, 0, 0,

         a,  0, -b,  0, 0, 0,
         b, -a,  0,  0, 0, 0,
         0, -b, -a,  0, 0, 0,

        -a,  0,  b,  0, 0, 0,
        -b, -a,  0,  0, 0, 0,
         0, -b,  a,  0, 0, 0,

         b, -a,  0,  0, 0, 0,
         a,  0,  b,  0, 0, 0,
         0, -b,  a,  0, 0, 0,
    };

    for (int vertex_index = 0; vertex_index < ARRAY_COUNT(vbo_data) / 6; vertex_index += 3)
    {
        int i0 = vertex_index * 6;
        int i1 = (vertex_index + 1) * 6;
        int i2 = (vertex_index + 2) * 6;

        vector3 v0 = V3(vbo_data[i0], vbo_data[i0 + 1], vbo_data[i0 + 2]);
        vector3 v1 = V3(vbo_data[i1], vbo_data[i1 + 1], vbo_data[i1 + 2]);
        vector3 v2 = V3(vbo_data[i2], vbo_data[i2 + 1], vbo_data[i2 + 2]);
        vector3 normal = normalized(cross(v0 - v1, v0 - v2));

        vbo_data[i0    ] = vbo_data[i0    ] / norm(v0);
        vbo_data[i0 + 1] = vbo_data[i0 + 1] / norm(v0);
        vbo_data[i0 + 2] = vbo_data[i0 + 2] / norm(v0);
        vbo_data[i0 + 3] = normal.x;
        vbo_data[i0 + 4] = normal.y;
        vbo_data[i0 + 5] = normal.z;

        vbo_data[i1    ] = vbo_data[i1    ] / norm(v1);
        vbo_data[i1 + 1] = vbo_data[i1 + 1] / norm(v1);
        vbo_data[i1 + 2] = vbo_data[i1 + 2] / norm(v1);
        vbo_data[i1 + 3] = normal.x;
        vbo_data[i1 + 4] = normal.y;
        vbo_data[i1 + 5] = normal.z;

        vbo_data[i2    ] = vbo_data[i2    ] / norm(v2);
        vbo_data[i2 + 1] = vbo_data[i2 + 1] / norm(v2);
        vbo_data[i2 + 2] = vbo_data[i2 + 2] / norm(v2);
        vbo_data[i2 + 3] = normal.x;
        vbo_data[i2 + 4] = normal.y;
        vbo_data[i2 + 5] = normal.z;
    }

    auto vbo = memory_buffer::from(vbo_data, sizeof(vbo_data));

    auto vbl = vertex_buffer_layout::make();
    vbl.push<float>(3);
    vbl.push<float>(3);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = memory_buffer{};
    result.vbl = vbl;

    return result;
}
