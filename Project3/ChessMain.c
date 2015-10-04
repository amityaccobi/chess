#include "Console.h"
#include "Gui.h"

int main(int argc, char * argv[]) {
	srand((int)time(0));  
	if (argc>1 && (strcmp(argv[1], "gui") == 0))
		return gui_mode();
	else
		return console_mode();

	return 0;
}
