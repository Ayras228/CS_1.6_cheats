#include<Windows.h>
#include<TlHelp32.h>

#include<iostream>
#include<utility>
#include<string>

std::uint32_t y_addres = 0x131D0CFC;
std::uint32_t y_pos_addr = 0x086FAAB0;




//money
//0FD775E4
//0FD77DC8
//client.dll + 1254E4
//client.dll + 12F500
// 
//std::uint32_t money1 = 0x0FD775E4;
//std::uint32_t money2 = 0x0FD77DC8;

std::uint32_t money1 = 0x1AF65E4;
std::uint32_t money2 = 0x1AF6F500;
std::uint32_t money3 = 0x0F0E9264;		//!! WORK
std::uint32_t money4 = 0xF0E9A48;		//!! WORK



//std::uint32_t y_pos_addr = 0xclient.dll + 1254E4		1AF65E4
//std::uint32_t y_pos_addr = 0xclient.dll + 12F500		1AF6F500



std::string cs_module_name = "hl.exe";
std::string hw_module_name = "hw.exe";

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}


std::uint32_t find_cs_pid()
{
	HANDLE process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (process_snap == NULL)
	{
		throw std::exception("process snap failed");
	}
	PROCESSENTRY32 pe{ sizeof(pe) };
	Process32First(process_snap, &pe);
	do
	{
		std::wstring w_module_name = s2ws(cs_module_name);
		bool equal = std::wstring(pe.szExeFile) == w_module_name;
		if (equal)
		{
			return pe.th32ProcessID;
		}
	} while (Process32Next(process_snap,&pe));

	return 0;
}


std::pair<std::uint32_t, std::wstring> find_module_base(std::uint32_t p_id)
{
	HANDLE modules_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, p_id);
	if (modules_snap == NULL)
	{
		throw std::exception("Can`t snap modules");
	}
	MODULEENTRY32 me{ sizeof(me) };
	Module32First(modules_snap, &me);
	do
	{
		bool match = reinterpret_cast<std::uint32_t>(me.modBaseAddr) 
			<= y_pos_addr && y_pos_addr 
			< reinterpret_cast<std::uint32_t>(me.modBaseAddr) 
			+ me.modBaseSize;
		if (match)
		{
			CloseHandle(modules_snap);
			return std::make_pair(reinterpret_cast<std::uint32_t>(me.modBaseAddr), me.szModule);
		}
	} while (Module32Next(modules_snap, &me));
	CloseHandle(modules_snap);
	return {};
}
int main()
{
	/*auto addr_name=find_module_base(find_cs_pid());
	std::uint32_t offset = y_addres - addr_name.first;
	std::wcout << addr_name.second << " " << offset;*/

	std::uint32_t pid = find_cs_pid();
	HANDLE csprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (csprocess==NULL)
	{
		return 1;
	}
	float yPos = 200;
	float MONEY = 16000;
	SIZE_T size;
	while (true)
	{
		//WriteProcessMemory(csprocess, reinterpret_cast<void*>(money1), &MONEY, sizeof(MONEY), &size);
		//WriteProcessMemory(csprocess, reinterpret_cast<void*>(money2), &MONEY, sizeof(MONEY), &size);
		WriteProcessMemory(csprocess, reinterpret_cast<void*>(money3), &MONEY, sizeof(MONEY), &size);
		WriteProcessMemory(csprocess, reinterpret_cast<void*>(money4), &MONEY, sizeof(MONEY), &size);
		//std::cout << check1 << " " << GetLastError << "\n" << check2<<std::endl;
	}
	return 0;
}