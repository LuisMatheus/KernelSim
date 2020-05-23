#pragma once
#include "process.h"

class process;

class core {
public:

    process* p;
    int id;

    core(int id);
};