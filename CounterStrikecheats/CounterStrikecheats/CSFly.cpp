#include<Windows.h>
#include<TlHelp32.h>

#include<iostream>
#include<utility>
#include<string>
#include<vector>
#include<chrono>

template<typename GameType>
class Cheat
{
public:
	Cheat();
	~Cheat();
	/*std::uint32_t get_pid();
	std::uint32_t get_module_base_address();
	std::uint32_t get_y_read_address();
	std::vector<std::uint32_t> get_addresses_value_type();*/
	HANDLE get_csProcess();
	//void find_address(GameType & y_value);
	void scan_memory(std::uint32_t read_address, GameType value);

private:
	std::uint32_t find_cs_pid();
	std::uint32_t find_module_base(const wchar_t*  module_name);

	void search_address(std::uint8_t* current_ptr,GameType value,  MEMORY_BASIC_INFORMATION& m_i);
	void _init();
	std::uint32_t pid;
	HANDLE csProcess;
	//std::uint32_t module_base_address;
	std::uint32_t y_read_address;
	std::uint32_t money_read_address;

	const std::uint32_t y_read_offset = 0x16C4E0;
	const std::uint32_t money_read_offset = 0x12F500;

	const wchar_t* cs_module_name = L"hl.exe";		//+ 0x016C4E0
	const wchar_t* hw_module_name = L"hw.dll";
	const wchar_t* cl_module_name = L"client.dll"; // + 0x12F500 money
	
	std::vector<std::uint32_t> addresses_value_type;
};

template<typename GameType>
Cheat<GameType>::Cheat()
{
	pid = find_cs_pid();
	std::cout << "pid: " << pid << std::endl;
	csProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (csProcess == NULL)
	{
		throw std::exception("OpenProcess failed: ",pid);
	}
	
	_init();
}

template<typename GameType>
Cheat<GameType>::~Cheat()
{
}
//template<typename GameType>
//std::uint32_t Cheat<GameType>::get_pid()
//{
//	return pid;
//}
//template<typename GameType>
//std::uint32_t Cheat<GameType>::get_module_base_address()
//{
//	return module_base_address;
//}
//template<typename GameType>
//std::uint32_t Cheat<GameType>::get_y_read_address()
//{
//	return y_read_address;
//}
//template<typename GameType>
//std::vector<std::uint32_t> Cheat<GameType>::get_addresses_value_type()
//{
//	return addresses_value_type;
//}
template<typename GameType>
HANDLE Cheat<GameType>::get_csProcess()
{
	return csProcess;
}

//template<typename GameType>
//void Cheat<GameType>::find_address(GameType & y_value)
//{
//	SIZE_T size;
//	ReadProcessMemory(csProcess, reinterpret_cast<void*>(y_read_address), &y_value, sizeof(y_value), &size);
//
//}


template<typename GameType>
std::uint32_t Cheat<GameType>::find_cs_pid()
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
		if (!wcscmp(pe.szExeFile, cs_module_name))
		{
			return pe.th32ProcessID;
		}
	} while (Process32Next(process_snap, &pe));

	throw std::exception("not found cs_module_name ");
}
template<typename GameType>
std::uint32_t Cheat<GameType>::find_module_base(const wchar_t*  module_name)
{
	HANDLE modules_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (modules_snap == NULL)
	{
		throw std::exception("Can`t snap modules");
	}
	MODULEENTRY32 me{ sizeof(me) };
	Module32First(modules_snap, &me);

	do
	{
		//std::wcout << "me.szModule: " << me.szModule<<std::endl;
		if (!wcscmp(me.szModule, module_name))
		{
			CloseHandle(modules_snap);
			return reinterpret_cast<std::uint32_t>(me.modBaseAddr);
		}
	} while (Module32Next(modules_snap, &me));
	CloseHandle(modules_snap);
	throw std::exception("not found module_name ");

	
}


template<typename GameType>
void Cheat<GameType>::search_address(std::uint8_t *current_ptr, GameType value,  MEMORY_BASIC_INFORMATION &m_i)
{

	std::size_t bytes = VirtualQueryEx(csProcess, current_ptr, &m_i, sizeof(m_i));
	if (m_i.State == MEM_COMMIT && m_i.Protect == PAGE_READWRITE)
	{
		std::vector<std::uint8_t> read_buffer(m_i.RegionSize);
		SIZE_T read_byte;
		if (ReadProcessMemory(csProcess, current_ptr, read_buffer.data(), m_i.RegionSize, &read_byte) == TRUE)
		{
			GameType* current_page_ptr = reinterpret_cast<GameType*>(read_buffer.data());
			while ((std::uint8_t*)current_page_ptr < read_buffer.data() + read_buffer.size())
			{
				if (*current_page_ptr == value)
				{
					addresses_value_type.push_back((std::uint32_t)(((std::uint8_t*)current_page_ptr -
						read_buffer.data()) + (std::uint8_t*)m_i.BaseAddress));

				}
				current_page_ptr = (GameType*)(((char*)current_page_ptr) + 1);
			}

		}
	}
	
}
template<typename GameType>
void Cheat<GameType>::_init()
{
	money_read_address = find_module_base(cl_module_name) + money_read_offset;
	y_read_address = find_module_base(hw_module_name) + y_read_offset;
}

template<typename GameType>
void Cheat<GameType>::scan_memory(std::uint32_t read_address,GameType value)
{

	SIZE_T size;
	ReadProcessMemory(csProcess, reinterpret_cast<void*>(read_address), &value, sizeof(value), &size);

	SYSTEM_INFO s_i;
	GetSystemInfo(&s_i);
	std::uint8_t* start_ptr = static_cast<std::uint8_t*>(s_i.lpMinimumApplicationAddress);
	std::uint8_t* end_ptr = static_cast<std::uint8_t*>(s_i.lpMaximumApplicationAddress);

	addresses_value_type.clear();
	std::uint8_t* current_ptr = start_ptr;

	while (current_ptr < end_ptr)
	{
		MEMORY_BASIC_INFORMATION m_i;
		search_address(current_ptr, value, m_i);
		current_ptr += m_i.RegionSize;
		
	}
	std::cout << "get_addresses_value_type().size(): " << addresses_value_type.size() << std::endl;
}





int main()
{
	Cheat<float> *cheat = new Cheat<float>;
	float y_value;
	//cheat->find_address(y_value);
	cheat->scan_memory(y_value);
	
	CloseHandle(cheat->get_csProcess());
	delete cheat;
	return 0;
}
