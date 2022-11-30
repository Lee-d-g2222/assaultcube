#include "pid_module.h"

void pid_module::getProcess(const wchar_t* procName)
{
    this->procId = getProcId(procName);
    this->moduleBaseAddr = getBaseAddr(procId, procName);
    this->hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->procId);
}