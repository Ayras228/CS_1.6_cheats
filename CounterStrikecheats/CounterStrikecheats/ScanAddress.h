#pragma once
#include"Library.h"
template<typename GameType>
class ScanAddress
{
public:
	void search(std::uint32_t read_address);
	ScanAddress(std::shared_ptr<HANDLE> csProcess);
	void filter_address(GameType write_value, const std::uint32_t read_address);



	const std::uint32_t get_Y_read_offset() {return Y_read_offset;}
	const std::uint32_t get_money_read_offset() { return money_read_offset; }

	const std::uint32_t get_Y_read_address() { return Y_read_address; }
	const std::uint32_t get_money_read_address() { return money_read_address; }

	void set_Y_read_address(std::uint32_t Y_read_address);
	void set_money_read_address(std::uint32_t money_read_address);
	
	GameType value;
private:
	void scan_memory(GameType value);
	
	GameType take_address(std::uint32_t read_address);
	void search_address(std::uint8_t* current_ptr, GameType value, MEMORY_BASIC_INFORMATION& m_i);
	std::vector<std::uint32_t> addresses_value;
	std::shared_ptr<HANDLE> p_csProcess;

	SIZE_T size;
	

	std::uint32_t Y_read_address;
	std::uint32_t money_read_address;

	const std::uint32_t Y_read_offset = 0x9CF4C0;	//hw.dll+6C2048		/hw.dll+16C4E0	// nice	hw.dll+9F45A4	hw.dll+9CF4C0
	const std::uint32_t money_read_offset = 0x12F500;
};

