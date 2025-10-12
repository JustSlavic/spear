#include "obj.h"
#include "../parse_primitives.h"
#include "../collision.h"


char const *obj_decode_result_to_cstring(obj_decode_result result)
{
    switch (result)
    {
    case ObjDecode_Success: return "ObjDecode_Success";
    case ObjDecode_VertexBufferOverflow: return "ObjDecode_VertexBufferOverflow";
    case ObjDecode_IndexBufferOverflow: return "ObjDecode_IndexBufferOverflow";
    }
    return NULL;
}

static obj_decode_result
obj_decode_internal(void *file_data,
                    usize file_size,
                    float **out_v,
                    uint32 *out_v_count,
                    int32 **out_vi,
                    uint32 *out_vi_count,
                    float **out_vt,
                    uint32 *out_vt_count,
                    int32 **out_vti,
                    uint32 *out_vti_count,
                    float **out_vn,
                    uint32 *out_vn_count,
                    int32 **out_vni,
                    uint32 *out_vni_count)
{
    char c = 0;
    uint32 index = 0;
    char *data = (char *) file_data;

    // uint32 f_count = 0, s_count = 0;

    uint32 v_count = 0;
    uint32 v_capacity = 0;
    float *v = NULL;
    if (out_v)
    {
        v_capacity = 12;
        v = malloc(v_capacity * sizeof(float));
    }

    uint32 vi_count = 0;
    uint32 vi_capacity = 0;
    int32 *vi = NULL;
    if (out_vi)
    {
        vi_capacity = 3;
        vi = malloc(vi_capacity * sizeof(int32));
    }

    uint32 vt_count = 0;
    uint32 vt_capacity = 0;
    float *vt = NULL;
    if (out_vt)
    {
        vt_capacity = 8;
        vt = malloc(vt_capacity * sizeof(float));
    }

    uint32 vti_count = 0;
    uint32 vti_capacity = 0;
    int32 *vti = NULL;
    if (out_vti)
    {
        vti_capacity = 3;
        vti = malloc(vti_capacity * sizeof(int32));
    }

    uint32 vn_count = 0;
    uint32 vn_capacity = 0;
    float *vn = NULL;
    if (out_vn)
    {
        vn_capacity = 12;
        vn = malloc(vn_capacity * sizeof(float));
    }

    uint32 vni_count = 0;
    uint32 vni_capacity = 0;
    int32 *vni = NULL;
    if (out_vni)
    {
        vni_capacity = 3;
        vni = malloc(vni_capacity * sizeof(int32));
    }

    while (index < file_size)
    {
        c = data[index];
        if (c == '#')
        {
            index += spear_parse_until(data + index, file_size - index, &is_ascii_newline);
            index += 1; // Consume '\n'
        }
        else if (c == 'v')
        {
            index += 1; // Consume 'v'
            c = data[index];
            if (c == 't')
            {
                index += 1; // Consume 't'

                float x, y;

                index += 1; // Consume ' '
                index += spear_parse_float32(data + index, file_size - index, &x);
                index += 1; // Consume ' '
                index += spear_parse_float32(data + index, file_size - index, &y);
                index += 1; // Consume '\n'

                if (vt)
                {
                    if (vt_capacity < vt_count + 2)
                    {
                        vt = realloc(vt, sizeof(float) * vt_capacity * 2);
                        vt_capacity *= 2;
                        // printf("Grow vt to capacity=%u\n", vt_capacity);
                    }
                    vt[vt_count++] = x;
                    vt[vt_count++] = y;
                }
                else
                {
                    vt_count += 2;
                }
            }
            else if (c == 'n')
            {
                index += 1; // Consume 'n'

                float32 x, y, z;

                index += 1; // Consume ' '
                index += spear_parse_float32(data + index, file_size - index, &x);
                index += 1; // Consume ' '
                index += spear_parse_float32(data + index, file_size - index, &y);
                index += 1; // Consume ' '
                index += spear_parse_float32(data + index, file_size - index, &z);

                if (vn)
                {
                    if (vn_capacity < vn_count + 3)
                    {
                        vn = realloc(vn, sizeof(float) * vn_capacity * 2);
                        vn_capacity *= 2;
                        // printf("Grow vn to capacity=%u\n", vn_capacity);
                    }
                    vn[vn_count++] = x;
                    vn[vn_count++] = y;
                    vn[vn_count++] = z;
                }
                else
                {
                    vn_count += 3;
                }
            }
            else // This is a 'v' line
            {
                float x, y, z;

                index += 1; // Consume ' '
                index += spear_parse_float32(data + index, file_size - index, &x);
                index += 1; // Consume ' '
                index += spear_parse_float32(data + index, file_size - index, &y);
                index += 1; // Consume ' '
                index += spear_parse_float32(data + index, file_size - index, &z);

                if (v)
                {
                    if (v_capacity < v_count + 3)
                    {
                        v = realloc(v, sizeof(float) * v_capacity * 2);
                        v_capacity *= 2;
                        // printf("Grow v to capacity=%u\n", v_capacity);
                    }
                    v[v_count++] = x;
                    v[v_count++] = y;
                    v[v_count++] = z;
                }
                else
                {
                    v_count += 3;
                }
            }
        }
        else if (c == 's')
        {
            index += 1; // Consume 's'
            // s_count += 1;
        }
        else if (c == 'f')
        {
            index += 1; // Consume 'f'
            // f_count += 1;

            int32 v0, v1, v2;
            int32 vn0, vn1, vn2;
            int32 vt0, vt1, vt2;

            index += 1; // consume ' '
            index += spear_parse_int32(data + index, file_size - index, &v0);
            index += 1; // consume '/'
            index += spear_parse_int32(data + index, file_size - index, &vt0);
            index += 1; // consume '/'
            index += spear_parse_int32(data + index, file_size - index, &vn0);
            index += 1; // consume ' '
            index += spear_parse_int32(data + index, file_size - index, &v1);
            index += 1; // consume '/'
            index += spear_parse_int32(data + index, file_size - index, &vt1);
            index += 1; // consume '/'
            index += spear_parse_int32(data + index, file_size - index, &vn1);
            index += 1; // consume ' '
            index += spear_parse_int32(data + index, file_size - index, &v2);
            index += 1; // consume '/'
            index += spear_parse_int32(data + index, file_size - index, &vt2);
            index += 1; // consume '/'
            index += spear_parse_int32(data + index, file_size - index, &vn2);

            c = data[index];
            if (c == ' ')
            {
                int v3, vn3, vt3;

                index += 1; // consume ' '
                index += spear_parse_int32(data + index, file_size - index, &v3);
                index += 1; // consume '/'
                index += spear_parse_int32(data + index, file_size - index, &vt3);
                index += 1; // consume '/'
                index += spear_parse_int32(data + index, file_size - index, &vn3);

                if (vi)
                {
                    if (vi_capacity < vi_count + 6)
                    {
                        vi_capacity *= 2;
                        vi = realloc(vi, sizeof(int32) * vi_capacity);
                        // printf("Grow vi to capacity=%u\n", vi_capacity);
                    }
                    vi[vi_count++] = v0;
                    vi[vi_count++] = v1;
                    vi[vi_count++] = v2;
                    vi[vi_count++] = v2;
                    vi[vi_count++] = v3;
                    vi[vi_count++] = v0;
                }
                else
                {
                    vi_count += 6;
                }

                if (vti)
                {
                    if (vti_capacity < vti_count + 6)
                    {
                        vti_capacity *= 2;
                        vti = realloc(vti, sizeof(int32) * vti_capacity);
                        // printf("Grow vti to capacity=%u\n", vti_capacity);
                    }
                    vti[vti_count++] = vt0;
                    vti[vti_count++] = vt1;
                    vti[vti_count++] = vt2;
                    vti[vti_count++] = vt2;
                    vti[vti_count++] = vt3;
                    vti[vti_count++] = vt0;
                }
                else
                {
                    vti_count += 6;
                }

                if (vni)
                {
                    if (vni_capacity < vni_count + 6)
                    {
                        vni_capacity *= 2;
                        vni = realloc(vni, sizeof(int32) * vni_capacity);
                        // printf("Grow vni to capacity=%u\n", vni_capacity);
                    }
                    vni[vni_count++] = vn0;
                    vni[vni_count++] = vn1;
                    vni[vni_count++] = vn2;
                    vni[vni_count++] = vn2;
                    vni[vni_count++] = vn3;
                    vni[vni_count++] = vn0;
                }
                else
                {
                    vni_count += 6;
                }
            }
            else
            {
                if (vi)
                {
                    if (vi_capacity < vi_count + 3)
                    {
                        vi_capacity *= 2;
                        vi = realloc(vi, sizeof(int32) * vi_capacity);
                        // printf("Grow vi to capacity=%u\n", vi_capacity);
                    }
                    vi[vi_count++] = v0;
                    vi[vi_count++] = v1;
                    vi[vi_count++] = v2;
                }
                else
                {
                    vi_count += 3;
                }

                if (vti)
                {
                    if (vti_capacity < vti_count + 3)
                    {
                        vti_capacity *= 2;
                        vti = realloc(vti, sizeof(int32) * vti_capacity);
                        // printf("Grow vti to capacity=%u\n", vti_capacity);
                    }
                    vti[vti_count++] = vt0;
                    vti[vti_count++] = vt1;
                    vti[vti_count++] = vt2;
                }
                else
                {
                    vti_count += 3;
                }

                if (vni)
                {
                    if (vni_capacity < vni_count + 3)
                    {
                        vni_capacity *= 2;
                        vni = realloc(vni, sizeof(int32) * vni_capacity);
                        // printf("Grow vni to capacity=%u\n", vni_capacity);
                    }
                    vni[vni_count++] = vn0;
                    vni[vni_count++] = vn1;
                    vni[vni_count++] = vn2;
                }
                else
                {
                    vni_count += 3;
                }
            }
        }
        else
        {
            index += 1;
        }
    }

    if (out_v_count) *out_v_count = v_count;
    if (out_v) *out_v = v;
    if (out_vi_count) *out_vi_count = vi_count;
    if (out_vi) *out_vi = vi;
    if (out_vt_count) *out_vt_count = vt_count;
    if (out_vt) *out_vt = vt;
    if (out_vti_count) *out_vti_count = vti_count;
    if (out_vti) *out_vti = vti;
    if (out_vn_count) *out_vn_count = vn_count;
    if (out_vn) *out_vn = vn;
    if (out_vni_count) *out_vni_count = vni_count;
    if (out_vni) *out_vni = vni;

    return ObjDecode_Success;
}

