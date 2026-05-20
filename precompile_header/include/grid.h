#include "plasma_type.h"
class PhaseGrid{
    public:
        explicit PhaseGrid(const GridParams& p);

        int Nx() const {
            return p_.Nx;
        }
        int Nv() const {
            return p_.Nv;
        }
        Real dx() const {
            return p_.dx();
        }
        Real dv() const {
            return p_.dv();
        }
        Real xMin() const {
            return p_.xMin;
        }
        Real vMin() const {
            return p_.vMin;
        }
        Real x(int i) const {
            return p_.xMin + (i + 0.5) * p_.dx();
        }
        Real v(int j) const {
            return p_.vMin + (j + 0.5) * p_.dv();
        }
        Grid2D allocateF() const;
        const GridParams& params() const {
            return p_;
        }
    private:
        GridParams p_;
};