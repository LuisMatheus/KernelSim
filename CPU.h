#pragma once

#include<vector>
#include "Core.h"

class CPU {
public:

    std::vector<core*> corePool;

    CPU();

    CPU(unsigned int coreNumber);

};