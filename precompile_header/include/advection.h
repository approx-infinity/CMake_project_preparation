#include "plasma_type.h"
#include "grid.h"
#include "interpolation.h"
enum class InterpOrder {Linear, Cubic, Quintic};
class SemiLagrangian{
    public:
        SemiLagrangian(const PhaseGrid& g, InterpOrder order = InterpOrder::Cubic);
        void step(Grid2D& f, const Field1D& E, Real dt);
    private:
        void xSweep(Grid2D& f, Real dtHalf);
        void vSweep(Grid2D& f, const Field1D& E, Real dtHalf);
        Real interp1D(const Field1D& row, Real origin, Real h, Real dep, bool periodic) const;
        const PhaseGrid& g_;
        InterpOrder order_;
        Grid2D tmp_;

    };