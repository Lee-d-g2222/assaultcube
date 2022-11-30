#pragma once
#include "includes.h"

class pid_module
{
private:
    uintptr_t getBaseAddr(DWORD pid, const wchar_t* moduleName) {
        // 프로세스의 모든 모듈을 가져오기 위한 인자전달
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        // 스냅샷의 핸들이 INVALID VALUE 가 아니라면 
        if (hSnap != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 moduleEntry; // 프로세스 모듈 목록을 설명하는 구조체(modEntry)
            moduleEntry.dwSize = sizeof(moduleEntry); // modEntry만큼 초기화
            if (Module32First(hSnap, &moduleEntry)) { // moduleEntry에 Module32First를 사용해서 첫 Module을 불러옴
                do {
                    // hsnap 모듈이름과 매개변수로 받은 모듈이름이 같으면
                    if (!wcscmp(moduleEntry.szModule, moduleName)) {
                        //핸들 종료
                        CloseHandle(hSnap);
                        // 같으면 모듈베이스 주소 반환
                        return (uintptr_t)moduleEntry.modBaseAddr;
                    }
                } while (Module32Next(hSnap, &moduleEntry)); //moduleEntry에 Module32Next를 사용해서 다음 Module을 불러옴
            }
        }
    }

    DWORD getProcId(const wchar_t* procName) {
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnap != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 procEntry;
            procEntry.dwSize = sizeof(procEntry);
            if (Process32First(hSnap, &procEntry)) {
                do {
                    if (!_wcsicmp(procEntry.szExeFile, procName)) {
                        CloseHandle(hSnap);
                        return procEntry.th32ProcessID;
                    }
                } while (Process32Next(hSnap, &procEntry));
            }
        }
    }


public:
    DWORD procId;
    uintptr_t moduleBaseAddr;
    HANDLE hProc;

    void getProcess(const wchar_t* procName);
};