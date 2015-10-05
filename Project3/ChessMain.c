#include "Console.h"
#include "Gui.h"

int main(int argc, char * argv[]) {
	srand((int)time(0)); 
	if (argc == 1 || (strcmp(argv[1], "console") == 0))
		return console_mode();
	else if (strcmp(argv[1], "gui") == 0)
		return gui_mode();
	else
		printf("Invalid number of arguments. Please run with \"consol\", \"gui\" or none");
	return 0;
}
