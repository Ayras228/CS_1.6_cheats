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
	money = new ScanAddress<int>(csProcess);
	Y_coordinate = new ScanAddress<float>(csProcess);

	std::uint32_t  money_read_offset = money->get_money_read_offset();
	std::uint32_t Y_read_offset = Y_coordinate->get_Y_read_offset();

	money->set_money_read_address(find_module_base(cl_module_name) + money_read_offset);
	Y_coordinate->set_Y_read_address(find_module_base(hw_module_name) + Y_read_offset);
}

void Cheat::start()
{
	const std::uint32_t money_read_address = money->get_money_read_address();
	const std::uint32_t Y_read_address = Y_coordinate->get_Y_read_address();

	//money->search(money_read_address);
	Y_coordinate->search(Y_read_address);
	
	//money->filter_address(16000, money_read_address);
	Y_coordinate->filter_address(Y_coordinate->value + 200, Y_read_address);
}
Cheat::~Cheat()
{
	delete Y_coordinate;
	delete money;
}
template<typename GameType>
GameType ScanAddress<GameType>::take_address( std::uint32_t read_address)
{
	GameType value;
	
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
					addresses.push_back((std::uint32_t)(((std::uint8_t*)current_page_ptr -
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
	this->value = value;
	scan_memory(value);
}

template<typename GameType>
ScanAddress<GameType>::ScanAddress(std::shared_ptr<HANDLE> csProcess)
{
	this->p_csProcess = csProcess;
}

template<typename GameType>
void ScanAddress<GameType>::set_Y_read_address(std::uint32_t Y_read_address)
{
	this->Y_read_address = Y_read_address;
}

template<typename GameType>
void ScanAddress<GameType>::set_money_read_address(std::uint32_t money_read_address)
{
	this->money_read_address = money_read_address;
}

template<typename GameType>
void ScanAddress<GameType>::scan_memory(GameType value)
{
	
	SYSTEM_INFO s_i;
	GetSystemInfo(&s_i);
	std::uint8_t* start_ptr = static_cast<std::uint8_t*>(s_i.lpMinimumApplicationAddress);
	std::uint8_t* end_ptr = static_cast<std::uint8_t*>(s_i.lpMaximumApplicationAddress);

	addresses.clear();
	std::uint8_t* current_ptr = start_ptr;

	while (current_ptr < end_ptr)
	{
		MEMORY_BASIC_INFORMATION m_i;
		search_address(current_ptr, value, m_i);
		current_ptr += m_i.RegionSize;

	}
	std::cout << "addresses size: " << addresses.size() <<"\t value: "<< value << std::endl;	//"\t sizeof type: " << sizeof value <<
}
template<typename GameType>
void ScanAddress<GameType>::filter_address(GameType write_value, const std::uint32_t read_address)
{
	//std::vector<std::uint32_t> filter_addresses;
	for (int i = 0; i < addresses.size()-1; i++)
	{
		//std::regex const reg = "271\d+";
		std::regex const reg{ R"~(271\d+)~" };
		std::smatch m;

		std::string tmps = std::to_string(addresses.at(i));
		//if (std::regex_match(tmps, result, re)) { negative_flag = true; }

		if (std::regex_match(tmps, m, reg))
		{
			std::cout << addresses.at(i)<<std::endl;
			while (true)
			{
				WriteProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(addresses.at(i)), &write_value, sizeof write_value, &size);
			}
		}
//		if (addresses.at(i) != read_address)
//		{
//			
//			
//			
//
//			/*std::cout << "addresses: " << addresses.at(i) << std::endl;
//			auto start_time = std::chrono::system_clock::now();
//			ReadProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(read_address), &buffer1, sizeof buffer1, &size);
//			WriteProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(addresses.at(i)), &write_value, sizeof write_value, &size);
//			while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count() < 20);
//			ReadProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(read_address), &buffer2, sizeof buffer2, &size);
//*/
//
//		}
	}

}
//template<typename GameType>
//void ScanAddress<GameType>::filter_address(GameType write_value, const std::uint32_t read_address)
//{
//	//GameType write_value = 16000;
//	//std::uint32_t Y_write_address = 0;
//	std::uint32_t start = 0;
//	std::uint32_t end = addresses.size() - 1;
//	GameType buffer1;
//	GameType buffer2;
//	while (start < end-1)
//	{
//		std::uint32_t middle = (start + end) / 2;
//		std::vector<std::uint32_t> filter_addresses;
//		for (int i = start; i < middle; i++)
//		{
//			if (addresses.at(i) != read_address)
//			{
//
//				std::cout << "addresses: "<<addresses.at(i) << std::endl;
//				auto start_time = std::chrono::system_clock::now();
//				ReadProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(read_address), &buffer1, sizeof buffer1, &size);
//				WriteProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(addresses.at(i)), &write_value, sizeof write_value, &size);
//				while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count() < 20);
//				ReadProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(read_address), &buffer2, sizeof buffer2, &size);
//
//				if (buffer1-buffer2>1)
//				{
//					filter_addresses.push_back(addresses[i]);
//				}
//
//			}	
//		}
//
//		//auto start_time = std::chrono::system_clock::now();
//		//while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count() < 5000);
//		//buffer = 0;
//		//ReadProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(read_address), &buffer, sizeof buffer, &size);
//		//std::cout << "buffer: " << buffer << std::endl;
//		if (filter_addresses.empty())
//		{
//			start = middle;
//
//		}
//		else
//		{
//			std::cout << "true" << std::endl;
//			return;
//			end = middle;
//		}
//	}
//	//10337CFC			// 271809788
//}
 
//template<typename GameType>
//void ScanAddress<GameType>::filter_address(GameType write_value, const std::uint32_t read_address)
//{
//
//	std::uint32_t start = 0;
//	std::uint32_t end = addresses.size();
//	GameType buffer;
//	GameType buffer1;
//	float lam = 5;
//		for (int i = start; i < end; i++)
//		{
//			if (addresses.at(i) != read_address)
//			{
//				//std::cout << "addresses: " << addresses.at(i) << std::endl;
//				ReadProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(read_address), &buffer1, sizeof buffer1, &size);
//
//				WriteProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(addresses.at(i)), &write_value, sizeof write_value, &size);
//				std::this_thread::sleep_for(std::chrono::milliseconds(100));
//				ReadProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(read_address), &buffer, sizeof buffer, &size);
//				if (buffer1 - buffer > lam)
//				{
//					auto start_time = std::chrono::system_clock::now();
//					std::cout << "buffer1: " << buffer1 << "\t" << "buffer: " << buffer << std::endl;
//					std::cout << "addresses: " << addresses.at(i) << std::endl;
//					while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count() < 210)
//					{
//						WriteProcessMemory(*p_csProcess.get(), reinterpret_cast<void*>(addresses.at(i)), &write_value, sizeof write_value, &size);
//					}
//				}
//				
//			}
//		}
//		
//
//	
//
//}

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