obj_decode_result
obj_extract_size(void *file_data,
                 usize file_size,
                 uint32 *out_vertex_buffer_size,
                 uint32 *out_index_buffer_size)
{
    uint32 v_count, vi_count, vt_count, vti_count, vn_count, vni_count;

    obj_decode_result decode_result = obj_decode_internal(
        file_data, file_size,
        NULL, &v_count,
        NULL, &vi_count,
        NULL, &vt_count,
        NULL, &vti_count,
        NULL, &vn_count,
        NULL, &vni_count);

    // printf("obj_extract_size:\n");
    // printf("    v={ %p, %u, %p, %u }; vt={ %p, %u, %p, %u }; vn={ %p, %u, %p, %u };\n",
    //     NULL, v_count, NULL, vi_count,
    //     NULL, vt_count, NULL, vti_count,
    //     NULL, vn_count, NULL, vni_count);

    *out_vertex_buffer_size = sizeof(float) * (v_count + vt_count + vn_count);
    *out_index_buffer_size = sizeof(int32) * vi_count;

    return decode_result;
}

obj_decode_result
obj_decode(void *file_data,
           usize file_size,
           void *vertex_data,
           uint32 vertex_size,
           void *index_data,
           uint32 index_size)
{
    float *v, *vt, *vn;
    int32 *vi, *vti, *vni;
    uint32 v_count, vi_count, vt_count, vti_count, vn_count, vni_count;

    obj_decode_internal(
        file_data, file_size,
        &v,   &v_count,
        &vi,  &vi_count,
        &vt,  &vt_count,
        &vti, &vti_count,
        &vn,  &vn_count,
        &vni, &vni_count);

    // printf("obj_decode:\n");
    // printf("v={ %p, %u, %p, %u }; vt={ %p, %u, %p, %u }; vn={ %p, %u, %p, %u };\n",
    //     v, v_count, vi, vi_count,
    //     vt, vt_count, vti, vti_count,
    //     vn, vn_count, vni, vni_count);

    float *v_data = (float *) vertex_data;
    int32 *i_data = (int32 *) index_data;

    uint32 v_write_index = 0;
    uint32 i_write_index = 0;

    // printf("vertex_size = %u; v_size = %zu\n", vertex_size, v_count * sizeof(float));

    memcpy(vertex_data, v, v_count * sizeof(float));

    uint32 *table = malloc(vi_count * sizeof(*table));
    memset(table, 0, vi_count * sizeof(*table));

    uint ii = 0;

    uint i;
    for (i = 0; i < vi_count; i++)
    {
        uint32 vertex_index = vi[i];
        uint32 normal_index = vni[i];
        // printf("i=%u; vertex_index = %u\n", i, vertex_index);
        uint32 *entry = table + vertex_index; // This is possibly new enumeration index
        // printf("  *entry = %u\n", *entry);
        if (*entry == 0) // If no index is present
        {
            *entry = ii + 1; // Make new index (starts from 1)
            ii += 1;

            // printf("    because *entry == 0, make new index for this vertex (%u)\n", *entry);

            // Write vertex data
            v_data[v_write_index++] = v[3 * (vertex_index - 1)]; // x
            v_data[v_write_index++] = v[3 * (vertex_index - 1) + 1];
            v_data[v_write_index++] = v[3 * (vertex_index - 1) + 2];
            v_data[v_write_index++] = vn[3 * (normal_index - 1)];
            v_data[v_write_index++] = vn[3 * (normal_index - 1) + 1];
            v_data[v_write_index++] = vn[3 * (normal_index - 1) + 2];

            // printf("    write vertex data: %f %f %f %f %f %f\n", v[3 * (vertex_index - 1)], v[3 * (vertex_index - 1) + 1], v[3 * (vertex_index - 1) + 2], 0.f, 0.f, 0.f);
            i_data[i_write_index++] = *entry - 1;
            // printf("    write index data i_data[%u] := %u\n",
            //     i_write_index - 1, *entry);
        }
        else
        {
            i_data[i_write_index++] = *entry - 1;
            printf("    *entry /= 0 (== %u), I just write index data i_data[%u] := %u\n",
                *entry, i_write_index - 1, *entry);
        }
    }

    return ObjDecode_Success;
}


