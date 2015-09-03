#include "Gui.h"


int main(){
	main_gui();
	return 0;
}

int main_gui(){

	extern gui_tree_node* window;
	extern SDL_Surface* buttons_img;
	extern SDL_Surface* board_img;
	extern SDL_Surface* tools_img;
	settings game_settings;
	int cur_window;
	init_board(game_settings.board);
	game_settings.color = WHITE;
	game_settings.minimax_depth = 1;
	game_settings.mode = 1;
	buttons_img = SDL_LoadBMP("sprites/buttons2.bmp");
	board_img = SDL_LoadBMP("sprites/board.bmp");
	tools_img = SDL_LoadBMP("sprites/tools.bmp");
	cur_window = MAIN_WINDOW;
	gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE];


	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
		exit(0);
	
	cur_window = create_main_window();
	while (cur_window != FALSE){
		switch (cur_window){
		case (MAIN_WINDOW) :
			cur_window = create_main_window();
			break;
		case (PLAYER_SELECTION_WINDOW) :
			cur_window = create_player_selection_window(&game_settings);
			break;
		case (LOAD_WINDOW) :
			cur_window = create_load_window(&game_settings);
			break;
		case (SAVE_WINDOW) :
			//cur_window = create_save_window();
			cur_window = QUIT;
			break;
		case (SETTINGS_WINDOW) :
			cur_window = create_settings_window(&game_settings);
			break;
		case (GAME_WINDOW) :
			cur_window = create_game_window(&game_settings, board_tools);
			break;
		case (QUIT_WINDOW) :
			//cur_window = create_quit_window();
			cur_window = QUIT;
			break;
		default:
			break;

		}
	}

	free_tree(window);
	SDL_FreeSurface(buttons_img);
	SDL_FreeSurface(board_img);
	SDL_FreeSurface(tools_img);
	SDL_Quit();
	return 1;
}

/*void create_board_tools(char board[BOARD_SIZE][BOARD_SIZE], gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			board_tools[i][j] = NULL;
				board[i][j];
		}
	}
}*/

// create the Main window
int create_main_window(){
	extern gui_tree_node* window;
	extern SDL_Surface* buttons_img;
	gui_tree_node *panel, *new_game_button, *load_game_button, *quit_button, *last_window,
																					*label;
	last_window = window;

	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!(create_panel(0, 0, 800, 600, window)))
		return FALSE;
	panel = window->children.last->data;
	if (!create_image(0, 0, 250, 0, 300, 100, buttons_img, panel))
		return FALSE;
	label = panel->children.last->data;
	if (!create_button(0, 100, 300, 150, 200, 75, buttons_img, panel))
		return FALSE;
	new_game_button = panel->children.last->data;
	if (!create_button(0, 175, 300, 250, 200, 75, buttons_img, panel))
		return FALSE;
	load_game_button = panel->children.last->data;
	if (!create_button(0, 250, 300, 350, 200, 75, buttons_img, panel))
		return FALSE;
	quit_button = panel->children.last->data;
	SDL_WM_SetCaption("this is a fucking owesome game", NULL);
	if (!draw_tree(window)){
		return FALSE;
	}
		
	if (SDL_Flip(window->surface) != 0) {
		//printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return FALSE;
	}
	free_tree(last_window);
	return listener_to_main_window(new_game_button, load_game_button, quit_button);
}

int listener_to_main_window(gui_tree_node *new_game_button, gui_tree_node *load_game_button,
															gui_tree_node *quit_button) {
	SDL_Event eventt;
	while (SDL_WaitEvent(&eventt)) {
		switch (eventt.type) {
		case (SDL_QUIT) :
			return FALSE;
		case (SDL_MOUSEBUTTONUP) :
			if (is_inside_gui_tree_node(new_game_button, eventt.button.x, eventt.button.y)) {
				return PLAYER_SELECTION_WINDOW;
			}
			else if (is_inside_gui_tree_node(load_game_button, eventt.button.x, eventt.button.y)) {
				return LOAD_WINDOW;
			}
			else if (is_inside_gui_tree_node(quit_button, eventt.button.x, eventt.button.y)) {
				return QUIT;
			}
		default:
			break;
		}
	}
	//printf("ERROR: %s\n", SDL_GetError());
	return FALSE;
}


