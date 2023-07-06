//#include<Windows.h>
//#include<TlHelp32.h>
//
//#include<iostream>
//#include<utility>
//#include<string>
//#include<vector>
//#include<chrono>
//
//
//
//
//
//std::uint32_t y_read_offset = 0x16C4E0;  //0x108AAB0 in video	//0x12F500	//0x1254E4		//my read Z coordinate 00189564		//0x16C4E0
//
//std::uint32_t y_addres = 0x131D0CFC;
//std::uint32_t y_pos_addr = 0x086FAAB0;
//
//
//
//
//
//
//std::uint32_t money1 = 0x1AF65E4;
//std::uint32_t money2 = 0x1AF6F500;
//std::uint32_t money3 = 0x0F0E9264;
//std::uint32_t money4 = 0xF0E9A48;
//
//
//
////std::uint32_t y_pos_addr = 0xclient.dll + 1254E4		1AF65E4
////std::uint32_t y_pos_addr = 0xclient.dll + 12F500		1AF6F500
//
//
//
//const wchar_t* cs_module_name = L"hl.exe";		//+0x016C4E0
//const wchar_t* hw_module_name = L"hw.dll";		//hw.dll+16C4E0
//
//bool GetPid(const wchar_t* targetProcess, DWORD* procID)
//{
//    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//    if (snap && snap != INVALID_HANDLE_VALUE)
//    {
//        PROCESSENTRY32 pe;
//        pe.dwSize = sizeof(pe);
//        if (Process32First(snap, &pe))
//        {
//            do
//            {
//                if (!wcscmp(pe.szExeFile, targetProcess))
//                {
//                    CloseHandle(snap);
//                    *procID = pe.th32ProcessID;
//                    return true;
//                }
//            } while (Process32Next(snap, &pe));
//        }
//    }
//    return false;
//}
//
//char* GetModuleBase(const wchar_t* ModuleName, DWORD procID)
//{
//    MODULEENTRY32 ModuleEntry = { 0 };
//    HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
//
//    if (!SnapShot) return NULL;
//
//    ModuleEntry.dwSize = sizeof(ModuleEntry);
//
//    if (!Module32First(SnapShot, &ModuleEntry)) return NULL;
//
//    do
//    {
//        if (!wcscmp(ModuleEntry.szModule, ModuleName))
//        {
//            CloseHandle(SnapShot);
//            return (char*)ModuleEntry.modBaseAddr;
//        }
//    } while (Module32Next(SnapShot, &ModuleEntry));
//
//    CloseHandle(SnapShot);
//    return NULL;
//}
//
//uintptr_t FindDmaAddy(int PointerLevel, HANDLE hProcHandle, uintptr_t Offsets[], uintptr_t BaseAddress)
//{
//    uintptr_t pointer = BaseAddress;
//    uintptr_t pTemp;
//
//    uintptr_t pointerAddr;
//    for (int i = 0; i < PointerLevel; i++)
//    {
//        if (i == 0)
//        {
//            ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), NULL);
//        }
//        pointerAddr = pTemp + Offsets[i];
//
//        ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, sizeof(pTemp), NULL);
//    }
//    return pointerAddr;
//}
//int main()
//{
//    DWORD ProcId;
//    GetPid(cs_module_name, &ProcId);
//    char* ExeBaseAddress = GetModuleBase(hw_module_name, ProcId);
//    DWORD BaseAddress = (DWORD)GetModuleHandle(hw_module_name);
//
//}
