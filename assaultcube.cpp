#include <iostream>
#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include <vector>

uintptr_t calcMultiOffsets(HANDLE hProc, uintptr_t baseAddr, std::vector<unsigned short> offsets);
uintptr_t getBaseAddr(DWORD pid, const wchar_t* moduleName);
DWORD findProcId(LPCWSTR windowsName);


int main() {

    DWORD pid = findProcId(L"AssaultCube");
	uintptr_t moduleBaseAddr = getBaseAddr(pid, L"midimap.dll");

	std::cout << "Process ID : " << std::dec << pid << "\t ModuleBaseAddress : 0x" << std::hex << moduleBaseAddr << std::endl;

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	uintptr_t hpAddr = calcMultiOffsets(hProc, (moduleBaseAddr + 0x000FAD28), { 0x98, 0x28, 0x0, 0x0, 0x4D8, 0x38, 0x4E0 });
	short ammoValue = 0;
	ReadProcessMemory(hProc, (LPVOID)hpAddr, &ammoValue, sizeof(short), NULL);

	std::cout << "Assault Rifle ammo Value : " << std::dec << ammoValue << std::endl;


	return 0;
}

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

DWORD findProcId(LPCWSTR windowsName) {
	DWORD pid;
	GetWindowThreadProcessId(FindWindow(0, windowsName), &pid);
	return pid;
}

uintptr_t calcMultiOffsets(HANDLE hProc, uintptr_t baseAddr, std::vector<unsigned short> offsets) {
	uintptr_t addr = baseAddr;
	for (unsigned __int8 i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), NULL);
		addr += offsets[i];
	}
	return addr;
}
