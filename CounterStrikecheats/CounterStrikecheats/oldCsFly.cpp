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
//std::uint32_t y_read_offset = 0x16C4E0;
//
//std::uint32_t y_address = 0x131D0CFC;
//std::uint32_t y_pos_addr = 0x086FAAB0;
//
//
///*std::uint32_t money1 = 0x1AF65E4;
//std::uint32_t money2 = 0x1AF6F500;
//std::uint32_t money3 = 0x0F0E9264;
//std::uint32_t money4 = 0xF0E9A48;*/
//
//
//const wchar_t* cs_module_name = L"hl.exe";		//+0x016C4E0
//const wchar_t* hw_module_name = L"hw.dll";
//
//
//
//
//
//
//
//std::uint32_t find_cs_pid()
//{
//	HANDLE process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
//	if (process_snap == NULL)
//	{
//		throw std::exception("process snap failed");
//	}
//	PROCESSENTRY32 pe{ sizeof(pe) };
//	Process32First(process_snap, &pe);
//	do
//	{
//		bool equal = !wcscmp(pe.szExeFile, cs_module_name);
//		if (equal)
//		{
//			return pe.th32ProcessID;
//		}
//	} while (Process32Next(process_snap, &pe));
//	std::cerr << "ERROR find_cs_pid() \n";
//	return 0;
//}
//std::uint32_t find_module_base(std::uint32_t p_id, const wchar_t* module_name)
//{
//	HANDLE modules_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, p_id);
//	if (modules_snap == NULL)
//	{
//		throw std::exception("Can`t snap modules");
//	}
//	MODULEENTRY32 me{ sizeof(me) };
//	Module32First(modules_snap, &me);
//
//	do
//	{
//		//std::wcout << "me.szModule: " << me.szModule<<std::endl;
//		if (!wcscmp(me.szModule, module_name))
//		{
//			CloseHandle(modules_snap);
//			return reinterpret_cast<std::uint32_t>(me.modBaseAddr);
//		}
//	} while (Module32Next(modules_snap, &me));
//	CloseHandle(modules_snap);
//	std::cerr << "ERROR find_module_base() \n";
//	return 0;
//}
//
//
//
//template<typename GameType>
//void scan_memory(HANDLE process_handle_, GameType value, std::vector<std::uint32_t>& addresses_out)
//{
//	SYSTEM_INFO s_i;
//	GetSystemInfo(&s_i);
//	std::uint8_t* start_ptr = static_cast<std::uint8_t*>(s_i.lpMinimumApplicationAddress);
//	std::uint8_t* end_ptr = static_cast<std::uint8_t*>(s_i.lpMaximumApplicationAddress);
//
//	addresses_out.clear();
//	std::uint8_t* current_ptr = start_ptr;
//
//	while (current_ptr < end_ptr)
//	{
//		MEMORY_BASIC_INFORMATION m_i;
//		std::size_t bytes = VirtualQueryEx(process_handle_, current_ptr, &m_i, sizeof(m_i));
//		if (m_i.State == MEM_COMMIT && m_i.Protect == PAGE_READWRITE)
//		{
//			std::vector<std::uint8_t> read_buffer(m_i.RegionSize);
//			SIZE_T read_byte;
//			if (ReadProcessMemory(process_handle_, current_ptr, read_buffer.data(), m_i.RegionSize, &read_byte) == TRUE)
//			{
//				GameType* current_page_ptr = reinterpret_cast<GameType*>(read_buffer.data());
//				while ((std::uint8_t*)current_page_ptr < read_buffer.data() + read_buffer.size())
//				{
//					if (*current_page_ptr == value)
//					{
//						addresses_out.push_back((std::uint32_t)(((std::uint8_t*)current_page_ptr -
//							read_buffer.data()) + (std::uint8_t*)m_i.BaseAddress));
//
//					}
//					current_page_ptr = (GameType*)(((char*)current_page_ptr) + 1);
//				}
//
//			}
//		}
//		current_ptr += m_i.RegionSize;
//	}
//}
//
//
//
//int main()
//{
//	std::uint32_t pid = find_cs_pid();
//	HANDLE csProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
//	if (csProcess == NULL)
//	{
//		return 1;
//	}
//	std::cout << "pid: " << pid << std::endl;
//	std::uint32_t module_base_addr = find_module_base(pid, hw_module_name);
//	std::uint32_t y_read_address = module_base_addr + y_read_offset;
//	float y_value;
//	SIZE_T size;
//	ReadProcessMemory(csProcess, reinterpret_cast<void*>(y_read_address), &y_value, sizeof(y_value), &size);
//
//	std::vector<std::uint32_t> addresses;
//	scan_memory(csProcess, y_value, addresses);
//	std::cout << addresses.size();
//	CloseHandle(csProcess);
//
//	//float yPos = 200;
//	//float MONEY = 16000;
//	//SIZE_T size;
//	//while (true)
//	//{
//	//	//WriteProcessMemory(csprocess, reinterpret_cast<void*>(money1), &MONEY, sizeof(MONEY), &size);
//	//	//WriteProcessMemory(csprocess, reinterpret_cast<void*>(money2), &MONEY, sizeof(MONEY), &size);
//	//	WriteProcessMemory(csProcess, reinterpret_cast<void*>(money3), &MONEY, sizeof(MONEY), &size);
//	//	WriteProcessMemory(csProcess, reinterpret_cast<void*>(money4), &MONEY, sizeof(MONEY), &size);
//	//	//std::cout << check1 << " " << GetLastError << "\n" << check2<<std::endl;
//	//}
//return 0;
//}