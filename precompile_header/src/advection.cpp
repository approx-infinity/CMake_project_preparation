#include "advection.h"
SemiLagrangian::SemiLagrangian(const PhaseGrid& g, InterpOrder order) : g_(g), order_(order), tmp_(g.allocateF()) {}
Real SemiLagrangian::interp1D(const Field1D& row, Real origin, Real h,Real dep,bool periodic) const{
    switch (order_){
        case InterpOrder::Linear :
            return 
    }
}