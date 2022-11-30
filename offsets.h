#pragma once
#include "includes.h"

uintptr_t calcMultiOffsets(HANDLE hProc, uintptr_t baseAddr, std::vector<unsigned short> offsets);