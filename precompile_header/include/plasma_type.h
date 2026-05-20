#include <cstdint>
#include <cmath>
#include <array>
#include <vector>

using Real = double;
using Index = std::ptrdiff_t;
namespace Constants{
    inline constexpr Real pi = 3.141598675556555434;
    inline constexpr Real two_pi = 2 * pi;
    inline constexpr Real eps0 = 1.0;
    inline constexpr Real q0 = 1.0;
    inline constexpr Real me = 1.0;
}

struct Dim1V1 {
    static constexpr int Nx = 1;
    static constexpr int Nv = 1;
};
struct GridParams{
    int Nx = 64;
    Real xMin = 0.0;
    Real xMax = 4.0 * Constants::pi;
    int Nv = 128;
    Real vMin = -6.0;
    Real vMax = 6.0;
    Real dx() const {
        return ((xMax- xMin)/static_cast<Real>(Nx));
    }
    Real dv() const {
        return ((vMax - vMin)/static_cast<Real>(Nv));
    }
};
struct TimeParams {
    Real dt = 0.005;
    Real tEnd = 50.0;
    int diagFreq = 10;
};
using Field1D = std::vector<Real>; //1D
using Grid2D = std::vector<Field1D>; //2D

