#include "Gui.h"

char *path[NUM_OF_SLOTS] = { "chess2.xml" };


gui_tree_node* window = NULL;
SDL_Surface* buttons_img = NULL;
SDL_Surface* board_img = NULL;
SDL_Surface* tools_img = NULL;


int main_gui(){
	settings game_settings;
	int cur_window;
	init_board(game_settings.board);
	game_settings.color = WHITE;
	game_settings.minimax_depth = 1;
	game_settings.next = WHITE;
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
			cur_window = FALSE;
			break;
		case (SETTINGS_WINDOW) :
			cur_window = create_settings_window(&game_settings);
			break;
		case (GAME_WINDOW) :
			cur_window = create_game_window(&game_settings, board_tools);
			break;
		case (SET_BOARD_WINDOW) :
			//cur_window = create_set_board_window();
			cur_window = FALSE;
			break;
		case (QUIT_WINDOW) :
			//cur_window = create_quit_window();
			cur_window = FALSE;
			break;
		default:
			break;

		}
	}
	if (window != NULL)
		free_tree(window);
	SDL_FreeSurface(buttons_img);
	SDL_FreeSurface(board_img);
	SDL_FreeSurface(tools_img);
	SDL_Quit();
	return 1;
}


// create the Main window
int create_main_window(){
	gui_tree_node *panel, *new_game_button, *load_game_button, *quit_button, *last_window,
		*label;
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!(create_panel(0, 0, 800, 600, window)))
		return FALSE;
	panel = window->children.last->data;
	if (!create_image(0, 0, 250, 0, 300, 100, buttons_img, panel, UNACTIVE))
		return FALSE;
	label = panel->children.last->data;
	if (!create_button(0, 100, 300, 150, 200, 75, buttons_img, panel, UNACTIVE))
		return FALSE;
	new_game_button = panel->children.last->data;
	if (!create_button(0, 175, 300, 250, 200, 75, buttons_img, panel, UNACTIVE))
		return FALSE;
	load_game_button = panel->children.last->data;
	if (!create_button(0, 250, 300, 350, 200, 75, buttons_img, panel, UNACTIVE))
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

