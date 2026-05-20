#include "grid.h"
#include <stdexcept>

PhaseGrid::PhaseGrid(const GridParams& p) : p_(p) {
    if (p_.Nx <= 0 || p_.Nv <= 0){
        throw std::invalid_argument("Grid dimension must be positive! ");
    }
    if(p_.xMin <=0 || p_.vMin <= 0){
        throw std::invalid_argument("x minimum value or  v minimumvalue must be positive! ");
    }
    if(p_.xMin>= p_.xMax){
        throw std::invalid_argument("x min < x max");
    }
    if(p_.vMin >= p_.vMax){
        throw std::invalid_argument("v min < v max");
    }
}

Grid2D PhaseGrid::allocateF() const {
    return Grid2D(p_.Nx, Field1D(p_.Nv,Real(0)));
}