#pragma once
#include"Library.h"

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

