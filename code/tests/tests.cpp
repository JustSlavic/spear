#include <base.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <math/matrix2.hpp>
#include <math/matrix3.hpp>
#include <math/matrix4.hpp>

#include <math/complex.hpp>
#include <math/quaternion.hpp>
#include <math/unitary2.hpp>
#include <math/transform.hpp>
#include <ui/ui.hpp>


// @todo:
// - make so that test_##NAME function do not return bool32, it is confusing
// - go through all the test suites even if some are failed
// - collect statistics, how many tests passed, how many failed, how failed ones were called
// - color the failed and passed words in terminal

#define TEST_BEGIN(...) \
    int32 passed_tests_count = 0; \
    int32 failed_tests_count = 0; \
    (void)0
#define TEST_END(...) \
    printf("--------------------\n"); \
    printf("Tests passed: %d\n", passed_tests_count); \
    printf("Tests failed: %d\n", failed_tests_count); \
    (void)0

#define TEST(NAME) \
    static bool32 already_printed_result_##NAME = false; \
    void test_##NAME(bool32 *success)

#define TEST_PRINT_OK_RESULT(NAME) \
    if (!already_printed_result_##NAME) { \
        printf("Ok.\n"); \
        already_printed_result_##NAME = true; \
    } (void)0
#define TEST_RUN(NAME) \
    do { \
        printf("TEST %s... ", STRINGIFY(NAME)); \
        bool32 success = true; \
        test_##NAME(&success); \
        if (success) { \
            passed_tests_count += 1; \
            TEST_PRINT_OK_RESULT(NAME); \
        } else { \
            failed_tests_count += 1; \
        } \
    } while(0)

