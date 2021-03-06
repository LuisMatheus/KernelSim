#pragma once
#include "CPU.h"
#include "Kernel.h"

#include<thread>
class simulator {
private:

    unsigned int processInit;
    CPU cpu;
    kernel* ker;

public:

    simulator(unsigned int coreNumber, unsigned int processInit, unsigned int sched, unsigned int quantum, int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls);

    void run();

    void createRandomProcess();

    void batchProcessInit(unsigned int processInit);

};