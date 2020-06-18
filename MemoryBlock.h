#pragma once
class memoryBlock {
public:
    unsigned int totalBlockSize;
    unsigned int occupiedSize;
    unsigned int blockAdress;
    memoryBlock* nextFreeBlock;

    memoryBlock(unsigned int totalBlockSize, unsigned int blockAdress);
};