#define TEST_ASSERT(COND) \
    do { \
        if (COND) { } else { \
            printf("\n%s:%d FAIL\n", __FILE__, __LINE__); \
            *success = false; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_EQ(A, B) \
    do { \
        if (A == B) { } else { \
            printf("\n%s:%d FAIL (%s != %s)\n", __FILE__, __LINE__, STRINGIFY(A), STRINGIFY(B)); \
            *success = false; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_NEQ(A, B) \
    do { \
        if (A != B) { } else { \
            printf("\n%s:%d FAIL (%s == %s)\n", __FILE__, __LINE__, STRINGIFY(A), STRINGIFY(B)); \
            *success = false; \
            return; \
        } \
    } while(0)

#define TEST_ASSERT_FLOAT_EQ(A, B) \
    do { \
        if (::math::equal(A, B)) { } else { \
            printf("\n%s:%d FAIL (%s != %s)\n", __FILE__, __LINE__, STRINGIFY(A), STRINGIFY(B)); \
            *success = false; \
            return; \
        } \
    } while (0)

#define TEST_ASSERT_FLOAT_NEQ(A, B) \
    do { \
        if (!::math::equal(A, B)) { } else { \
            printf("\n%s:%d FAIL (%s == %s)\n", __FILE__, __LINE__, STRINGIFY(A), STRINGIFY(B)); \
            *success = false; \
            return; \
        } \
    } while (0)


// ----------------------------------------------


TEST(Matrix2)
{
    // Determinant
    {
        auto m1 = math::make_matrix2(1, 0,
                                     0, 1);
        auto det1 = determinant(m1);
        TEST_ASSERT_FLOAT_EQ(det1, 1.f);

        auto m2 = math::make_matrix2(2, 0,
                                     0, 2);
        auto det2 = determinant(m2);
        TEST_ASSERT_FLOAT_EQ(det2, 4.f);

        auto m3 = math::make_matrix2(1, 0,
                                     9, 1);
        auto det3 = determinant(m3);
        TEST_ASSERT_FLOAT_EQ(det3, 1.f);
    }
    // Inverse
    {
        auto m = math::make_matrix2(1, 2,
                                    100, 1);

        TEST_ASSERT_FLOAT_NEQ(determinant(m), 0.f);

        auto inversed = inverse(m);
        auto id = m * inversed;

        TEST_ASSERT_FLOAT_EQ(id, math::matrix2::identity());
    }
}

TEST(Matrix3)
{
    // Determinant
    {
        auto m1 = math::make_matrix3(1, 0, 0,
                                     0, 1, 0,
                                     0, 0, 1);
        auto det1 = determinant(m1);
        TEST_ASSERT_FLOAT_EQ(det1, 1.f);

        auto m2 = math::make_matrix3(2, 0, 0,
                                     0, 2, 0,
                                     0, 0, 2);
        auto det2 = determinant(m2);
        TEST_ASSERT_FLOAT_EQ(det2, 8.f);

        auto m3 = math::make_matrix3(1, 0, 0,
                                     9, 2, 0,
                                     9, 9, 2);
        auto det3 = determinant(m3);
        TEST_ASSERT_FLOAT_EQ(det3, 4.f);

        auto m4 = math::make_matrix3(2, 5, 3,
                                     6, 1, 4,
                                     7, 8, 9);
        auto det4 = determinant(m4);
        TEST_ASSERT_FLOAT_EQ(det4, -53.f);
    }
    // Inverse
    {
        auto m = math::make_matrix3(2, 5, 9,
                                    6, 1, 4,
                                    7, 8, 3);
        TEST_ASSERT_FLOAT_NEQ(determinant(m), 0.f);

        auto inversed = inverse(m);
        auto id = m * inversed;
        TEST_ASSERT_FLOAT_EQ(id, math::matrix3::identity());
    }
}

TEST(Matrix4)
{
    // Determinant
    {
        auto m = math::make_matrix4(1, 1, 1, 7,
                                    1, 5, 9, 1,
                                    0, 4, 5, 5,
                                    9, 1, 1, 2);
        TEST_ASSERT_FLOAT_EQ(determinant(m), 524.f);
    }
    // Inverse
    {
        auto m = math::make_matrix4(1, 1, 1, 7,
                                    1, 5, 9, 1,
                                    0, 4, 5, 5,
                                    9, 1, 1, 2);
        TEST_ASSERT_FLOAT_NEQ(determinant(m), 0.f);

        auto inversed = inverse(m);
        auto id = m * inversed;
        TEST_ASSERT_FLOAT_EQ(id, math::matrix4::identity());
    }
}

TEST(Transform)
{
    auto m = math::make_matrix4(1,   2,  1, 0,
                                3,   2,  2, 0,
                                1,   1,  2, 0,
                                10, 10, -2, 1);
    TEST_ASSERT_FLOAT_NEQ(determinant(m), 0.f);

    auto tm = math::make_transform(1, 2, 1,
                                   3, 2, 2,
                                   1, 1, 2,
                                   10, 10, -2);
    // The transform does exactly as the matrix4
    {
        auto v = V4(4, 1, -8, 1);

        auto w = v * m;
        auto u = tm * v.xyz;

        TEST_ASSERT_FLOAT_EQ(w.x, u.x);
        TEST_ASSERT_FLOAT_EQ(w.y, u.y);
        TEST_ASSERT_FLOAT_EQ(w.z, u.z);
        TEST_ASSERT_FLOAT_EQ(w.w, 1.f);
    }
    // Inverse is correct
    {
        auto inv_m = inverse(m);
        auto inv_tm = inverse(tm);

        UNUSED(inv_m);
        UNUSED(inv_tm);

        auto v = V4(4, 1, -8, 1);

        auto w = v * inv_m;
        auto u = inv_tm * v.xyz;

        TEST_ASSERT_FLOAT_EQ(w.x, u.x);
        TEST_ASSERT_FLOAT_EQ(w.y, u.y);
        TEST_ASSERT_FLOAT_EQ(w.z, u.z);
        TEST_ASSERT_FLOAT_EQ(w.w, 1.f);
    }
}

TEST(ComplexNumbers)
{
    {
        auto a = -1_i;
        auto b = 1 + 1_i;

        auto c = a * b;
        TEST_ASSERT_FLOAT_EQ(c.re,  1.f);
        TEST_ASSERT_FLOAT_EQ(c.im, -1.f);
    }
    {
        auto a = 5 + 2_i;
        auto b = 3 + 0.5_i;

        auto c = a * b;
        TEST_ASSERT_FLOAT_EQ(c.re, 14.f);
        TEST_ASSERT_FLOAT_EQ(c.im, 8.5f);
    }
    {
        auto a = 1.f;
        auto b = 1_i;

        auto c = a * b;
        TEST_ASSERT_FLOAT_EQ(c.re, 0.f);
        TEST_ASSERT_FLOAT_EQ(c.im, 1.f);
    }
}

TEST(Quaternions)
{
    // Right hand rule:
    // Ox -> to the right
    // Oy  ^ to up
    // Oz  . to your face
    // Then rotation around Oy counter clockwise (as it is the natural rotation)
    // V3(1, 0, 0) => V3(0, 0, -1)
    {
        auto q = Q(V3(0, 1, 0), math::to_radians(90));
        auto v = V3(1, 0, 0);

        auto w = math::rotate_by_quaternion(q, v);

        TEST_ASSERT_FLOAT_EQ(w.x,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.y,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.z, -1.f);
    }
    {
        auto q = Q(V3(0, 1, 0), math::to_radians(90));
        auto v = V3(1, 0, 0);

        auto w = math::rotate_by_unit_quaternion(q, v);

        TEST_ASSERT_FLOAT_EQ(w.x,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.y,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.z, -1.f);
    }
}

TEST(SpecialUnitaryMatrices2)
{
    // Test that sigma_x * sigma_x is identity matrix
    {
        auto sigma_x = math::unitary2::sigma_x();
        auto squared = sigma_x * sigma_x;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(1, 0));
    }
    // Test that sigma_y * sigma_y is identity matrix
    {
        auto sigma_y = math::unitary2::sigma_y();
        auto squared = sigma_y * sigma_y;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(1, 0));
    }
    // Test that sigma_z * sigma_z is identity matrix
    {
        auto sigma_z = math::unitary2::sigma_z();
        auto squared = sigma_z * sigma_z;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C(0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(1, 0));
    }
    // Test that (sigma_x * sigma_y) squared is negatie identity
    {
        auto sigma_x = math::unitary2::sigma_x();
        auto sigma_y = math::unitary2::sigma_y();
        auto sigma_xy = sigma_x * sigma_y;
        auto squared = sigma_xy * sigma_xy;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(-1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(-1, 0));
    }
    // Test that (sigma_y * sigma_z) squared is negatie identity
    {
        auto sigma_y = math::unitary2::sigma_y();
        auto sigma_z = math::unitary2::sigma_z();
        auto sigma_yz = sigma_y * sigma_z;
        auto squared = sigma_yz * sigma_yz;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(-1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(-1, 0));
    }
    // Test that (sigma_z * sigma_x) squared is negatie identity
    {
        auto sigma_z = math::unitary2::sigma_z();
        auto sigma_x = math::unitary2::sigma_x();
        auto sigma_zx = sigma_z * sigma_x;
        auto squared = sigma_zx * sigma_zx;

        TEST_ASSERT_FLOAT_EQ(squared._11, C(-1, 0));
        TEST_ASSERT_FLOAT_EQ(squared._12, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._21, C( 0, 0));
        TEST_ASSERT_FLOAT_EQ(squared._22, C(-1, 0));
    }
    // Right hand rule:
    // Ox -> to the right
    // Oy  ^ to up
    // Oz  . to your face
    // Then rotation around Oy counter-clockwise (as it is the natural rotation)
    // means rotating a vector in the XZ plane, which is denoted by `sigma_z * sigma_x`
    // V3(1, 0, 0) => V3(0, 0, -1)
    {
        auto plane_of_rotation = math::unitary2::sigma_z() * math::unitary2::sigma_x();
        auto v = V3(1, 0, 0);
        auto w = math::rotate_by_unitary2(plane_of_rotation, math::to_radians(90), v);

        TEST_ASSERT_FLOAT_EQ(w.x,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.y,  0.f);
        TEST_ASSERT_FLOAT_EQ(w.z, -1.f);
    }
}

TEST(Ui)
{
    usize memory_size = MEGABYTES(1);
    void *memory = malloc(memory_size);
    TEST_ASSERT(memory);

    auto ui_system = ui::system{};
    memory::initialize_memory_arena(&ui_system.ui_allocator, memory_block{memory, memory_size});

    ui::create_root(&ui_system);

    auto group_1 = ui::create_child_group(&ui_system, &ui_system.root);
    group_1->position.xy = V2(100, 100);
    auto shape_1 = ui::create_child_shape(&ui_system, group_1);
    shape_1->position.xy = V2(100, 100);

    auto group_2 = ui::create_child_group(&ui_system, &ui_system.root);
    auto shape_2 = ui::create_child_shape(&ui_system, group_2);

    // @note: This should be applied each frame after update phase, right?
    update_transforms(&ui_system);
    update_transforms_to_root(&ui_system);

    TEST_ASSERT(group_1);
    TEST_ASSERT(shape_1);

    TEST_ASSERT(group_2);
    TEST_ASSERT(shape_2);

    TEST_ASSERT_FLOAT_EQ(shape_1->transform.e[3][0], 100.f);
    TEST_ASSERT_FLOAT_EQ(shape_1->transform.e[3][1], 100.f);

    math::vector3 root_of_shape_1 = shape_1->transform * V3(0);
    math::vector3 root_of_shape_1_in_screen_coords = shape_1->transform_to_root * V3(0);

    TEST_ASSERT_FLOAT_EQ(root_of_shape_1.x, 100.f);
    TEST_ASSERT_FLOAT_EQ(root_of_shape_1.y, 100.f);
    TEST_ASSERT_FLOAT_EQ(root_of_shape_1.z, 0.f);

    TEST_ASSERT_FLOAT_EQ(root_of_shape_1_in_screen_coords.x, 200.f);
    TEST_ASSERT_FLOAT_EQ(root_of_shape_1_in_screen_coords.y, 200.f);
    TEST_ASSERT_FLOAT_EQ(root_of_shape_1_in_screen_coords.z, 0.f);
}


// ----------------------------------------------


int32 main(int32 argc, char **argv, char **env)
{
    TEST_BEGIN();

    TEST_RUN(Matrix2);
    TEST_RUN(Matrix3);
    TEST_RUN(Matrix4);
    TEST_RUN(Transform);
    TEST_RUN(ComplexNumbers);
    TEST_RUN(Quaternions);
    TEST_RUN(SpecialUnitaryMatrices2);
    TEST_RUN(Ui);

    TEST_END();
    return 0;
}


#include <memory/allocator.cpp>
