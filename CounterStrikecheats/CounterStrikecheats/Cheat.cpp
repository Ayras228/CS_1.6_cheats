#include "Cheat.h"



Cheat::Cheat()
{
	
	pid = find_cs_pid();
	std::cout << "pid: " << pid << std::endl;
	csProcess= std::make_shared<HANDLE>(OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid));
	if (csProcess == NULL)
	{
		throw std::exception("OpenProcess failed: ", pid);
	}

	_init();
	start();
}

void Cheat::_init()
{
	money_read_address = find_module_base(cl_module_name) + money_read_offset;
	Y_read_address = find_module_base(hw_module_name) + y_read_offset;
	
	money_read = new ScanAddress<int>(csProcess);
	Y_coordinate_read = new ScanAddress<float>(csProcess);
}

void Cheat::start()
{
	
	money_read->search( money_read_address);
	Y_coordinate_read->search( Y_read_address);


}
Cheat::~Cheat()
{
	delete Y_coordinate_read;
	delete money_read;
}
template<typename GameType>
GameType ScanAddress<GameType>::take_address( std::uint32_t read_address)
{
	GameType value;
	SIZE_T size;
	ReadProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(read_address), &value, sizeof(value), &size);

	return value;
}
template<typename GameType>
void ScanAddress<GameType>::search_address(std::uint8_t* current_ptr, GameType value, MEMORY_BASIC_INFORMATION& m_i)
{

	std::size_t bytes = VirtualQueryEx(*p_csProcess.get(), current_ptr, &m_i, sizeof(m_i));
	if (m_i.State == MEM_COMMIT && m_i.Protect == PAGE_READWRITE)
	{
		std::vector<std::uint8_t> read_buffer(m_i.RegionSize);
		SIZE_T read_byte;
		if (ReadProcessMemory(*p_csProcess.get(), current_ptr, read_buffer.data(), m_i.RegionSize, &read_byte) == TRUE)
		{
			GameType* current_page_ptr = reinterpret_cast<GameType*>(read_buffer.data());
			while ((std::uint8_t*)current_page_ptr < read_buffer.data() + read_buffer.size())
			{
				if (*current_page_ptr == value)
				{
					addresses_value.push_back((std::uint32_t)(((std::uint8_t*)current_page_ptr -
						read_buffer.data()) + (std::uint8_t*)m_i.BaseAddress));

				}
				current_page_ptr = (GameType*)(((char*)current_page_ptr) + 1);
			}

		}
	}

}
template<typename GameType>
void ScanAddress<GameType>::search( std::uint32_t read_address)
{
	GameType value = take_address( read_address);
	scan_memory(value);
}

template<typename GameType>
ScanAddress<GameType>::ScanAddress(std::shared_ptr<HANDLE> csProcess)
{
	this->p_csProcess = csProcess;
}

template<typename GameType>
void ScanAddress<GameType>::scan_memory(GameType value)
{
	
	SYSTEM_INFO s_i;
	GetSystemInfo(&s_i);
	std::uint8_t* start_ptr = static_cast<std::uint8_t*>(s_i.lpMinimumApplicationAddress);
	std::uint8_t* end_ptr = static_cast<std::uint8_t*>(s_i.lpMaximumApplicationAddress);

	addresses_value.clear();
	std::uint8_t* current_ptr = start_ptr;

	while (current_ptr < end_ptr)
	{
		MEMORY_BASIC_INFORMATION m_i;
		search_address(current_ptr, value, m_i);
		current_ptr += m_i.RegionSize;

	}
	std::cout << "addresses size: " << addresses_value.size() <<"\t value: "<< value << std::endl;	//"\t sizeof type: " << sizeof value <<
}

template<typename GameType>
std::uint32_t ScanAddress<GameType>::filter_address()
{
	for (const auto address_value :addresses_value)
	{
		/*if (WriteProcessMemory())
		{ }*/
	}
	return std::uint32_t();
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
//	return Y_read_address;
//}
//template<typename GameType>
//std::vector<std::uint32_t> Cheat<GameType>::get_addresses_value_type()
//{
//	return addresses_value_type;
//}

HANDLE Cheat::get_csProcess()
{
	return *csProcess.get();
}


std::uint32_t Cheat::find_cs_pid()
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

std::uint32_t Cheat::find_module_base(const wchar_t* module_name)
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
