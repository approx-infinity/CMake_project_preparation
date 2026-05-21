#include "advection.h"
SemiLagrangian::SemiLagrangian(const PhaseGrid& g, InterpOrder order) : g_(g), order_(order), tmp_(g.allocateF()) {}
Real SemiLagrangian::interp1D(const Field1D& row, Real origin, Real h,Real dep,bool periodic) const{
    switch (order_){
        case InterpOrder::Linear : Interp::linear(row,origin,h,dep,periodic);
        case InterpOrder::Quintic : Interp::quantic(row, origin,h,dep,periodic);
        default : Interp::cubic(row,origin,h,dep,periodic);
    }
}
void SemiLagrangian::xSweep(Grid2D& f, Real dtHalf){
    int Nx = g_.Nx();
    int Nv = g_.Nv();
    Real dx = g_.dx();
#ifdef PLASMA_OPENMP 
    #pragma omp parallel for schedule(static)
#endif

    for(int j = 0; j<Nv; ++j){
        Real vj = g_.v(j);
        Field1D col(Nx);
        for(int i=0; i<Nx; ++i){
            col[i] = f[i][j];
        }
        for(int i=0; i<Nx; ++i){
            Real xi = g_.x(i);
            Real dep = xi - vj * dtHalf;
            tmp_[i][j] = interp1D(col,g_.xMin(),dx,dep,/*periodic=*/true);
        }
    }
    std::swap(f,tmp_);
}
void SemiLagrangian::vSweep(Grid2D& f, const Field1D& E, Real dt){
    int Nx = g_.Nx();
    int Nv = g_.Nv();
    Real dv = g_.dv();
#ifdef PLASMA_OPENMP
    #pragma omp parallel for schedule(static)
#endif
    for(int i =0; i<Nx; ++i){
        Real Ei = E[i];
        for(int j = 0; j<Nv; ++j){
            Real vj = g_.v(j);
            Real dep = vj - Ei * dt;
            tmp_[i][j] = interp1D(f[i],g_.vMin(),dv,dep, /*periodic=*/true);
        }
    }
    std::swap(f,tmp_);
}
void SemiLagrangian::step(Grid2D& f, const Field1D& E, Real dt){
    xSweep(f,dt*0.5);
    vSweep(f,E,dt);
    xSweep(f,dt * 0.5);
}