/*
 * Program Vulnerability Factor (PVF) Analysis
 * Author: Robert Schofield
 */


#include "base/pvf/pvfmain.hh"
#include "cpu/base.hh"

#include <string>

using namespace std;

static const int NUM_REGISTERS = 16;

PVFAnalyzer::PVFAnalyzer(std::string statsFileName, int instInterval) {
    instructionCount = 0;
    //TODO: numRegisters = arch->GetNumRegistersForArch();
    this->registers = new PVFRegister[NUM_REGISTERS];
    this->statsFile.open(statsFileName);
    this->instInterval = instInterval;
    this->debugOn = false; //true;
    /*std::cout << "PVFAnalyzer class was successfully" <<
        "constructed." << std::endl;*/
}


PVFAnalyzer::~PVFAnalyzer() {
    // Deconstructor.
    this->statsFile.close();
}

void
PVFAnalyzer::receiveInst(StaticInstPtr& inst) {

    int numDestRegisters = inst->numDestRegs();
    int numSrcRegisters = inst->numSrcRegs();

    if (debugOn) {
        std::cout << "[" << instructionCount << "] " << inst->getName() << " instInterval = " << instInterval << std::endl;
    }

    for (int i = 0; i < numSrcRegisters; i++) {
        // These are all READS.

        if (inst->srcRegIdx(i) <= NUM_REGISTERS) {
            registers[inst->srcRegIdx(i)].vulInstCount += (instructionCount -
                registers[inst->srcRegIdx(i)].writeAt);
            /*statsFile << "     READ: reg#= " << inst->srcRegIdx(i) << " writeAt = "
                    << registers[inst->srcRegIdx(i)].writeAt <<
                    " vulInstCount = "
                    << registers[inst->srcRegIdx(i)].vulInstCount << std::endl;*/
            if (debugOn) {
                std::cout << "     READ: reg#= " << inst->srcRegIdx(i) << " writeAt = "
                    << registers[inst->srcRegIdx(i)].writeAt <<
                    " vulInstCount = "
                    << registers[inst->srcRegIdx(i)].vulInstCount << std::endl;
            }

            registers[inst->srcRegIdx(i)].writeAt = instructionCount;
        }
    }

    for (int i = 0; i < numDestRegisters; i++) {
        // These are all WRITES.
        if (inst->destRegIdx(i) <= NUM_REGISTERS) {
            registers[inst->destRegIdx(i)].writeAt = instructionCount;
            /*statsFile << "     WRITE: reg#= " << inst->destRegIdx(i) <<
                    " vulInstCount = "
                    << registers[inst->destRegIdx(i)].vulInstCount << std::endl;*/
            if (debugOn) {
                std::cout << "     WRITE: reg#= " << inst->destRegIdx(i) <<
                    " vulInstCount = "
                    << registers[inst->destRegIdx(i)].vulInstCount << std::endl;
            }
        }

    }

    instructionCount++;

    if (instructionCount % instInterval == 0) {
        statsFile << "     INSTCOUNT = " << instructionCount << std::endl;
        statsFile << "     PVF: " << this->getPVF() << std::endl;
    }
    

    if (debugOn) {
        std::cout << "     INSTCOUNT = " << instructionCount << std::endl;
        std::cout << "     PVF: " << this->getPVF() << std::endl;
    }

}

double
PVFAnalyzer::getPVFReg(int regIndex) {
    double pvfForReg = (double) registers[regIndex].vulInstCount / (double) instructionCount;
    return pvfForReg; 
}

double
PVFAnalyzer::getPVF() {
    if (instructionCount == 0) {
        return 0;
    }
    else {
        double pvfTotal = 0;
        for (int i = 0; i < NUM_REGISTERS; i++) {
            pvfTotal += getPVFReg(i);
            //std::cout << "         Reg[" << i << "] PVF = " << getPVFReg(i) << std::endl;
        }
        //std::cout << "       pvfTotal = " << pvfTotal << std::endl;
        return pvfTotal / NUM_REGISTERS;
    }
}
