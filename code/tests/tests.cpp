#include <base.hpp>

#include <stdio.h>

#include <math/complex.hpp>
#include <math/quaternion.hpp>


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

#define TEST_ASSERT_FLOAT_EQ(A, B) \
    do { \
        if (::math::equal(A, B)) { } else { \
            printf("\n%s:%d FAIL (%s != %s)\n", __FILE__, __LINE__, STRINGIFY(A), STRINGIFY(B)); \
            *success = false; \
            return; \
        } \
    } while (0)

TEST(ComplexNumbers)
{
    {
        auto a = C(0, -1);
        auto b = C(1, 1);

        auto c = a * b;
        TEST_ASSERT_FLOAT_EQ(c.re,  1.f);
        TEST_ASSERT_FLOAT_EQ(c.im, -1.f);
    }
    {
        auto a = C(5, 0);
        auto b = C(3, 0);

        auto c = a * b;
        TEST_ASSERT_FLOAT_EQ(c.re, 15.f);
        TEST_ASSERT_FLOAT_EQ(c.im, 0.f);
    }
    {
        auto a = math::complex::r();
        auto b = math::complex::i();

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

int32 main(int32 argc, char **argv, char **env)
{
    TEST_BEGIN();

    TEST_RUN(ComplexNumbers);
    TEST_RUN(Quaternions);

    TEST_END();
    return 0;
}
