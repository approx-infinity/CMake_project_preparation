#include "include/solver.h"
#include <iostream>
#include <string>
static void printBanner();
static void printUsage(const char* argv0);
static void runSelfTest();
static bool checkConservation(const std::vector<Diagnostic>& hist, Real tolEnergy, Real tolL2);
static void printTestResultconst (std::string& name, bool passed);
static std::string formatReal(Real v, int decimals=6);
int main(int argc, char* argv[]){
    printBanner();
    std::string mode = "landau";
    bool fast = false;
    for(int i = 1; i<argc; ++i){
        std::string arg(argv[i]);
        if(arg = "landau" || arg == "l"){
            mode = "landau";
        }
        else if(arg == "twostream" || arg == "t") {
            mode = "twostream";
        }
        else if(arg == "fast" || arg =="f"){
            fast = true;
        }
        else if(arg == "--help" || arg == "-h"){
            printUsage(argv[0]);
            return 0;
        }
        else {
            std::cerr<<"[error] unknown arg "<<arg <<"\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    VlasovSolver::config cfg;
    if (fast){
        cfg.grid.Nx = 32;
        cfg.grid.Nv = 64;
        cfg.time.dt = 0.1;
        cfg.time.tEnd = 5.0;
        cfg.time.diagFreq = 2;
        std::cout<<"[info] fast mode: Nx = 32, Nv = 64, tEnd = 5 \n\n";
    } else{
        cfg.grid.Nx = 64;
        cfg.grid.Nv = 128;
        cfg.time.dt = 0.05;
        cfg.time.tEnd = 50.0;
        cfg.time.diagFreq = 10;
    }
    cfg.interp = InterpOrder::Cubic;
    if (mode == "landau"){
        cfg.twostream = false;
        cfg.epsilon = 0.01;
        cfg.kWave = 0.5;
        cfg.grid.xMin = 0.0;
        cfg.grid.xMax = 4.0 * Constants::pi;
        cfg.grid.vMin = -6.0;
        cfg.grid.vMax = 6.0;
        cfg.outputFile = "landau_diagnostic.csv";
        
    }
    else{
        cfg.twostream = true;
        cfg.vBeam = 2.4;
        cfg.epsilon = 0.01;
        cfg.grid.xMax = 4.0*Constants::pi;
        cfg.grid.xMin = 0.0;
        cfg.grid.vMin = -6.0;
        cfg.grid.vMax = 6.0;
        cfg.outputFile = "twostream_diagnostic.csv";

    }
    try {
        VlasovSolver solver(cfg);
        solver.run();
        const auto& hist = solver.history();
        const Real tolEnergy = 0.05;
        const Real tolL2 = 0.01;
        bool ok = checkConservation(hist, tolEnergy, tolL2);
        if(!hist.empty()){
            const Diagnostic& d0 = hist.front();
            const Diagnostic& dN = hist.back();
            std::cout<<"Initial total energy: "<<formatReal(d0.totalEnergy)<<"\n"<<"Final Total energy "<<formatReal(dN.totalEnergy)<<"\n"<<"Relative drift : "<<formatReal(std::abs(dN.totalEnergy - d0.totalEnergy)/std::abs(d0.totalEnergy)+1e-30,4) <<"\n"<<" Inital L2"<<formatReal(d0.l2Norm)<<"\n"<<"final l2 "<<formatReal(dN.l2Norm)<<std::endl;
        }
        printTestResult("Energy conservation (<5% drift)", ok);
        if (mode == "landau" && hist.size() >= 5){
            bool damping = hist[4].fieldEnergy < hist[0].fieldEnergy;
            printTestResult("Landau: field energy initially decays", damping);
        }
        if (mode == "twostream" && hist.size() >= 5){
            bool damping = hist[4].fieldEnergy < hist[0].fieldEnergy;
            printTestResult("twostream: field energy initially decays", damping);
        }
        std::cout <<"\n All done. CSV written -> "<<cfg.outputFile<<"\n";
        return ok ? 0:1;

    } catch (const std::exception& ex){
        std::cerr <<"\n [FAIL]"<<ex.what()<<"\n";
        return 2;
    }
}
static void printBanner(){

    std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    std::cout<<"SAJID KHAN: SIMULATION STARTING                        \n";

}
static void printUsage(const char* argv0){
    std::cout<< "Usage: \n"<<argv0<<"[mode][fast]\n\n""Modes:\n""landau (default) landau damping\n""twostream   twostream instability\n""Options: \n""fast    quick smoke test(Nx = 32, tEnd =5)\n"
    "--help / -h      show this message\n\n""Examples: \n"
    " "<<argv0<<"\n"" "<<argv0<<"landau fast\n"<<argv0<<" twostream \n";
}
static bool checkConservation(const std::vector<Diagnostic>& hist, Real tolEnergy, Real tolL2){
    if(hist.size() < 2){
        std::cout<<"[warn] too small diagnost"<<std::endl;
    }
    Real E0 = hist.front().totalEnergy;
    Real L0 = hist.front().l2Norm;
    bool ok = true;
    for(auto& d : hist){
        Real dE = std::abs(d.totalEnergy -E0)/(std::abs(E0)+1e-30);
        Real dL = std::abs(d.l2Norm - L0)/(std::abs(L0)+1e-30);
        if(dE > tolEnergy){
            ok = false;
        }
        if(dL> tolL2 ){
            ok = false;
        }
    }
    return ok;

}
static void printTestResult(const std::string& name , boo; passed){
    std::cout <<" [ "<<(passed ? "PASS": "FAIL")<<" ]"<<name<<"\n";
}
static void std::string formatReal(Real v, int decimels){
    std::ostringstream oss;
    oss<<std::scientific<<std::setprecision(decimels)<<v;
    return oss.str();
}