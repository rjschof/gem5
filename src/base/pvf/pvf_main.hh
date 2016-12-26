


#ifndef __PVF__MAIN__HH__
#define __PVF__MAIN__HH__

#include "cpu/static_inst.hh"

class PVFAnalyzer {



private:

public:
    static void
    pvfAnalysis() {
        std::cout << "PVF Analysis started." << std::endl;

        // TODO: PVF Analysis.


        std::cout << "PVF Analysis completed." << std::endl;
    }

    static void
    giveInst(StaticInstPtr& inst) {
        std::cout << inst->getName() << std::endl;
    }

/*
private:

    int readCount;

    bool is_enabled;

public:

    PVFAnalyzer();

    ~PVFAnalyzer();

    void pvfAnalysis();

    bool isEnabled();*/

};


#endif
