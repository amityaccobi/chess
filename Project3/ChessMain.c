#include "Console.h"
#include "Gui.h"

int main(int argc, char * argv[]) {
	srand((int)time(0));  
//	return gui_mode();
	if (argc==1 || (strcmp(argv[1], "console") == 0))
		return console_mode();
	else if ((strcmp(argv[1], "gui") == 0))
		return gui_mode();
	return 0;
}
