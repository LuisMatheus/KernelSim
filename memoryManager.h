#pragma once
#include "memoryBlock.h"

#include<iostream>
#include<vector>

class memoryManager {
public:
    int allocationAlgorithim;
    unsigned int totalMemory;
    unsigned int memoryOverhead;
    unsigned int occupiedMemory;

    //statistics table
    //quickfit free blocks

    unsigned int minAmountCalls;
    unsigned int numberQuickLists;

    std::vector<memoryBlock> memory;
    memoryBlock* freeBlocksList;

    memoryManager();

    memoryManager(int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls);

    memoryBlock malloc(unsigned int SIZE);

    void free(memoryBlock mem);

    bool checkFreeMemory(unsigned int SIZE);

    void setAllocationAlgorithim();

    void firstFit();

    void bestFit();

    void quickFit();

};