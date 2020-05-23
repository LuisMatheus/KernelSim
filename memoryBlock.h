#pragma once
class memoryBlock {
    unsigned int totalBlockSize;
    unsigned int occupiedSize;
    unsigned int blockAdress;
    memoryBlock* nextFreeBlocks;
};