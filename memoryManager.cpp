#include "memoryManager.h"

memoryManager::memoryManager()
{
}

memoryManager::memoryManager(int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls){
    this->allocationAlgorithim = memAlockAlgo;
    this->totalMemory = totalInstalledMemory;
    this->numberQuickLists = numberQuickList;
    this->minAmountCalls = numberMemoryCalls;
}

memoryBlock memoryManager::malloc(unsigned int SIZE){
    return memoryBlock();
}

void memoryManager::free(memoryBlock mem)
{
}

bool memoryManager::checkFreeMemory(unsigned int SIZE)
{
    return false;
}

void memoryManager::setAllocationAlgorithim(){
    switch (allocationAlgorithim) {
    case 1:
        firstFit();
        break;
    case 2:
        bestFit();
        break;
    case 3:
        quickFit();
        break;
    default:
        break;
    }
}

void memoryManager::firstFit()
{
}

void memoryManager::bestFit()
{
}

void memoryManager::quickFit()
{
}
