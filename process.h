#pragma once
#include "memoryBlock.h"
#include "kernel.h"
#include "Definer.h"

class kernel;

class process {

public:

    unsigned int id;
    unsigned int totalTime;
    int state;
    unsigned int remaningTime;

    unsigned int totalMemoryUsed;

    memoryBlock* memoryPointer;

    kernel* ker;

    process(int id, int time, unsigned int totalMemoryUsed, kernel* ker);

    bool generateRandomStaticMemoryCall(unsigned int totalMemoryUsed);

    bool generateRandomDynamicMemoryCall();

};