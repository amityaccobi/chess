#include "Console.h"
#include "Gui.h"

int main(int argc, char * argv[]) {
	srand((int)time(0));  
	if (argc==1){
	printf("Console or GUI? (c/g)\n");
	char c[3];
	scanf("%s", c);
	if (c[0] == 'c')
		return console_mode();
	else 
		return main_gui();
	}
<<<<<<< HEAD
	
	if (argc == 2 && (strcmp(argv[1], "gui") == 0))
		return main_gui();
	else if (argc == 1 || strcmp(argv[1], "console") == 0)
=======
	if (argc > 1 && (strcmp(argv[1], "gui") == 0))
		return main_gui();
	else
>>>>>>> parent of 362def4... MASSAGE
		return console_mode();
	return 0;
}
