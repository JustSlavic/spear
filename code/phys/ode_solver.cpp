#include "ode_solver.hpp"

namespace phys {

void solve_ode(float32 *in, float32 *out, uint32 count, uint32 stride, dydt_f dydt, float32 dt)
{
    ASSERT(count % stride == 0);

    for (uint32 i = 0; i < count; i += stride)
    {
        dydt(in + i, out + i, dt);
    }
}

} // namespace phys
