#pragma once
#include "Defines.h"
#include "MemoryBlock.h"

#include<vector>

using namespace std;

class process {

public:

    unsigned int id;
    unsigned int totalTime;
    int state;
    unsigned int remaningTime;

    unsigned int totalMemoryUsed;

    vector<memoryBlock*> memoryPointer;

    process(int id, int time, unsigned int totalMemoryUsed);

    void generateRandomStaticMemoryCall(memoryBlock* adress);

    void generateRandomDynamicMemoryCall(memoryBlock* m);

};