#include "MemoryBlock.h"

memoryBlock::memoryBlock(unsigned int totalBlockSize, unsigned int blockAdress){
    this->totalBlockSize = totalBlockSize;
    this->occupiedSize = totalBlockSize;
    this->blockAdress = blockAdress;
    this->nextFreeBlock = nullptr;
}
