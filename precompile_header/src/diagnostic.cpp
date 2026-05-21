#include "diagnostic.h"
#include <iomanip>
#include <cmath>
Diagnostic DiagnosticCollector::collect(Real t, const Grid2D& f, const Field1D& E) const{
    Diagnostic d{};
    d.time =t;
    int Nx = g_.Nx();
    int Nv = g_.Nv();
    Real dx = g_.dx();
    Real dv = g_.dv();
    Real Ek =0.0;
    Real l2 = 0.0;
    Real S = 0.0;
    
}