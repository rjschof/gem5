


#ifndef __PVF__MAIN__HH__
#define __PVF__MAIN__HH__

#include "cpu/static_inst.hh"

class PVFAnalyzer {
    struct _PVFRegister {
        // The number of vulnerable instructions for a register.
        int vulInstCount = 0;

        // The current instruction number that we last wrote at. After a read,
        // this is set to -1.
        int writeAt = -1;
    };
    typedef _PVFRegister PVFRegister;

private:

    // The number of instructions that are received. Initial = 0.
    int instructionCount;

    // An array of the registers that are found in the current architecture.
    PVFRegister *registers;

    // The number of registers in the current architecture.
    int numRegisters;

public:

    // Constructor for the PVFAnalyzer class.
    PVFAnalyzer();

    // Deconstructor for the PVFAnalyzer class.
    ~PVFAnalyzer();

    // Receive a new instruction from the interpreter.
    void receiveInst(StaticInstPtr& inst);

    // Retrieve the current PVF for a specific register
    // (by number as integer)
    double getPVFReg(int regIndex);

    // Retrieve the overall PVF.
    double getPVF();
};

#endif
