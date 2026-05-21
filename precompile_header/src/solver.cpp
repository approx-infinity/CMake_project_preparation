#include "solver.h"
#include "distribution.h"
#include <iostream>
VlasovSolver::VlasovSolver(const Config& cfg) : cfg_(cfg), grid_(cfg.grid),poisson_(grid_), advect(grid_,cfg.interp), diag_(grid_),f_(grid_.allocateF()),E_(cfg.grid.Nx,0.0) {
    if(cfg_.twostream ){
        f_= Distribution::twostream(grid_,cfg_.vBeam,cfg_.epsilon);
    } else {
        f_ = Distribution::landaudamping(grid_,cfg_.epsilon,cfg_kWave);
    }
    Field1D rho = poisson_.chargeDensity(f_);
    E_ = poisson_.electricField(rho);
#ifdef PLASMA_VERBOSE
    std::cout<<"[Plasma ValsovPoisson] Initialization: with "<<cfg_.grid.Nx<<" Number of point "<<cfg_.grid.Nv <<"Number of points "<<cfg_.time.dt<<" dt steps , "<<cfg_.time.tEnd<<" time end"<<"\n";
#endif
}
void VlasovSolver::run(){
    const TimeParams& tp = cfg_.time;
    int nSteps = static_cast<int>(tp.tEnd /tp.dt);
    std::cout<<"---------------------------------------------\n"<<"SemiLagrange Vlasov-Poisson Solver\n"<<"Mode: "<<(cfg_.twostream ? "Two-Stream ": "Landau Damping")<<"\n"<<"Nx = "<<cfg_.grid.Nx<<" Nv = "<<cfg_.grid.Nv<<"dt = "<<tp.dt << "nSteps = "<<nSteps<<"\n"<<"-------------------------------------------------------" <<"\n";

    Real t = 0.0;
    for(int n= 0; n<=nSteps; ++n){
        if (n % tp.diagFreq == 0){
            Diagnostic d = diag_.collect(t,f_,E_);
            history_.push_back(d);
            diag_.print(d);
        }
        if (n == nSteps){
            break;
        }
        advect_.step(f_,E_,tp.dt);
        Field1D rho = poisson_.chargeDensity(f_);
        E_ = poisson_.electricField(rho);
        t += tp.dt;
    }
    diag_.writeCsv(cfg_.outputFile, history_);
    std::cout<<"\n Done! DIagnostics written to "<<cfg_.outputFile <<"\n";
}
