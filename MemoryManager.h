#pragma once
#include<vector>
#include<map>
#include "MemoryBlock.h"
#include "Defines.h"
#include <mutex>

using namespace std;

class memoryManager {
public:
    int allocationAlgorithim;
    int totalMemory = 0;
    int memoryOverhead = 0;
    int occupiedMemory = 0;

    //quickfit special blocks
    vector<vector<memoryBlock*>> quickFitBlocks;

    map<int,int> statistics;

    //tamanhos sendo usado pelo quick list
    vector<int> quickListMembers;

    int minAmountCalls;
    unsigned int numberQuickLists;

    unsigned int adrCounter = 1;

    vector<memoryBlock*> memory;
    memoryBlock* freeBlocksList = nullptr;

    memoryManager();

    memoryManager(int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls);

    int getAvailableMemory();

    memoryBlock* malloc(unsigned int SIZE);

    void free(memoryBlock* mem);

    bool checkFreeMemory(unsigned int SIZE);

    memoryBlock* setAllocationAlgorithim(unsigned int SIZE);

    memoryBlock* firstFit(int SIZE);

    memoryBlock* bestFit(int SIZE);

    memoryBlock* quickFit(int SIZE);

    void updateStatistics(int SIZE);
    
    vector<int> getMostFrequent( int QTY);

    void freeQuickList(memoryBlock* mem);

    void freeMemory(memoryBlock* mem);

    memoryBlock* quickFitAlocation( int SIZE);
};