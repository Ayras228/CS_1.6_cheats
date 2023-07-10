#pragma once
#include"Library.h"
template<typename GameType>
class ScanAddress
{
public:

	void scan_memory(HANDLE csProcess, std::uint32_t read_address);
	const std::vector<std::uint32_t> get_addresses_value();
private:
	GameType take_address(HANDLE csProcess, std::uint32_t read_address);
	void search_address(HANDLE csProcess, std::uint8_t* current_ptr, GameType value, MEMORY_BASIC_INFORMATION& m_i);
	std::vector<std::uint32_t> addresses_value;

};

