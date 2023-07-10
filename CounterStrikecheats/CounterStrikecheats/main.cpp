#include"Cheat.h"



int main()
{
	Cheat *cheat = new Cheat;
	CloseHandle(cheat->get_csProcess());
	delete cheat;

	return 0;
}
