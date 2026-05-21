#include "diagnostic.h"
#include <iomanip>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
DiagnosticsCollector::DiagnosticsCollector(const PhaseGrid& g) : g_(g) {}
Diagnostic DiagnosticsCollector::collect(Real t, const Grid2D& f, const Field1D& E) const{
    Diagnostic d{};
    d.time =t;
    int Nx = g_.Nx();
    int Nv = g_.Nv();
    Real dx = g_.dx();
    Real dv = g_.dv();
    Real Ek =0.0;
    Real l2 = 0.0;
    Real S = 0.0;
    for(int i =0; i<Nx; ++i){
        for(int j =0; j<Nv; ++j){
            Real fij = f[i][j];
            Real vj = g_.v(j);
            Ek += 0.5* vj * vj * fij;
            l2 += fij * fij;
            if (fij > 1e-30){
                S -= fij * std::log(fij);
            }
        }
    }
    d.kineticEnergy = Ek * dx * dv;
    d.l2Norm = l2 * dx * dv;
    d.entropy = S * dx* dv;
    Real Ef = 0.0;
    d.maxE= 0.0;
    for(int i =0; i<Nx; ++i){
        Ef += 0.5 * E[i] * E[i];
        d.maxE = std::max(d.maxE,std::abs(E[i]));
    }
    d.fieldEnergy = Ef * dx;
    d.totalEnergy = d.kineticEnergy + d.fieldEnergy;
    return d;
    
}
void DiagnosticsCollector::print(const Diagnostic& d) const {
    std::cout<< std::fixed << std::setprecision(5) << "t = "<<std::setw(8)<<d.time<<" E total = "<<std::setw(12)<<d.totalEnergy << "Ekin= "<<std::setw(12)<<d.kineticEnergy<<"Efield = "<<std::setw(12)<<d.fieldEnergy<<"|E| max = "<<std::setw(10)<<d.maxE<<" L2 = "<<d.l2Norm<<"\n";
}

void DiagnosticsCollector::writeCsv(const std::string& filename, const std::vector<Diagnostic>& history) const {
    std::ofstream ofs(filename);
    if (!ofs){
        throw std::runtime_error("cannot open file : "+ filename);
    }
    ofs << "time, kineticEnergy , fieldEnergy, totalEnergy, l2Norm ,entropy,maxE\n";
    ofs<<std::scientific<<std::setprecision(10);
    for(const auto& d : history){
        ofs<<d.time<<","<<d.kineticEnergy<<","<<d.fieldEnergy<<","<<d.totalEnergy<<","<<d.l2Norm<<","<<d.entropy<<","<<d.maxE<<"\n";
    }
}