// create the "Player Selection" window
int create_player_selection_window(settings *default_settings){
	extern gui_tree_node* window;
	extern SDL_Surface* buttons_img;
	gui_tree_node *panel, *player_vs_player_button, *player_vs_comp_button, *cancel_button,
																*label, *last_window;
	last_window = window;
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 800, 600, window))
		return FALSE;
	panel = window->children.last->data;
	if (!create_image(0, 325, 250, 0, 300, 100, buttons_img, panel))
		return FALSE;
	label = panel->children.last->data;
	if (!create_button(0, 425, 300, 150, 200, 75, buttons_img, panel))
		return FALSE;
	player_vs_player_button = panel->children.last->data;
	if (!create_button(0, 500, 300, 250, 200, 75, buttons_img, panel))
		return FALSE;
	player_vs_comp_button = panel->children.last->data;
	if (!create_button(0, 575, 300, 350, 200, 75, buttons_img, panel))
		return FALSE;
	cancel_button = panel->children.last->data;
	SDL_WM_SetCaption("Player Selection", NULL);
	if (!draw_tree(window))
		return FALSE;

	if (SDL_Flip(window->surface) != 0) {
		//printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return FALSE;
	}

	free_tree(last_window);
	return listener_to_player_selection_window(default_settings, player_vs_player_button,
													player_vs_comp_button, cancel_button);
}

int listener_to_player_selection_window(settings *default_settings, gui_tree_node 
		*player_vs_player_button, gui_tree_node *player_vs_comp_button, gui_tree_node *cancel_button){
	SDL_Event eventt;
	while (SDL_WaitEvent(&eventt)){
		switch (eventt.type){
		case(SDL_MOUSEBUTTONUP) :
			if (is_inside_gui_tree_node(player_vs_player_button, eventt.button.x, eventt.button.y)){
				default_settings->mode = PLAYER_VS_PLAYER;
				return GAME_WINDOW;
			}
			else if (is_inside_gui_tree_node(player_vs_comp_button, eventt.button.x, eventt.button.y)){
				default_settings->mode = PLAYER_VS_COMP;
				return SETTINGS_WINDOW;
			}
			else if (is_inside_gui_tree_node(cancel_button, eventt.button.x, eventt.button.y)){
				return MAIN_WINDOW;
			}
		default:
			break;
		}
	}
	//printf("ERROR: %s\n", SDL_GetError());
	return FALSE;

}

// create the "Settings" window
int create_settings_window(settings *default_settings){
	extern gui_tree_node* window;
	extern SDL_Surface* buttons_img;
	gui_tree_node *panel, *color_white_button, *color_black_button, *best_depth_button,
						*cancel_button, *ok_button, *label, *diff_label, *last_window;
	gui_tree_node *game_difficulty[4];
	int i, color_button_active, dif_button_active, best_active;
	last_window = window;
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 800, 600, window))
		return FALSE;
	panel = window->children.last->data;
	if (!create_image(0, 650, 250, 0, 300, 100, buttons_img, panel))
		return FALSE;
	label = panel->children.last->data;
	color_button_active = (default_settings->color == WHITE) ? 100 : 0;
	if (!create_button(color_button_active + 200, 750, 275, 150, 100, 75, buttons_img, panel))
		return FALSE;
	color_white_button = panel->children.last->data;
	color_button_active = (default_settings->color == BLACK) ? 100 : 0;
	if (!create_button(color_button_active, 750, 425, 150, 100, 75, buttons_img, panel))
		return FALSE;
	color_black_button = panel->children.last->data;
	best_active = (default_settings->minimax_depth == BEST_DIFFICULTY) ? 100 : 0;
	if (!create_button(best_active, 825, 350, 250, 100, 75, buttons_img, panel))
		return FALSE;
	best_depth_button = panel->children.last->data;
	if (!create_image(200, 975, 300, 350, 200, 50, buttons_img, panel))
		return FALSE;
	diff_label = panel->children.last->data;
	for (i = 0; i < 4; i++){
		dif_button_active = (default_settings->minimax_depth == i + 1) ? 50 : 0;
		if (!create_button(i * 50, 975 + dif_button_active, 290 + (i * 60), 415,
													50, 50, buttons_img, panel))
			return FALSE;
		game_difficulty[i] = panel->children.last->data;
	}
	if (!create_button(100, 900, 275, 500, 100, 75, buttons_img, panel))
		return FALSE;
	ok_button = panel->children.last->data;
	if (!create_button(0, 900, 425, 500, 100, 75, buttons_img, panel))
		return FALSE;
	cancel_button = panel->children.last->data;
	SDL_WM_SetCaption("Settings Window", NULL);
	if (!draw_tree(window))
		return FALSE;

	if (SDL_Flip(window->surface) != 0) {
		//printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return FALSE;
	}

	free_tree(last_window);
	return listener_to_Settings_window(default_settings, color_white_button,color_black_button,
								best_depth_button, ok_button, cancel_button, game_difficulty);
}


