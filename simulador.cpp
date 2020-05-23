#include "simulador.h"

simulator::simulator(unsigned int coreNumber, unsigned int processInit, unsigned int sched, unsigned int quantum, int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls)
{

    this->processInit = processInit;
    cpu = CPU(coreNumber);
    ker = new kernel(&cpu,sched,quantum,memAlockAlgo,totalInstalledMemory,numberQuickList,numberMemoryCalls);
}

void simulator::run()
{
    batchProcessInit(this->processInit);
    this_thread::sleep_for(chrono::milliseconds(500));
    thread kert(&kernel::run, ker);
    while (true) {
        createRandomProcess();
        this_thread::sleep_for(chrono::seconds(10));
    };
    kert.join();
}

void simulator::createRandomProcess()
{
    if ((rand() % 10 + 1) % 2 == 0) {
        for (unsigned int i = 0; i < rand() % 5 + 1; i++)
        {

            ker->createProcess();

        }
    }
}

void simulator::batchProcessInit(unsigned int processInit)
{
    for (unsigned int i = 0; i < processInit - 1; i++) {

        ker->createProcess();

    }
}
