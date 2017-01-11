/*
 * Program Vulnerability Factor (PVF) Analysis
 * Author: Robert Schofield
 */

#include "base/pvf/pvfmain.hh"
#include "cpu/static_inst.hh"

static const int NUM_REGISTERS = 32;

PVFAnalyzer::PVFAnalyzer() {
    instructionCount = 0;
    //TODO: numRegisters = arch->GetNumRegistersForArch();
    registers = new PVFRegister[NUM_REGISTERS];
    std::cout << "PVFAnalyzer class was successfully" <<
        "constructed." << std::endl;
}


PVFAnalyzer::~PVFAnalyzer() {
    // Deconstructor. Intentionally left empty.
}

void
PVFAnalyzer::receiveInst(StaticInstPtr& inst) {
    int numDestRegisters = inst->numDestRegs();
    int numSrcRegisters = inst->numSrcRegs();

    for (int i = 0; i < numSrcRegisters; i++) {
        // These are all READS.
        registers[inst->srcRegIdx(i)].vulInstCount += (instructionCount -
            registers[inst->srcRegIdx(i)].writeAt);
        std::cout << "     READ: reg#= " << inst->srcRegIdx(i) << " writeAt = "
            << registers[inst->srcRegIdx(i)].writeAt <<
            " vulInstCount = "
            << registers[inst->srcRegIdx(i)].vulInstCount << std::endl;

        registers[inst->srcRegIdx(i)].writeAt = instructionCount;
    }

    for (int i = 0; i < numDestRegisters; i++) {
        // These are all WRITES.
        registers[inst->destRegIdx(i)].writeAt = instructionCount;
        std::cout << "     WRITE: reg#= " << inst->destRegIdx(i) <<
            " vulInstCount = "
            << registers[inst->destRegIdx(i)].vulInstCount << std::endl;

    }
    std::cout << "INSTCOUNT = " << instructionCount << std::endl;

    std::cout << inst->getName() << "   PVF=" << this->getPVF() << std::endl;

    instructionCount++;

}

double
PVFAnalyzer::getPVFReg(int regIndex) {
    return (double)(registers[regIndex].vulInstCount / instructionCount);
}

double
PVFAnalyzer::getPVF() {
    if (instructionCount == 0) {
        return 0;
    }
    else {
        double pvfTotal = 0;
        double vulICount[NUM_REGISTERS];
        for (int i = 0; i < NUM_REGISTERS; i++) {
            vulICount[i] = registers[i].vulInstCount;
            pvfTotal += vulICount[i] / instructionCount;
        }
        return pvfTotal / NUM_REGISTERS;
    }
}
