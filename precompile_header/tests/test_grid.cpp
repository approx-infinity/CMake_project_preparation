#include "include/grid.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
static int failures = 0;
#define CHECK(cond , msg) \
    do {
        if(!(cond)){
            std::cerr<<"[FATAL] "<<msg<<"\n";
            failures++;
        } else{
            std::cout<<"[PASS] "<<msg<<"\n";
        }
    }while(0)
#define CHECK_NEAR(a,b,tol,msg) \
    CHECK(std::abs((a)-(b)) < (tol),msg)

int main(){
    std::cout<<" ======= test grid ======="<<std::endl;
    GridParams p;
    p.Nx =64;
    p.Nv = 128;
    p.xMin = 0.0;
    p.xMax = 4.0 * Constants::pi;
    p.vMin = -6.0;
    p.vMax = 6.0;
    PhaseGrid g(p);
    CHECK(g.Nx() == 64, "Nx == 64");
    CHECK(g.Nv() == 128 , "g.Nv() == 128");
    Real expectdx = (4.0 * Constants::pi)/64.0;
    Real expectdv = 12.0/128.0;
    CHECK_NEAR(g.dx(), expectdx, 1e-12,"dx correct");
    CHECK_NEAR(g.dv(),expectdv, 1e-12,"dv correct");
    CHECK_NEAR(g.x(0),p.xMin + 0.5 * g.dx(), 1e-12,"x(0) = xMin + dx/2");
    CHECK_NEAR(g.x(63),p.xMax - 0.5 * g.dx(), 1e-12, "x(63) = xMax - dx/2");
    CHECK_NEAR(g.v(0), p.vMin + 0.5 * g.dv(), 1e-12, "v(0) = vMin + dv/2");

    Grid2D f = g.allocateF();
    CHECK(static_cast<int>(f.size()) == 64,"f has Nx rows");
    CHECK(static_cast<int>(f[0].size()) == 128, "Nv has 128 col");
    bool allzero = true;
    for(const auto& row:f){
        for(Real v: row){
            if(v != 0.0){
                allzero = false;
                break;
            }
        }
    }
    CHECK(allzero, "allocateF() initialises to zero");
    bool threw = false;
    try {
    GridParams bad;
    bad.Nx = -1;
    PhaseGrid gbad(bad);
    } catch(const std::invalid_argument&){
        threw= true;
    }
    CHECK(threw, "Negative Nx throws invalid_argument");
    std::cout<<"\n Result "<<(failures ? std::to_string(failures) + "Failed":"all passed")<<"\n";
    return failures ? 1: 0;
}
