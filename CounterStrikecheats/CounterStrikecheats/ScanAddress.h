#pragma once
#include"Library.h"
template<typename GameType>
class ScanAddress
{
public:
	void search(std::uint32_t read_address);
	ScanAddress(std::shared_ptr<HANDLE> csProcess);
	
private:
	void scan_memory(GameType value);
	std::uint32_t filter_address();
	GameType take_address(std::uint32_t read_address);
	void search_address(std::uint8_t* current_ptr, GameType value, MEMORY_BASIC_INFORMATION& m_i);
	std::vector<std::uint32_t> addresses_value;
	std::shared_ptr<HANDLE> p_csProcess;
};

