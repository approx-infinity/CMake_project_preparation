#include "distribution.h"
namespace Distribution{
    Field1D maxwellian(const PhaseGrid& g){
        Field1D f0(g.Nv());
        const Real inv_sqrt2pi = 1.0 / std::sqrt(Constants::two_pi);
        for(int j = 0; j< g.Nv(); j++){
            Real vj = g.v(j);
            f0[j] = inv_sqrt2pi * std::exp(-0.5*vj *vj);
        }
        return f0;
    }
    Grid2D landauDamping(const PhaseGrid& g, Real epsilon, Real k){
        Grid2D f = g.allocateF();
        Field1D f0 = maxwellian(g);
        for (int i = 0; i<g.Nx(); ++i){
            Real xi = g.x(i);
            Real perturbation = 1.0 + epsilon * std::cos(k*xi);
            for(int j =0; j<g.Nv(); ++j){
                f[i][j] = f0[j] * perturbation;
            }
        }
        return f;
    }
    Grid2D twostream(const PhaseGrid& g, Real vBeam, Real epsilon){
        Grid2D f = g.allocateF();
        const Real inv_sqrt2pi = 1.0 / std::sqrt(Constants::two_pi);
        const Real sigma = 1.0;
        for(int i = 0; i<g.Nx(); ++i){
            Real xi = g.x(i);
            Real perturbation = 1.0 + epsilon * std::cos(0.5 * xi);
            for(int j=0; j<g.Nv(); ++j){
                Real vj  = g.v(j);
                Real beam1 = std::exp(-0.5 * ((vj - vBeam)/sigma)*((vj - vBeam)/sigma));
                Real beam2 = std::exp(-0.5 * ((vj + vBeam)/sigma)*((vj +vBeam)/sigma));
                f[i][j] = 0.5 * inv_sqrt2pi /sigma*(beam1+beam2)*perturbation;
            }
        }
    return f;
    }
}