// the listener for the main window controls
int listener_to_main_window(gui_tree_node *new_game_button, gui_tree_node *load_game_button,
	gui_tree_node *quit_button) {
	SDL_Event eventt;
	while (SDL_WaitEvent(&eventt)) {
		switch (eventt.type) {
		case (SDL_QUIT) :
			return FALSE;
		//when mouse is clicked:
		case (SDL_MOUSEBUTTONUP) :
			if (is_inside_gui_tree_node(new_game_button, eventt.button.x, eventt.button.y)) {
				return PLAYER_SELECTION_WINDOW;
			}
			else if (is_inside_gui_tree_node(load_game_button, eventt.button.x, eventt.button.y)) {
				return LOAD_WINDOW;
			}
			else if (is_inside_gui_tree_node(quit_button, eventt.button.x, eventt.button.y)) {
				return FALSE;
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
	gui_tree_node *panel, *player_vs_player_button, *player_vs_comp_button, *cancel_button,
		*label, *last_window;
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 800, 600, window))
		return FALSE;
	panel = window->children.last->data;
	if (!create_image(0, 325, 250, 0, 300, 100, buttons_img, panel, UNACTIVE))
		return FALSE;
	label = panel->children.last->data;
	if (!create_button(0, 425, 300, 150, 200, 75, buttons_img, panel, UNACTIVE))
		return FALSE;
	player_vs_player_button = panel->children.last->data;
	if (!create_button(0, 500, 300, 250, 200, 75, buttons_img, panel, UNACTIVE))
		return FALSE;
	player_vs_comp_button = panel->children.last->data;
	if (!create_button(0, 575, 300, 350, 200, 75, buttons_img, panel, UNACTIVE))
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


// the listener for the player selection window controls
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
	gui_tree_node *panel, *color_white_button, *color_black_button,
		*cancel_button, *ok_button, *set_board_button, *label, *diff_label, *last_window;
	gui_tree_node *game_difficulty[4];
	int i, color_button_active, dif_button_active;
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 800, 600, window))
		return FALSE;
	panel = window->children.last->data;
	if (!create_image(0, 650, 250, 0, 300, 100, buttons_img, panel, UNACTIVE))
		return FALSE;
	label = panel->children.last->data;
	if (!create_image(0, 1072, 50, 150, 200, 50, buttons_img, panel, UNACTIVE))
		return FALSE;
	label = panel->children.last->data;
	//check if button should be active or not
	color_button_active = (default_settings->color == WHITE) ? ACTIVE : UNACTIVE;
	if (!create_button(color_button_active, 824, 275, 150, 100, 75, buttons_img, panel, color_button_active))
		return FALSE;
	color_white_button = panel->children.last->data;
	//check if button should be active or not
	color_button_active = (default_settings->color == BLACK) ? ACTIVE : UNACTIVE;
	if (!create_button(color_button_active, 750, 425, 150, 100, 75, buttons_img, panel, color_button_active))
		return FALSE;
	color_black_button = panel->children.last->data;
	if (!create_image(0, 1023, 50, 50, 250, 50, buttons_img, panel, UNACTIVE))
		return FALSE;
	diff_label = panel->children.last->data;
	for (i = 0; i < 5; i++){
		dif_button_active = (default_settings->minimax_depth == i + 1) ? ACTIVE : UNACTIVE;
		if (!create_button(i * 50, 973 + dif_button_active, 290 + (i * 60), 250,
			50, 50, buttons_img, panel, dif_button_active))
			return FALSE;
		game_difficulty[i] = panel->children.last->data;
	}
	if (!create_button(200, 899, 150, 500, 100, 75, buttons_img, panel, UNACTIVE))
		return FALSE;
	set_board_button = panel->children.last->data;
	if (!create_button(100, 899, 100, 500, 100, 75, buttons_img, panel, UNACTIVE))
		return FALSE;
	ok_button = panel->children.last->data;
	if (!create_button(0, 899, 300, 500, 100, 75, buttons_img, panel, UNACTIVE))
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
	return listener_to_Settings_window(default_settings, color_white_button, color_black_button,
		ok_button, cancel_button, set_board_button, game_difficulty);
}

