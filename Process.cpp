#include "Process.h"

process::process(int id, int time, unsigned int totalMemoryUsed){
    this->id = id;
    this->totalTime = time;
    this->remaningTime = this->totalTime;
    this->state = READY;
    this->totalMemoryUsed = totalMemoryUsed;
}

void process::generateRandomStaticMemoryCall(memoryBlock* adress){
    if (adress != nullptr) {
        totalMemoryUsed += adress->totalBlockSize;
        memoryPointer.emplace_back(adress);
    }    
}

void process::generateRandomDynamicMemoryCall(memoryBlock* m){
    totalMemoryUsed += m->occupiedSize;
    memoryPointer.emplace_back(m);
}
