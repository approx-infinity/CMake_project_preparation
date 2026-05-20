#pragma once
#include "plasma_type.h"
namespace Interp {
    Real linear(const Field1D& f, Real origin, Real h, Real xDep,bool periodic = true);
    Real cubic(const Field1D& f, Real origin, Real h, Real xDep, bool periodic = true);
    Real quantic(const Field1D& f, Real origin, Real h, Real xDep,bool periodic = true);
}