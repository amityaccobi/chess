#include "Console.h"
#include "Gui.h"

int main(int argc, char * argv[]) {
	srand((int)time(0)); // for random choose between same-score moves

	return main_gui();
	//return console_mode();

	while (TRUE) {
		//	if (argc > 2) {
		//			printf("TWO ARGS PLEASE");
		//			break;
	//		}
	//	else if (argc == 1 || (strcmp(argv[1], "console") == 0))
	//	return console_mode();
//		else if (strcmp(argv[1], "gui") == 0)
			//continue;
	}
	return 0;
}