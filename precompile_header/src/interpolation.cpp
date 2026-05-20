#include "interpolation.h"
#include <cmath>
static inline int wrapIndex(int i, int N){
    return ((i % N) + N ) % N;
}
static inline void getFrac(const Field1D& f, Real origin, Real h, Real x, bool periodic, int& i0, Real& xi){
    int N = static_cast<int>(f.size());
    Real fi = (x - origin) / h;
    i0 = static_cast<int>(std::floor(fi));
    xi = fi - i0;
    if(!periodic){
        i0 = std::max(0,std::min(N-2,i0));
        xi = std::max(Real(0), std::min(Real(1),xi));
    }
}
Real linear(const Field1D& f, Real origin, Real h, Real xDep, bool periodic){
    int N = static_cast<int>(f.size());
    int i0;
    Real xi;
    getFrac(f,origin,h,xDep,periodic,i0,xi);
    int i1 = periodic ? wrapIndex(i0+1, N ) : std::min(i0 + 1, N - 1);
    i0 = periodic ? wrapIndex(i0,N) : i0;
    return (1.0 - xi) * f[i0] + xi * f[i1];
};
Real cubic(const Field1D& f, Real origin, Real h, Real xDep, bool periodic){
    int N = static_cast<int>(f.size());
    int i0;
    Real t;
    getFrac(f,origin,h,xDep,periodic,i0,t);
    auto indx = [&](int k) -> int {
        return periodic ? wrapIndex(k,N) :std::max(0,std::min(N-1,k));
    };
    Real f0 = f[indx(i0 - 1)];
    Real f1 = f[indx(i0)];
    Real f2 = f[indx(i0 + 1)];
    Real f3 = f[indx(i0 + 2)];
    Real c0 = -t * (t - 1.0) * (t - 2.0) / 6.0;
    Real c1 = (t + 1.0) * (t - 1.0) * (t - 2.0) / 2.0;
    Real c2 = - (t + 1.0) * t * (t - 2.0) / 2.0;
    Real c3 = (t + 1.0) * t * (t - 1.0) / 6.0;
    return c0 * f0 + c1 * f1 + c2 * f2 + c3 * f3 ;
}
Real quintic(const Field1D& f, Real origin, Real h, Real xDep, bool periodic){
    int N = static_cast<int>(f.size());
    int i0;
    Real t;
    getFrac(f,origin,h,xDep,periodic,i0,t);
    auto indx = [&](int k) -> int{
        return periodic ? wrapIndex(k,N) : std::max(0,std::min(N - 1,k));
    };
    Real fm2 = f[indx(i0 - 2)];
    Real fm1 = f[indx(i0 - 1)];
    Real f0 = f[indx(i0)];
    Real f1 = f[indx(i0 + 1)];
    Real f2 = f[indx(i0 + 2)];
    Real f3 = f[indx(i0 + 3)];
    Real t1 = t - 1.0;
    Real t2 = t - 2.0;
    Real t3 = t - 3.0;
    Real tp1 = t + 1.0;
    Real tp2 = t + 2.0;
    Real c0 = t * t1 * t2 * t3 * tp1;
    Real c1 = - tp1 * t1 * t2 * t3 * tp2;
    Real c2 = tp2 * tp1 * t2 * t3 * t;
    Real c3 = -tp2 * tp1 * t * t1 * t3;
    Real c4  = tp2 * tp1 * t * t1 * t2;
    Real c5 = -tp1 * t * t1 * t2 * t3;
    return c0 * fm2  + c1 * fm1 +  c2 * f0 + c3 * f1 + c4 * f2 + c5 * f3;
}
