#include "include/interpolation.h"
#include <cmath>
#include <iostream>
static int failures = 0;
#define CHECK(cond, msg) \
    do {
        if(!(cond)){
            std::cerr<<"[FAIL]"<<msg<<"\n";
            failures++;
        } else{
            std::cout<<"[PASSED]"<<msg<<"\n";
        }
    }while(0)

int main() {
    std::cout<<" ----- test_interp ----"<<std::endl;
    const int N = 64;
    const Real h = Constants::two_pi / N;
    const Real org = 0.0;
    Field1D f(N);
    for (int i =0; i<N; ++i){
        f[i] = std::sin(org + i * h);
    }
    struct TestPoint {
        Real x;
        Real exact;
    };
    std::vector<TestPoint> pts;
    for(int q = 0; q<20; ++q){
        Real x = 0.37 + q * 0.29;
        pts.push_back({x,std::sin(x)});
    }
    Real maxErrLin = 0.0;
    Real maxErrCub = 0.0;
    Real maxErrQui = 0.0;
    for(auto& pt: pts){
        Real v1 = Interp::linear(f,org,h, pt.x,true);
        Real vc = Interp::cubic(f,org,h,pt.x,true);
        Real vq = Interp::quintic(f,org,h,pt.x,true);
        maxErrLin = std::max(maxErrLin, std::abs(v1 - pt.exact));
        maxErrCub = std::max(maxErrCub, std::abs(vc - pt.exact));
        maxErrQui = std::max(maxErrQui, std::abs(vq - pt.exact));
    }
    std::cout<< "MaxLin " <<maxErrLin<<" MaxErrCub "<<maxErrCub<<" MaxErrQui "<<maxErrQui;
    CHECK(maxErrLin < 1e-02, "Linear interpolation error < 1e-02");
    CHECK(maxErrCub < 1e-02, "Cube interpolation error < 1e-02");
    CHECK(maxErrQui < 1e-02, "Quintic interpolation error < 1e-02");
    Real maxOnGrid = 0.0;
    for(int i =0; i<N; ++i){
        Real x = org + i*h;
        Real val = Interp::cubic(f,org,h,x,true);
        maxOnGrid = std::max(maxOnGrid, std::abs(val - f[i]));
    }
    CHECK(maxOnGrid< 1e-12,"Cubic on grid is very exact");
    std::cout<<"\n Result "<<(failures?std::to_string(failures) + "Failed":"all passed") <<"\n";
    return failures ? 1: 0;
}