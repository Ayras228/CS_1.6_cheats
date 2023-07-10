#include "ScanAddress.h"
template<typename GameType>
GameType ScanAddress<GameType>::take_address(HANDLE csProcess, std::uint32_t read_address)
{
	GameType value;
	SIZE_T size;
	ReadProcessMemory(csProcess, reinterpret_cast<void*>(read_address), &value, sizeof(value), &size);

	return value;
}
template<typename GameType>
void ScanAddress<GameType>::search_address(HANDLE csProcess, std::uint8_t* current_ptr, GameType value, MEMORY_BASIC_INFORMATION& m_i)
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
					addresses_value.push_back((std::uint32_t)(((std::uint8_t*)current_page_ptr -
						read_buffer.data()) + (std::uint8_t*)m_i.BaseAddress));

				}
				current_page_ptr = (GameType*)(((char*)current_page_ptr) + 1);
			}

		}
	}

}
template<typename GameType>
void ScanAddress<GameType>::scan_memory(HANDLE csProcess, std::uint32_t read_address)
{
	//GameType value = take_address(csProcess, read_address);
	//SYSTEM_INFO s_i;
	//GetSystemInfo(&s_i);
	//std::uint8_t* start_ptr = static_cast<std::uint8_t*>(s_i.lpMinimumApplicationAddress);
	//std::uint8_t* end_ptr = static_cast<std::uint8_t*>(s_i.lpMaximumApplicationAddress);

	//addresses_value.clear();
	//std::uint8_t* current_ptr = start_ptr;

	//while (current_ptr < end_ptr)
	//{
	//	MEMORY_BASIC_INFORMATION m_i;
	//	search_address(csProcess, current_ptr, value, m_i);
	//	current_ptr += m_i.RegionSize;

	//}
	//std::cout << "get_addresses_value_type().size(): " << addresses_value.size() << std::endl;
}

template<typename GameType>
const std::vector<std::uint32_t> ScanAddress<GameType>::get_addresses_value()і
{
	return addresses_value;
}
