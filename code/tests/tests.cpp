#include <base.hpp>

#include <stdio.h>

#include <math/complex.hpp>
#include <math/quaternion.hpp>


// @todo:
// - make so that test_##NAME function do not return bool32, it is confusing
// - go through all the test suites even if some are failed
// - collect statistics, how many tests passed, how many failed, how failed ones were called
// - color the failed and passed words in terminal


#define TEST(NAME) void prelude_##NAME() { printf("TEST %s... ", STRINGIFY(NAME)); } bool32 test_##NAME()
#define RUN_TEST(NAME) do { \
    prelude_##NAME(); \
    result = result && test_##NAME(); \
    if (result) { printf("success.\n"); } else { printf("failure!\n"); } \
} while(0)
#define TEST_ASSERT(COND) do if (COND) {} else { return false; } while(0)
#define TEST_ASSERT_EQ(A, B) do if (A == B) {} else { printf("Failure: %s != %s\n", STRINGIFY(A), STRINGIFY(B)); } while(0)

TEST(ComplexNumbers)
{
    {
        auto a = C(0, -1);
        auto b = C(1, 1);

        auto c = a * b;
        TEST_ASSERT_EQ(c.re,  1.f);
        TEST_ASSERT_EQ(c.im, -1.f);
    }

    return true;
}

TEST(Quaternions)
{
    {

    }

    return true;
}

int32 main(int32 argc, char **argv, char **env)
{
    bool32 result = true;
    RUN_TEST(ComplexNumbers);
    if (result)
    {
        printf("Tests: success!\n");
    }
    else
    {
        printf("Tests: failure!\n");
    }
    return result;;
}
