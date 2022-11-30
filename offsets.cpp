#include "offsets.h"

uintptr_t calcMultiOffsets(HANDLE hProc, uintptr_t baseAddr, std::vector<unsigned short> offsets) {
    uintptr_t addr = baseAddr;
    for (unsigned __int8 i = 0; i < offsets.size(); ++i)
    {
        ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), NULL);
        addr += offsets[i];
    }
    return addr;
}