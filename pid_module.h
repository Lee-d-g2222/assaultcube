#pragma once
#include "includes.h"

class pid_module
{
private:
    uintptr_t getBaseAddr(DWORD pid, const wchar_t* moduleName) {
        // ���μ����� ��� ����� �������� ���� ��������
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        // �������� �ڵ��� INVALID VALUE �� �ƴ϶�� 
        if (hSnap != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 moduleEntry; // ���μ��� ��� ����� �����ϴ� ����ü(modEntry)
            moduleEntry.dwSize = sizeof(moduleEntry); // modEntry��ŭ �ʱ�ȭ
            if (Module32First(hSnap, &moduleEntry)) { // moduleEntry�� Module32First�� ����ؼ� ù Module�� �ҷ���
                do {
                    // hsnap ����̸��� �Ű������� ���� ����̸��� ������
                    if (!wcscmp(moduleEntry.szModule, moduleName)) {
                        //�ڵ� ����
                        CloseHandle(hSnap);
                        // ������ ��⺣�̽� �ּ� ��ȯ
                        return (uintptr_t)moduleEntry.modBaseAddr;
                    }
                } while (Module32Next(hSnap, &moduleEntry)); //moduleEntry�� Module32Next�� ����ؼ� ���� Module�� �ҷ���
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