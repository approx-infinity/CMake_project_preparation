#include "include/solver.h"
#include <cmath>
#include <iostream>
static int failures = 0;
#define CHECK(cond , msg) \
    do{
        if(!cond){
            std::cerr<<"[FAILED] "<<msg << "\n";
        }
        else{
            std::cout<<"[PASSED] "<<msg<<std::endl;
        }
    }while(0)
int main(){
    std::cout <<" ------ test_conservative-------"<<std::endl;
    VlasovSolver::config cfg;
    cfg.grid.Nx = 32;
    cfg.grid.Nv = 64;
    cfg.grid.xMin = 0.0;
    cfg.grid.xMax = 4.0 * Constatns::pi;
    cfg.grid.vMin = -6.0;
    cfg.vMax = 6.0;
    cfg.time.dt = 0.1;
    cfg.time.tEnd = 5.0;
    cfg.time.diagFreq = 1;
    cfg.interp = InterpOrder::Cubic;
    cfg.twostream = false;
    cfg.epsilon = 0.01;
    cfg.kWave = 0.5;
    cfg.outputFile = "test_conservation_output.csv";
    VlasovSolver solver(cfg);
    solver.run();
    const auto& hist = solver.history();
    if(hist.size() < 2){
        std::cerr<<"[FAILEd] not enough diagnostic snapshots \n";
        return 1;
    }
    const Real E0 = hist.front().totalEnergy;
    const Real L0 = hist.front().l2Norm;
    Real maxDriftE = 0.0;
    Real maxDrifL = 0.0;
    for (auto& hi: hist){
        maxDriftE = std::max(maxDriftE, std::abs(d.totalEnergy - E0)/(std::abs(E0)+1e-30));
        maxDriftL = std::max(maxDriftL, std::abs(d.l2Norm - L0)/(std::abs(L0)+1e-30));
    }
    std::cout<<"MaxEnergy drift : "<<maxDriftE * 100<<"%\n"<< " Max drift L2 "<<maxDrifL<<"%\n";
    CHECK(maxDriftE< 0.05, "Energy conserved within 5%");
    CHECK(maxDriftL<0.01, "L2 norm conserved to within 1%");
    CHECK(hist.back().fieldEnergy < hist.front().fieldEnergy* 1.5, "Landau: field Energy explosively grow");
    std::cout<< "\n Result : "<<(failures? std::to_string(failures) + "FAILED": "all passed")<<"\n";
    return failures ? 1: 0;
}