int listener_to_Settings_window(settings *default_settings, gui_tree_node *color_white_button,
	gui_tree_node *color_black_button, gui_tree_node *best_depth_button, gui_tree_node *ok_button,
								gui_tree_node *cancel_button, gui_tree_node *game_difficulty[4]){
	SDL_Event eventt;
	int i;
	while (SDL_WaitEvent(&eventt)){
		switch (eventt.type){
		case(SDL_MOUSEBUTTONUP) :
			if (is_inside_gui_tree_node(color_white_button, eventt.button.x, eventt.button.y)){
				default_settings->color = WHITE;
				return SETTINGS_WINDOW;
			}
			else if (is_inside_gui_tree_node(color_black_button, eventt.button.x, eventt.button.y)){
				default_settings->color = BLACK;
				return SETTINGS_WINDOW;
			}
			else if (is_inside_gui_tree_node(best_depth_button, eventt.button.x, eventt.button.y)){
				default_settings->minimax_depth = 0;
				return SETTINGS_WINDOW;
			}
			else if (is_inside_gui_tree_node(cancel_button, eventt.button.x, eventt.button.y)){
				//return all settings to default
				default_settings->minimax_depth = 1;
				default_settings->color = WHITE;
				return PLAYER_SELECTION_WINDOW;
			}
			else if (is_inside_gui_tree_node(ok_button, eventt.button.x, eventt.button.y)){
				return GAME_WINDOW;
			}
			else{
				for (i = 0; i < 4; i++){
					//gui_tree_node* curr_node = game_difficulty[i];
					if (is_inside_gui_tree_node(game_difficulty[i], eventt.button.x, eventt.button.y)){
						default_settings->minimax_depth = i + 1;
						return SETTINGS_WINDOW;
					}
				}
			}
		default:
			break;
		}
	}
	//printf("ERROR: %s\n", SDL_GetError());
	return FALSE;

}

// create the "Load window"
int create_load_window(settings *default_settings){
	extern gui_tree_node* window;
	extern SDL_Surface* buttons_img;
	gui_tree_node *panel, *cancel_button, *label, *last_window;
	gui_tree_node *slot[NUM_OF_SLOTS];
	int i, slot_button_active;
	last_window = window;
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 800, 600, window))
		return FALSE;
	panel = window->children.last->data;
	if (!create_image(0, 1075, 250, 0, 300, 100, buttons_img, panel))
		return FALSE;
	label = panel->children.last->data;
	for (i = 0; i < NUM_OF_SLOTS; i++){
		slot_button_active = (load_game(path[i*PATH_LENGTH], default_settings)) ? 0 : 200;
		if (!create_button(slot_button_active, 1175 + (i * 50), 300, 125 + (i * 60),
														200, 50, buttons_img, panel))
			return FALSE;
		slot[i] = panel->children.last->data;
	}
	if (!create_button(0, 900, 425, 500, 100, 75, buttons_img, panel))
		return FALSE;
	cancel_button = panel->children.last->data;
	SDL_WM_SetCaption("Load Game Window", NULL);
	if (!draw_tree(window))
		return FALSE;

	if (SDL_Flip(window->surface) != 0) {
		//printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return FALSE;
	}

	free_tree(last_window);
	return listener_to_load_window(slot, default_settings, cancel_button);
}

int listener_to_load_window(gui_tree_node *slot[4], settings *default_settings,
													gui_tree_node *cancel_button){
	SDL_Event eventt;
	int i;
	while (SDL_WaitEvent(&eventt)){
		switch (eventt.type){
		case(SDL_MOUSEBUTTONUP) :
			if (is_inside_gui_tree_node(cancel_button, eventt.button.x, eventt.button.y)){
				return MAIN_WINDOW;
			}
			else{
				for (i = 0; i < NUM_OF_SLOTS; i++){
					if (is_inside_gui_tree_node(slot[i * 10], eventt.button.x, eventt.button.y)){
						load_game(path[i*PATH_LENGTH], default_settings);
						return PLAYER_SELECTION_WINDOW;
					}
				}
			}
		default:
			break;
		}
	}
	//printf("ERROR: %s\n", SDL_GetError());
	return FALSE;
}


