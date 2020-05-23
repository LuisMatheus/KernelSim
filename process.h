#pragma once
#include "memoryBlock.h"
#include "Definer.h"

class process {

public:

    unsigned int id;
    unsigned int totalTime;
    int state;
    unsigned int remaningTime;

    unsigned int totalMemoryUsed;

    memoryBlock* memoryPointer;

    process(int id, int time, unsigned int totalMemoryUsed);

    bool generateRandomStaticMemoryCall(unsigned int totalMemoryUsed);

    bool generateRandomDynamicMemoryCall();

};