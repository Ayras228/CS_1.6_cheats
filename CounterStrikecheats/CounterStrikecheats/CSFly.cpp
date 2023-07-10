
#include"Cheat.h"
#include"ReadAddress.h"




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
