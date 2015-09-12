#include "Console.h"
#include "Gui.h"

int main(int argc, char * argv[]) {
	srand((int)time(0)); 
	printf("Console or GUI? (c/g)\n");
	char c[2];
	scanf("%s", c);
	if (c[0] == 'c')
		return console_mode();
	else 
		return main_gui();
	
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