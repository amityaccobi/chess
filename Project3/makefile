all: ChessMain

clean:
	-rm ChessMain ChessMain.o Console.o List.o Chess.o Gui.o GuiTree.o GuiUtils.o

ChessMain: List.o Chess.o Console.o GuiTree.o GuiUtils.o Gui.o ChessMain.o 
	gcc -o ChessMain ChessMain.o Console.o Chess.o List.o GuiTree.o GuiUtils.o Gui.o -lm -std=c99 -pedantic-errors -g -L/usr/local/stow/mxml-2.8/lib/mxml-2.8/lib -lmxml -lpthread `sdl-config --libs`

List.o: List.c
	gcc -std=c99 -pedantic-errors -c -g -lm List.c -I/usr/local/stow/mxml-2.8/lib/mxml-2.8/include `sdl-config --cflags`
	
Chess.o: Chess.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Chess.c -I/usr/local/stow/mxml-2.8/lib/mxml-2.8/include `sdl-config --cflags`

Console.o: Console.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Console.c -I/usr/local/stow/mxml-2.8/lib/mxml-2.8/include `sdl-config --cflags`

ChessMain.o: ChessMain.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm ChessMain.c -I/usr/local/stow/mxml-2.8/lib/mxml-2.8/include `sdl-config --cflags`

GuiUtils.o: GuiUtils.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm GuiUtils.c -I/usr/local/stow/mxml-2.8/lib/mxml-2.8/include `sdl-config --cflags`

GuiTree.o: GuiTree.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm GuiTree.c -I/usr/local/stow/mxml-2.8/lib/mxml-2.8/include `sdl-config --cflags`

Gui.o: Gui.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Gui.c -I/usr/local/stow/mxml-2.8/lib/mxml-2.8/include `sdl-config --cflags`

