#include "plasma_type.h"
#include "grid.h"
namespace Distribution{
    Field1D maxwellian(const PhaseGrid& g);
    Grid2D landaudamping(const PhaseGrid& g,Real epsilon = 0.01, Real k = 0.5);
    Grid2D twostream(const PhaseGrid& g, Real vBeam = 2.4, Real epsilon = 0.001);
}