// create the "Game window"
int create_game_window(settings *game_settings, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE]){
	extern gui_tree_node* window;
	extern SDL_Surface* buttons_img;
	extern SDL_Surface* board_img;
	gui_tree_node *side_panel, *game_panel, *save_button, *main_menu_button, *quit_button, *last_window;
	last_window = window;
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 600, 600, window))
		return FALSE;
	game_panel = window->children.last->data;

	if (!create_panel(630, 0, 200, 600, window))
		return FALSE;
	side_panel = window->children.last->data;
	if (!create_button(0, 1525, 10, 10, 100, 75, buttons_img, side_panel))
		return FALSE;
	save_button = side_panel->children.last->data;
	if (!create_button(100, 1525, 10, 100, 100, 75, buttons_img, side_panel))
		return FALSE;
	main_menu_button = side_panel->children.last->data;
	if (!create_button(200, 1525, 10, 500, 100, 75, buttons_img, side_panel))
		return FALSE;
	quit_button = side_panel->children.last->data;

	if (!draw_board(game_settings, game_panel, board_tools))
		return FALSE;
	SDL_WM_SetCaption("Load Game Window", NULL);
	if (!draw_tree(window))
		return FALSE;

	if (SDL_Flip(window->surface) != 0) {
		//printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return FALSE;
	}

	free_tree(last_window);
	return listener_to_game_window(game_panel, side_panel, save_button, main_menu_button, quit_button, board_tools);
//	return listener_to_game_window(game_panel, &board_tools);
}

// draw the current board state
int draw_board(settings *game_settings, gui_tree_node *panel,
	gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE]){
	extern SDL_Surface* tools_img;
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			int square_color = ((i + j) % 2 == 0) ? 0 : SQUARE_SIZE;
			gui_tree_node *cur_square;
			switch (game_settings->board[i][j]){
				/*make create_square*/
			case(WHITE_P) :
				if (!create_button(5 * SQUARE_SIZE, 3 * SQUARE_SIZE - square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(WHITE_B) :
				if (!create_button(3 * SQUARE_SIZE, 3 * SQUARE_SIZE - square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(WHITE_N) :
				if (!create_button(4 * SQUARE_SIZE, 3 * SQUARE_SIZE - square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(WHITE_R) :
				if (!create_button(2 * SQUARE_SIZE, 3 * SQUARE_SIZE - square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(WHITE_Q) :
				if (!create_button(1 * SQUARE_SIZE, 3 * SQUARE_SIZE - square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(WHITE_K) :
				if (!create_button(0 * SQUARE_SIZE, 3 * SQUARE_SIZE - square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_P) :
				if (!create_button(5 * SQUARE_SIZE, 0 * SQUARE_SIZE + square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_B) :
				if (! create_button(3 * SQUARE_SIZE, 0 * SQUARE_SIZE + square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_N) :
				if(!create_button(4 * SQUARE_SIZE, 0 * SQUARE_SIZE + square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_R) :
				if(!create_button(2 * SQUARE_SIZE, 0 * SQUARE_SIZE + square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_Q) :
				if (!create_button(1 * SQUARE_SIZE, 0 * SQUARE_SIZE + square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_K) :
				if (!create_button(0 * SQUARE_SIZE, 0 * SQUARE_SIZE + square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(EMPTY) :
				if(!create_button(6 * SQUARE_SIZE, square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			default:
				break;
			}
		}
	}
	return TRUE;
}


void set_square_selected(gui_tree_node *board_tools){

}
void get_player_next_moves(gui_tree_node *board_tools){

}


int listener_to_game_window(gui_tree_node *game_panel, gui_tree_node *side_panel, gui_tree_node *save_button,
	gui_tree_node *main_menu_button, gui_tree_node *quit_button, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE]){
	SDL_Event eventt;
	int i, j;
	while (SDL_WaitEvent(&eventt)){
		switch (eventt.type){
		case(SDL_MOUSEBUTTONUP) :
			if (is_inside_gui_tree_node(game_panel, eventt.button.x, eventt.button.y)){
				i = eventt.button.x / SQUARE_SIZE;
				j = 7 - (eventt.button.y / SQUARE_SIZE);
				if (is_inside_gui_tree_node(&board_tools[i][j], eventt.button.x, eventt.button.y)){
					//need to add a check if he can click this square (if its his tools, and if they can move
					set_square_selected(&board_tools[i][j]); 
					get_player_next_moves(&board_tools[i][j]); //will call inside of it to set_square_selected for the relevant suggested next_moves 
					return GAME_WINDOW;
				}
			}
 			else if (is_inside_gui_tree_node(main_menu_button, eventt.button.x, eventt.button.y)){
				free_tree(side_panel->parent);
				return MAIN_WINDOW;
			}
			else if (is_inside_gui_tree_node(save_button, eventt.button.x, eventt.button.y)){
				return SAVE_WINDOW;
				}
			else if (is_inside_gui_tree_node(quit_button, eventt.button.x, eventt.button.y)){
				return QUIT_WINDOW;
			}
		default:
			break;
		}
	}
	//printf("ERROR: %s\n", SDL_GetError());
	return FALSE;
}