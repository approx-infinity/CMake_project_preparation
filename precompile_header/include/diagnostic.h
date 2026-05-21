#pragma once
#include "plasma_type.h"
#include "grid.h"
struct Diagnostic{
    Real time;
    Real kineticEnergy;
    Real totalEnergy;
    Real l2Norm;
    Real entropy;
    Real maxE;
};
class DiagnosticsCollector{
    public:
        explicit DiagnosticsCollector(const PhaseGrid& g);
        Diagnostic collect(Real t, const Grid2D& f, const Field1D& E) const;
        void print(const Diagnostic& d) const;
        void writeCsv(const std::string& filename,const std::vector<Diagnostic>& history) const;
    private:
        const PhaseGrid& g_;
}