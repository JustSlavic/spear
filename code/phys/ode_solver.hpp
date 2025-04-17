#ifndef PHYS_ODE_SOLVER
#define PHYS_ODE_SOLVER

#include <base.h>


namespace phys {

typedef void (*dydt_f)(float32 *in, float32 *out, float32 *f, float32 dt);

void solve_ode(float32 *in, float32 *out, float32 *forces, uint32 count, uint32 stride, dydt_f dydt, float32 dt);

} // namespace phys


#endif // PHYS_ODE_SOLVER
