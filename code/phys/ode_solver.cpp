#include "ode_solver.hpp"

namespace phys {

void solve_ode(float32 *in, float32 *out, uint32 count, uint32 stride, dydt_f dydt, float32 dt)
{
    for (uint32 body_index = 0; body_index < count; body_index++)
    {
        dydt(in + body_index*stride, out + body_index*stride, dt);
    }
}

} // namespace phys
