#include "world.hpp"

#define PHYS_DT (1.0 / 120.0)


namespace phys {

/*
            | y^2 + z^2    -yx       -zx    |
    I = int(| -xy       x^2 + z^2    -zy    |)
            | -xz          -yz    x^2 + y^2 |

               | b^2 + c^2     0         0     |
    I = 1/12 V |     0     a^2 + c^2     0     |
               |     0         0     a^2 + b^2 |
*/

matrix3 make_inertia_tensor(float64 a, float64 b, float64 c)
{
    matrix3 I = {};
    I._11 = b*b + c*c;
    I._22 = a*a + c*c;
    I._33 = a*a + b*b;

    float32 V = a*b*c;
    return (1.0f/12.0f) * V * I;
}

void simulate_step(phys::world *world)
{
    for (int body_index = 0; body_index < world->body_count; body_index++)
    {
        body *b = world->bodies + body_index;
        b->X += PHYS_DT * b->P / b->M;

        matrix3 R = to_matrix3(b->Q);

        float32 norm_ = norm(b->Q);
        console::print("norm = %f\n", norm_);
        float32 det = determinant(R);
        console::print("det = %f\n", det);

        matrix3 I0 = make_inertia_tensor(2.5f, 5.0f, 0.5f);

        matrix3 invI0 = I0;
        invI0._11 = 1.0/invI0._11;
        invI0._22 = 1.0/invI0._22;
        invI0._33 = 1.0/invI0._33;

        console::print("I0 = |%f, %f, %f|\n"
                       "     |%f, %f, %f|\n"
                       "     |%f, %f, %f|\n",
            I0._11, I0._12, I0._13,
            I0._21, I0._22, I0._23,
            I0._31, I0._32, I0._33);
        console::print("invI0 = |%f, %f, %f|\n"
                       "        |%f, %f, %f|\n"
                       "        |%f, %f, %f|\n",
            invI0._11, invI0._12, invI0._13,
            invI0._21, invI0._22, invI0._23,
            invI0._31, invI0._32, invI0._33);

        // transpose(invI0);
        // invI0._1 = apply_unit_quaternion(b->Q, invI0._1);
        // invI0._2 = apply_unit_quaternion(b->Q, invI0._2);
        // invI0._3 = apply_unit_quaternion(b->Q, invI0._3);
        // transpose(invI0);

        vector3 omega = R * invI0 * transposed(R) * b->L;
        quaternion omega_q = {};
        omega_q.axis = omega;
        b->Q += 0.5f * omega_q * b->Q * PHYS_DT;

        printf("Q = (%f, %f, %f, %f) ", b->Q._1, b->Q._e32, b->Q._e13, b->Q._e21);
        normalize(b->Q);
        printf("and normalized(%f, %f, %f, %f);\n", b->Q._1, b->Q._e32, b->Q._e13, b->Q._e21);
    }
}

void update_world(phys::world *world, float32 dt)
{
    world->residual_time += dt;
    while (world->residual_time >= PHYS_DT)
    {
        simulate_step(world);
        world->residual_time -= PHYS_DT;
    }
}

} // namespace phys
