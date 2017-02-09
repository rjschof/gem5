


#ifndef __PVF__MAIN__HH__
#define __PVF__MAIN__HH__

#include "cpu/static_inst.hh"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

typedef struct {
    // The number of vulnerable instructions for a register.
    uint64_t vulInstCount = 0;

    // The current instruction number that we last wrote at. After a read,
    // this is set to -1.
    uint64_t writeAt = -1;
} PVFRegister;

class PVFAnalyzer {
private:

    // --- Configuration information for PVF Analyzer
    // Number of cycles per interval for analysis output
    int instInterval;

    // Whether to print out debug information with the analysis. 
    bool debugOn;
    // ---!

    // File stream for the output.
    ofstream statsFile;

    // The number of instructions that are received. Initial = 0.
    int instructionCount;

    // An array of the registers that are found in the current architecture.
    PVFRegister *registers;

    // The number of registers in the current architecture.
    int numRegisters;

public:

    // Constructor for the PVFAnalyzer class.
    PVFAnalyzer(std::string statsFileName, int instInterval);

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
