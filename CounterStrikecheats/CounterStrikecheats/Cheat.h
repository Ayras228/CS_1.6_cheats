#pragma once
#include"Library.h"
#include"ScanAddress.h"

class Cheat
{
public:
	Cheat();
	~Cheat();
	//std::uint32_t get_pid();
	//std::uint32_t get_module_base_address();
	//std::uint32_t get_y_read_address();
	//std::vector<std::uint32_t> get_addresses_value_type();
	//void find_address(GameType & y_value);
	//void scan_memory(std::uint32_t read_address, GameType value
	HANDLE get_csProcess();
	
	
	
private:
	std::uint32_t find_cs_pid();
	std::uint32_t find_module_base(const wchar_t*  module_name);


	ScanAddress<float>* Y_coordinate_read;
	ScanAddress<int>* money_read;

	void start();
	void _init();
	std::uint32_t pid;
	std::shared_ptr<HANDLE>  csProcess;

	const wchar_t* cs_module_name = L"hl.exe";		//+ 0x016C4E0
	const wchar_t* hw_module_name = L"hw.dll";
	const wchar_t* cl_module_name = L"client.dll"; // + 0x12F500 money


	std::uint32_t Y_read_address;
	std::uint32_t money_read_address;
	
	const std::uint32_t y_read_offset = 0x16C4E0;
	const std::uint32_t money_read_offset = 0x12F500;

};

