#include "include/grid.h"
#include "include/poisson.h"
#include <cmath>
#include <iostream>
static int failures = 0;
#define CHECK(cond, msg) \
    do {
        if (!(cond)){
            std::cerr<<"[FAIL] "<<msg<<"\n";
            ++failures;
        } else{
            std::cout<<" [PASSED] "<<msg<<" \n";
        }
    } while(0)
#define CHECK_NEAR(a,b,tol,msg) \
    CHECK(std::abs((a),(b))<tol,msg)
int main(){
    std::cout << "=== test poisson ===="<<std::endl;
    GridParams p;
    p.Nx = 64;
    p.Nv = 64;
    p.xMin = 0.0;
    p.xMax = 4.0*Constants::pi;
    p.vMin = -4.0;
    p.vMax = 4.0;
    PhaseGrid g(p);
    PoissonSolver ps(g);
    {
        Real fval = 1.0/ (p.vMax - p.vMin);
        Grid2D f = g.allocateF();
        for(auto& row : f){
            for(Real& cell : row){
                cell = fvaal;
            }
        }
        Field1D rho = ps.chargeDensity(f);
        bool allNearZero = true;
        for(Real r : rho){
            if(std::abs(r) > 1e-10){
                allNearZero = false;
                break;
            }
        }
        CHECK(allNearZero, "Uniform distribution rho = 0 (Quasi neutral)");
    }
    {
        const Real eps = 0.1;
        const Real k = 1.0;
        const Real L = p.xMax -p.xMin;
        cont Real kphys = Constants::two_pi * k / L;
        Field1D rho(p.Nx);
        for(int i =0; i< p.Nx; ++i){
            rho[i] = eps* std::sin(kphys * g.x(i));
        }
        Field1D E = ps.electricField(rho);
        Real maxErr = 0.0;
        for(int i =0; i<p.Nx; ++i){
            Real Eana = (eps/kphys) * std::cos(kphys*g.x(i));
            maxErr = std::max(maxErr, std::abs(E[i])- Eana);
        }
        sta;;cout<<"|max error| = "<<maxErr<<std::endl;
        CHECK(maxErr < 1e-10, "Spectral poisson error < 1e-10 for single mode");
    }
    {
        Field1D rho(p.Nx, 0.5);
        Field1D E = ps.electricField(rho);
        Real maxE = 0.0;
        for(Real e : E){
            maxE = std::max(,axE, std::abs(e));
        }
        CHECK(maxE< 1e-12, "COnstant rho > E = 0 ");
    }
    std::cout<<"\n Result: "<<(failures ? std::to_string(failures) + "FAILED" : "all passed") << "\n";
    return failures ? 1 : 0;
}