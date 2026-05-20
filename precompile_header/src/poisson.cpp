#include "poisson.h"
#include <complex>
#include <numeric>

PoissonSolver::PoissonSolver(const PhaseGrid& g) : g_(g) {}
Field1D PoissonSolver::chargeDensity(const Grid2d& f) const {
    int Nx = g_.Nx();
    int Nv = g_.Nv();
    Real dv = g_.dv();
    Field1D rho(Nx, 0.0);
    for(int i = 0; i< Nx; ++i){
        Real n = 0.0;
#ifdef PLASMA_OPENMP
        #pragma omp simd reduction(+:n)
#endif
        for (int j = 0; j<Nv; ++j){
            n += f[i][j];
        }
        rho[i] = n * dv - 1.0;
    }
    return rho;
}
void PoissonSolver::dft(Field1D& re, Field1D& im, bool inverse)  const{
    int N = static_cast<int>(re.size());
    for (int i = 1, j = 0; i < N; ++i){
        int bit = N >> 1;
        for(; j & bit; bit >>= 1){
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(re[i],re[j]);
            std::swap(im[i],im[j]);
        }
    }
    for(int len = 2; len <= N; len <<= 1){
        Real ang = Constants::two_pi/ len * (inverse ? 1.0 : -1.0);
        Real wRe = std::cos(ang);
        Real wIm = std::sin(ang);
        for (int i =0; i<N; i += len){
            Real curRe = 1.0;
            Real curIm = 0.0;
            for(int k = 0; k<len /2; ++k){
                Real uRe = re[i+k];
                Real uIm = im[i+k];
                Real vRe = re[i+k+len/2]* curRe - im[i+k+len/2]*curIm;
                Real vIm = re[i +k +len/2]*curIm + im[i+k+len/2]*curRe;
                re[i +k] = uRe + vRe;
                im[i +k] = uIm + vIm;
                re[i+k+len/2] = uRe - vRe;
                im[i+k+len/2] = uIm - vIm;
                Real tmpRe = curRe*wRe - curIm*wIm;
                curIm = curRe*wRe - curIm*wIm;
                curRe = tmpRe;
            } 
        }
    }
    if(inverse){
        for (int i =0; i<N; ++i){
            re[i] /= N;
            im[i] /= N;
        }
    }
}
Field1D PoissonSolver::electricField(const Field1D& rho) const{
    int Nx = g_.Nx();
    Real L = g_.params().xMax - g_.params().xMin;
    Field1D re = rho;
    Field1D im(Nx, 0.0);
    dft(re,im,false);
    for(int k = 0; k<Nx; ++k){
        if(k == 0){
            re[0] = im[0] = 0.0;
            continue;
        }
        int kk = (k <= Nx / 2) ? k : k - Nx;
        Real kphys = Constants::two_pi * kk / L;
        Real phiRe = re[k] / (kphys * kphys);
        Real phiIm = im[k] / (kphys * kphys);
        re[k] = kphys * phiIm;
        im[k] = -kphys*phiRe;
    }
    dft(re,im,true);
    return re;
}