// the listener for the player selection window controls
int listener_to_Settings_window(settings *default_settings, gui_tree_node *color_white_button,
	gui_tree_node *color_black_button, gui_tree_node *ok_button,
	gui_tree_node *cancel_button, gui_tree_node *set_board_button, gui_tree_node *game_difficulty[5]){
	SDL_Event eventt;
	int i;
	while (SDL_WaitEvent(&eventt)){
		switch (eventt.type){
		case(SDL_MOUSEBUTTONUP) :
			if (is_inside_gui_tree_node(color_white_button, eventt.button.x, eventt.button.y)){
				default_settings->color = WHITE;
				color_white_button->is_active = ACTIVE;
				return SETTINGS_WINDOW;
			}
			else if (is_inside_gui_tree_node(color_black_button, eventt.button.x, eventt.button.y)){
				default_settings->color = BLACK;
				color_black_button->is_active = ACTIVE;
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
			else if (is_inside_gui_tree_node(set_board_button, eventt.button.x, eventt.button.y)){
				return SET_BOARD_WINDOW;
			}
			else{
				for (i = 0; i < 5; i++){
					//gui_tree_node* curr_node = game_difficulty[i];
					if (is_inside_gui_tree_node(game_difficulty[i], eventt.button.x, eventt.button.y)){
						//default_settings->minimax_depth = i + 1;
						default_settings->minimax_depth = (i == 4) ? 0 : i + 1;
						game_difficulty[i] = ACTIVE;
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
	gui_tree_node *panel, *cancel_button, *set_board_button, *label, *last_window;
	gui_tree_node *slot[NUM_OF_SLOTS];
	int i, slot_button_active;
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 800, 600, window))
		return FALSE;
	panel = window->children.last->data;
	if (!create_image(0, 1200, 250, 0, 300, 100, buttons_img, panel, UNACTIVE))
		return FALSE;
	label = panel->children.last->data;
	for (i = 0; i < NUM_OF_SLOTS; i++){
		//check if button should be active or not
		slot_button_active = (load_game(path[i*PATH_LENGTH], default_settings)) ? UNACTIVE : ACTIVE;
		if (!create_button(slot_button_active, 1300 + (i * 50), 300, 125 + (i * 60),
			200, 50, buttons_img, panel, slot_button_active))
			return FALSE;
		slot[i] = panel->children.last->data;
	}
	if (!create_button(0, 899, 425, 500, 100, 75, buttons_img, panel, UNACTIVE))
		return FALSE;
	cancel_button = panel->children.last->data; 
	if (!create_button(200, 899, 150, 500, 100, 75, buttons_img, panel, UNACTIVE))
		return FALSE;
	set_board_button = panel->children.last->data;
	SDL_WM_SetCaption("Load Game Window", NULL);
	if (!draw_tree(window))
		return FALSE;

	if (SDL_Flip(window->surface) != 0) {
		//printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return FALSE;
	}

	free_tree(last_window);
	return listener_to_load_window(slot, default_settings, cancel_button, set_board_button);
}

// the listener for the player selection window controls
int listener_to_load_window(gui_tree_node *slot[4], settings *default_settings,
	gui_tree_node *cancel_button, gui_tree_node *set_board_button){
	SDL_Event eventt;
	int i;
	while (SDL_WaitEvent(&eventt)){
		switch (eventt.type){
		case(SDL_MOUSEBUTTONUP) :
			if (is_inside_gui_tree_node(cancel_button, eventt.button.x, eventt.button.y)){
				return MAIN_WINDOW;
			}
			else if (is_inside_gui_tree_node(set_board_button, eventt.button.x, eventt.button.y)){
				return SET_BOARD_WINDOW;
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
	gui_tree_node *side_panel, *game_panel, *save_button, *main_menu_button, *quit_button, *last_window;
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 600, 600, window))
		return FALSE;
	game_panel = window->children.last->data;

	if (!create_panel(630, 0, 200, 600, window))
		return FALSE;
	side_panel = window->children.last->data;
	if (!create_button(0, 1525, 10, 10, 100, 75, buttons_img, side_panel, UNACTIVE))
		return FALSE;
	save_button = side_panel->children.last->data;
	if (!create_button(100, 1525, 10, 100, 100, 75, buttons_img, side_panel, UNACTIVE))
		return FALSE;
	main_menu_button = side_panel->children.last->data;
	if (!create_button(200, 1525, 10, 500, 100, 75, buttons_img, side_panel, UNACTIVE))
		return FALSE;
	quit_button = side_panel->children.last->data;
	
	moves all_possible_moves = make_all_moves(*game_settings);
	if (all_possible_moves.len == -1){
		//free all controlls
		return FALSE;
	}
	if (!draw_board(game_settings, game_panel, board_tools, all_possible_moves))
		return FALSE;
	SDL_WM_SetCaption("Load Game Window", NULL);
	if (!draw_tree(window))
		return FALSE;

	if (SDL_Flip(window->surface) != 0) {
		//printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return FALSE;
	}

	free_tree(last_window);
	return listener_to_game_window(game_settings, game_panel, side_panel, save_button, main_menu_button, quit_button, board_tools, all_possible_moves);
	//	return listener_to_game_window(game_panel, &board_tools);
}

/*cord * extract_cords(moves possible_moves){
	int len = possible_moves.len;
	cord *to_activate;
	to_activate = (cord*)malloc(len * 2 * sizeof(cord));
	node * curr_node = possible_moves.first;
	move *curr_move;
	while (curr_node != NULL){
		curr_move = curr_node->data;
	}

			node * curr_node = possible_moves.first;
			move *curr_move;
			int mark_sign = SQUARE_SIZE;
			while (curr_node != NULL){
				curr_move = curr_node->data;
				char piece_char = board_piece(game_settings->board, curr_move->start);
				int piece_type = which_type(piece_char) * SQUARE_SIZE;
				int piece_color = which_color(piece_char) * SQUARE_SIZE;
				int square_color = ((curr_move->start.x + curr_move->start.y) % 2 == 0) ? 0 : SQUARE_SIZE;
				int x = curr_move->end.x * SQUARE_SIZE;
				int y = curr_move->end.y * SQUARE_SIZE;
				if (!create_button(piece_type, piece_color + square_color + mark_sign,
					x, (7 * SQUARE_SIZE) - y, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
			
		
	}
}*/

// draw the current board state
int draw_board(settings *game_settings, gui_tree_node *panel,
	gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], moves possible_moves){
	return 0;
}
/*
	int i, j;
	cord * to_activate_cords = extract_cords(possible_moves);
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			int square_color = ((i + j) % 2 == 0) ? 0 : SQUARE_SIZE;
			gui_tree_node *cur_square;
			cord cur_cord = { i, j };
			cord tmp_cord = *to_activate_cords;
			while ()
			switch (game_settings->board[i][j]){
				/*make create_square*/
/*			case(WHITE_P) :
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
				if (!create_button(3 * SQUARE_SIZE, 0 * SQUARE_SIZE + square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_N) :
				if (!create_button(4 * SQUARE_SIZE, 0 * SQUARE_SIZE + square_color,
					i*SQUARE_SIZE, (7 * SQUARE_SIZE) - j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_R) :
				if (!create_button(2 * SQUARE_SIZE, 0 * SQUARE_SIZE + square_color,
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
				if (!create_button(6 * SQUARE_SIZE, square_color,
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
*/

void set_square_selected(gui_tree_node *board_tools){

}
void get_player_next_moves(gui_tree_node *board_tools){

}

int listener_to_game_window(settings *game_settings, gui_tree_node *game_panel, gui_tree_node *side_panel, gui_tree_node *save_button,
	gui_tree_node *main_menu_button, gui_tree_node *quit_button, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
				moves all_possible_moves){
	SDL_Event eventt;
	int i, j;
	while (SDL_WaitEvent(&eventt)){
		switch (eventt.type){
		case(SDL_MOUSEBUTTONUP) :
			if (is_inside_gui_tree_node(game_panel, eventt.button.x, eventt.button.y)){
				i = eventt.button.x / SQUARE_SIZE;
				j = 7 - (eventt.button.y / SQUARE_SIZE);
				if (is_inside_gui_tree_node(&board_tools[i][j], eventt.button.x, eventt.button.y)){
					cord piece_cord = { i, j };
					int piece_color;
					piece_color = (which_color(board_piece(game_settings->board, piece_cord)));
					game_settings->color = piece_color;
					moves moves_for_piece = get_moves_for_piece(all_possible_moves, piece_cord);
					if (moves_for_piece.len == -1){
						return FALSE;
					}
					//mark_all_possible_moves(moves_for_piece, board_tools, game_settings, game_panel);
					//free possible moves
					free_list(&moves_for_piece, free);
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


int which_type(char c){
	if ((c == 'm') || (c == 'M'))
		return 5;
	if ((c == 'b') || (c == 'B'))
		return 3;
	if ((c == 'n') || (c == 'N'))
		return 4;
	if ((c == 'r') || (c == 'R'))
		return 2;
	if ((c == 'q') || (c == 'Q'))
		return 1;
	if ((c == 'k') || (c == 'K'))
		return 0;
	else
		return 6;
}

/*int mark_all_possible_moves(moves possible_moves, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
							settings *game_settings, gui_tree_node *panel){
	node * curr_node = possible_moves.first;
	move *curr_move;
	int mark_sign = SQUARE_SIZE;
	while (curr_node != NULL){
		curr_move = curr_node->data;
		char piece_char = board_piece(game_settings->board, curr_move->start);
		int piece_type = which_type(piece_char) * SQUARE_SIZE;
		int piece_color = which_color(piece_char) * SQUARE_SIZE;
		int square_color = ((curr_move->start.x + curr_move->start.y) % 2 == 0) ? 0 : SQUARE_SIZE;
		int x = curr_move->end.x * SQUARE_SIZE;
		int y = curr_move->end.y * SQUARE_SIZE;
		if (!create_button(piece_type, piece_color + square_color + mark_sign,
			x, (7 * SQUARE_SIZE) - y, SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
			return FALSE;

		gui_tree_node * cur_square = panel->children.last->data;
		memcpy(&board_tools[x][7 - y], cur_square);
		free(&board_tools[x][7 - y]);
		board_tools[x][7-y] = *cur_square;
	}
	return TRUE;
}*/
