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

struct triangle
{
    uint32 i0, i1, i2;
};

static triangle T(uint32 i0, uint32 i1, uint32 i2)
{
    triangle result;
    result.i0 = i0;
    result.i1 = i1;
    result.i2 = i2;
    return result;
}

struct subdivide_result
{
    bool32 success;
    array<vector3> vertices;
    array<triangle> triangles;
};

subdivide_result subdivide_triangles(memory_allocator a, array<vector3> vertices, array<triangle> triangles)
{
    subdivide_result result = {};
    usize edge_count = 3 * triangles.size(); // Every triangles has 3 edges, but every edge is edge for two triangles
    result.vertices = a.allocate_array<vector3>(vertices.size() + edge_count / 2);
    result.triangles = a.allocate_array<triangle>(4 * triangles.size());

    // I will need index map, which will map pairs of indices, to the single index
    // of vertex, which will divide selected edge
    usize index_map_dim = vertices.size();
    auto index_map = a.allocate_array<uint32>(index_map_dim * index_map_dim);
    index_map.resize(index_map_dim * index_map_dim);

    // Copy all current vertices
    for (int i = 0; i < vertices.size(); i++)
    {
        result.vertices.push_back(vertices[i]);
    }

    // Push new vertices for all edges and record its indices into index map
    for (int i = 0; i < triangles.size(); i++)
    {
        uint32 iis[4] = { triangles[i].i0, triangles[i].i1, triangles[i].i2, triangles[i].i0 };
        for (int j = 0; j < 3; j++)
        {
            uint32 i0 = iis[j];
            uint32 i1 = iis[j + 1];
            usize index = index_map[i0 * index_map_dim + i1];
            if (index == 0)
            {
                index = result.vertices.size();
                vector3 v01 = 0.5f * (result.vertices[i0] + result.vertices[i1]);
                result.vertices.push_back(v01);
                index_map[i0 * index_map_dim + i1] = (uint32) index;
                index_map[i1 * index_map_dim + i0] = (uint32) index;
            }
        }
    }

    // Now when we have vertices and index_map
    // we can create new index buffer object (ibo)
    for (int triangle_index = 0; triangle_index < triangles.size(); triangle_index++)
    {
        /*
                 0
                / \
              01   20
              /     \
             1---12--2
        */

        uint32 i0 = triangles[triangle_index].i0;
        uint32 i1 = triangles[triangle_index].i1;
        uint32 i2 = triangles[triangle_index].i2;

        uint32 i01 = index_map[i0 * index_map_dim + i1];
        uint32 i12 = index_map[i1 * index_map_dim + i2];
        uint32 i20 = index_map[i2 * index_map_dim + i0];

        result.triangles.push_back(T(i0, i01, i20));
        result.triangles.push_back(T(i01, i1, i12));
        result.triangles.push_back(T(i20, i12, i2));
        result.triangles.push_back(T(i20, i01, i12));
    }

    result.success = true;
    return result;
}

cpu_mesh make_ico_sphere(memory_allocator temp_allocator, memory_allocator allocator)
{
    float phi = (1.0f + sqrtf(5.0f)) * 0.5f;
    float a = 1.0f;
    float b = 1.0f / phi;

    auto vertices = temp_allocator.allocate_array<vector3>(12);
    vertices.push_back(V3( b,  a,  0));
    vertices.push_back(V3( 0,  b,  a));
    vertices.push_back(V3( a,  0,  b));
    vertices.push_back(V3(-b,  a,  0));
    vertices.push_back(V3( 0, -b,  a));
    vertices.push_back(V3( a,  0, -b));
    vertices.push_back(V3( b, -a,  0));
    vertices.push_back(V3( 0,  b, -a));
    vertices.push_back(V3(-a,  0,  b));
    vertices.push_back(V3(-b, -a,  0));
    vertices.push_back(V3( 0, -b, -a));
    vertices.push_back(V3(-a,  0, -b));

    auto triangles = temp_allocator.allocate_array<triangle>(20);
    triangles.push_back(T(3, 0, 7));
    triangles.push_back(T(0, 3, 1));
    triangles.push_back(T(8, 4, 1));
    triangles.push_back(T(4, 2, 1));
    triangles.push_back(T(5, 10, 7));
    triangles.push_back(T(10, 11, 7));
    triangles.push_back(T(9, 6, 4));
    triangles.push_back(T(6, 9, 10));
    triangles.push_back(T(11, 8, 3));
    triangles.push_back(T(8, 11, 9));
    triangles.push_back(T(2, 5, 0));
    triangles.push_back(T(5, 2, 6));
    triangles.push_back(T(3, 8, 1));
    triangles.push_back(T(2, 0, 1));
    triangles.push_back(T(11, 3, 7));
    triangles.push_back(T(0, 5, 7));
    triangles.push_back(T(9, 11, 10));
    triangles.push_back(T(5, 6, 10));
    triangles.push_back(T(8, 9, 4));
    triangles.push_back(T(6, 2, 4));

    subdivide_result subdiv0 = subdivide_triangles(temp_allocator, vertices, triangles);
    subdivide_result subdiv1 = subdivide_triangles(temp_allocator, subdiv0.vertices, subdiv0.triangles);
    subdivide_result subdiv = subdivide_triangles(temp_allocator, subdiv1.vertices, subdiv1.triangles);


    auto vbo = allocator.allocate_buffer(subdiv.vertices.size() * (2 * sizeof(vector3)));
    auto vbo_data = (float32 *) vbo.data;
    auto vbo_count = 0;
    for (int i = 0; i < subdiv.vertices.size(); i++)
    {
        auto normal = norm(subdiv.vertices[i]);

        vbo_data[vbo_count++] = subdiv.vertices[i].x / normal;
        vbo_data[vbo_count++] = subdiv.vertices[i].y / normal;
        vbo_data[vbo_count++] = subdiv.vertices[i].z / normal;
        vbo_data[vbo_count++] = subdiv.vertices[i].x / normal;
        vbo_data[vbo_count++] = subdiv.vertices[i].y / normal;
        vbo_data[vbo_count++] = subdiv.vertices[i].z / normal;
    }

    auto ibo = allocator.allocate_buffer(subdiv.triangles.size() * sizeof(triangle));
    auto ibo_data = (uint32 *) ibo.data;
    auto ibo_count = 0;
    for (int i = 0; i < subdiv.triangles.size(); i++)
    {
        ibo_data[ibo_count++] = subdiv.triangles[i].i0;
        ibo_data[ibo_count++] = subdiv.triangles[i].i1;
        ibo_data[ibo_count++] = subdiv.triangles[i].i2;
    }

    auto vbl = vertex_buffer_layout::make();
    vbl.push<float>(3);
    vbl.push<float>(3);

    cpu_mesh result;
    result.vbo = vbo;
    result.ibo = ibo;
    result.vbl = vbl;

    return result;
}

