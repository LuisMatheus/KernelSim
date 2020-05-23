#include "process.h"


process::process(int id, int time, unsigned int totalMemoryUsed, kernel* ker)
{
    this->id = id;
    this->totalTime = time;
    this->remaningTime = this->totalTime;
    this->state = READY;
    this->totalMemoryUsed = totalMemoryUsed;
    generateRandomStaticMemoryCall(totalMemoryUsed);
    this->ker = ker;
}

bool process::generateRandomStaticMemoryCall(unsigned int totalMemoryUsed)
{
    return false;
}

bool process::generateRandomDynamicMemoryCall()
{
    return false;
}
