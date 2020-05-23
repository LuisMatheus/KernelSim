#pragma once
#include "core.h"

#include <vector>

class core;

class CPU {
public:

    std::vector<core*> corePool;

    CPU();

    CPU(unsigned int coreNumber);

};