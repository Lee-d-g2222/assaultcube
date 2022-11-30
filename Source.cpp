#include "pid_module.h"
#include "offsets.h"
#include "includes.h"

int main() {
    pid_module pidModule;
    pidModule.getProcess(L"ac_client.exe");

    uintptr_t ptrBaseAddr = pidModule.moduleBaseAddr + 0x10F4F4;

    uintptr_t ammoAddr = calcMultiOffsets(pidModule.hProc, ptrBaseAddr, { 0x150 });
    uintptr_t healthAddr = calcMultiOffsets(pidModule.hProc, ptrBaseAddr, { 0xF8 });

    short ammoValue = 0;
    short healthValue = 0;


    while (TRUE) {
        ReadProcessMemory(pidModule.hProc, (LPVOID)ammoAddr, &ammoValue, sizeof(ammoValue), NULL);
        ReadProcessMemory(pidModule.hProc, (LPVOID)healthAddr, &healthValue, sizeof(healthValue), NULL);
        std::cout << "Assault Rifle ammo Value : " << std::dec << ammoValue << std::endl;
        std::cout << "Health Value : " << std::dec << healthValue << std::endl;
        Sleep(100);
        system("cls");
    }
}