obj_decode_result
obj_decode_no_index(void *file_data,
                    usize file_size,
                    void **vertex_data,
                    uint32 *vertex_size)
{
    uint32 result_count = 0;
    uint32 result_capacity = 12;
    float *result_data = malloc(result_capacity * sizeof(float));

    float *v, *vt, *vn;
    int32 *vi, *vti, *vni;
    uint32 v_count, vi_count, vt_count, vti_count, vn_count, vni_count;

    obj_decode_internal(
        file_data, file_size,
        &v,   &v_count,
        &vi,  &vi_count,
        &vt,  &vt_count,
        &vti, &vti_count,
        &vn,  &vn_count,
        &vni, &vni_count);

    // printf("obj_decode:\n");
    // printf("v={ %p, %u, %p, %u }; vt={ %p, %u, %p, %u }; vn={ %p, %u, %p, %u };\n",
    //     v, v_count, vi, vi_count,
    //     vt, vt_count, vti, vti_count,
    //     vn, vn_count, vni, vni_count);

    int i;
    for (i = 0; i < vi_count; i+=3)
    {
        uint32 vertex_index_0 = vi[i + 0];
        uint32 vertex_index_1 = vi[i + 1];
        uint32 vertex_index_2 = vi[i + 2];

        if (result_capacity < result_count + 3 * 6)
        {
            result_capacity *= 2;
            result_data = realloc(result_data, result_capacity * sizeof(float));
            // printf("Resize: from %u to %u\n", result_capacity / 2, result_capacity);
        }

        float ax = v[3 * (vertex_index_0 - 1) + 0];
        float ay = v[3 * (vertex_index_0 - 1) + 1];
        float az = v[3 * (vertex_index_0 - 1) + 2];

        float bx = v[3 * (vertex_index_1 - 1) + 0];
        float by = v[3 * (vertex_index_1 - 1) + 1];
        float bz = v[3 * (vertex_index_1 - 1) + 2];

        float cx = v[3 * (vertex_index_2 - 1) + 0];
        float cy = v[3 * (vertex_index_2 - 1) + 1];
        float cz = v[3 * (vertex_index_2 - 1) + 2];

        float nx, ny, nz;
        triangle_get_normal(ax, ay, az, bx, by, bz, cx, cy, cz, &nx, &ny, &nz);

        result_data[result_count++] = ax;
        result_data[result_count++] = ay;
        result_data[result_count++] = az;
        result_data[result_count++] = nx;
        result_data[result_count++] = ny;
        result_data[result_count++] = nz;

        result_data[result_count++] = bx;
        result_data[result_count++] = by;
        result_data[result_count++] = bz;
        result_data[result_count++] = nx;
        result_data[result_count++] = ny;
        result_data[result_count++] = nz;

        result_data[result_count++] = cx;
        result_data[result_count++] = cy;
        result_data[result_count++] = cz;
        result_data[result_count++] = nx;
        result_data[result_count++] = ny;
        result_data[result_count++] = nz;
    }

    *vertex_data = result_data;
    *vertex_size = result_count * sizeof(float);

    return ObjDecode_Success;
}
