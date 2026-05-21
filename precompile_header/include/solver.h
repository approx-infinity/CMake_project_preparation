#pragma once 
#include "plasma_type.h"
#include "grid.h"
#include "poisson.h"
#include "advection.h"
#include "diagnostic.h"
#include <string>
class VlasovSolver{
    public:
        struct Config{
            GridParams grid;
            TimeParams time;
            InterpOrder interp = InterpOrder::Cubic;
            bool twostream = false;
            Real epsilon = false;
            Real kWave = 0.5;
            Real vBeam = 2.4;
            std::string outputFile = "diagnostic.csv";
        };
        explicit VlasovSolver(const Config& cfg);
        void run();
        const std::vector<Diagnostic>& history() const{
            return history_;
        }
    private:
        Config cfg_;
        PhaseGrid grid_;
        PoissonSolver poisson_;
        SemiLagrangian advect_;
        DiagnosticsCollector diag_;
        Grid2D f_;
        Field1D E_;
        std::vector<Diagnostic> history_;
};