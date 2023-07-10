#pragma once
#include"Library.h"
template<typename GameType>
class ReadAddress
{
public:
	ReadAddress();
	~ReadAddress();
	void scan_memory(HANDLE csProcess, std::uint32_t read_address, GameType value);

private:

	void search_address(HANDLE csProcess, std::uint8_t* current_ptr, GameType value, MEMORY_BASIC_INFORMATION& m_i);
	std::vector<std::uint32_t> addresses_value_type;

};

