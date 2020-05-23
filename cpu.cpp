#include "cpu.h"

CPU::CPU(){}

CPU::CPU(unsigned int coreNumber){
    for (unsigned int i = 0; i < coreNumber; i++) {
        corePool.emplace_back(new core(i));
    }
}
