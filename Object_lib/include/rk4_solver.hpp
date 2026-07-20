#ifndef RK4_SOLVER_HPP
#define RK4_SOLVER_HPP
#include <functional>
typedef std::function<double(double, double)> DerivativeFunction;
double solve_rk4_step(DerivativeFunction f, double t, double y, double h);
#endif
