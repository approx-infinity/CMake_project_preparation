#pragma once
#include "plasma_type.h"
#include "grid.h"
class PoissonSolver {
    public:
        explicit PoissonSolver(const PhaseGrid& g);
        Field1D chargeDensity(const Grid2D& f) const;
        Field1D electricField(const Field1D& rho) const;
    private:
        const PhaseGrid& g_;
        void dft(Field1D& re, Field1D& im, bool inverse) const;
};