#include "Gui.h"

char *path[NUM_OF_MEMORY_SLOTS] = { "chess1.xml","chess2.xml","chess3.xml","chess4.xml","chess5.xml","chess6.xml","chess7.xml" };
moves empty_moves = { 0 };
extern moves error_moves;
extern move error_move;
extern cord error_cord;
move player_move = { -1, -1 };


gui_tree_node* window = NULL;
gui_tree_node* dialog_window = NULL;
SDL_Surface* buttons_img = NULL;
SDL_Surface* popup_img = NULL;
SDL_Surface* board_img = NULL;
SDL_Surface* tools_img = NULL;


int main_gui(){
	/*instead of long list below to do this short:
	reset_game(&game_settings);
	*/



	settings game_settings = { 0 };
	int cur_window, last_window;
	init_board(game_settings.board);
	game_settings.color = WHITE;
	game_settings.minimax_depth = 1;
	game_settings.next = WHITE;
	game_settings.mode = 1;
	/*
	buttons_img = SDL_LoadBMP("C:/Users/ishefi/Documents/GitHub/chess/Project3/sprites/buttons3.bmp");
	popup_img = SDL_LoadBMP("C:/Users/ishefi/Documents/GitHub/chess/Project3/sprites/popup2.bmp");
	board_img = SDL_LoadBMP("C:/Users/ishefi/Documents/GitHub/chess/Project3/sprites/board.bmp");
	tools_img = SDL_LoadBMP("C:/Users/ishefi/Documents/GitHub/chess/Project3/sprites/tools2.bmp");
	*/

	buttons_img = SDL_LoadBMP("sprites/buttons3.bmp");
	if (buttons_img == NULL){
		exit(0);
	}
	popup_img = SDL_LoadBMP("sprites/popup2.bmp");
	if (popup_img == NULL){
		exit(0);
	}
	board_img = SDL_LoadBMP("sprites/board.bmp");
	if (board_img == NULL){
		exit(0);
	}
	tools_img = SDL_LoadBMP("sprites/tools2.bmp");
	if (tools_img == NULL){
		exit(0);
	}
	cur_window = MAIN_WINDOW;
	last_window = MAIN_WINDOW;
	gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE];


	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		exit(0);

	cur_window = create_main_window(&game_settings);
	while (cur_window != FALSE){
		switch (cur_window){
		case (MAIN_WINDOW) :
			last_window = cur_window;
			reset_game(&game_settings);
			cur_window = create_main_window(&game_settings);
			break;
		case (PLAYER_SELECTION_WINDOW) :
			last_window = cur_window;
			cur_window = create_player_selection_window(&game_settings);
			break;
		case (LOAD_WINDOW) :
			last_window = cur_window;

			cur_window = FALSE;
			//cur_window = create_load_save_window(&game_settings, FALSE);
			//cur_window = create_dialog(&game_settings, dialog_window, NUM_OF_SLOTS, 200, 50, 0, 1200, GUI_LOAD);
			//free_tree(dialog_window);
			break;
		case (SAVE_WINDOW) :
			last_window = cur_window;
			cur_window = FALSE;
			//cur_window = create_load_save_window(&game_settings, TRUE);
			//cur_window = create_dialog(&game_settings, dialog_window, NUM_OF_SLOTS, 200, 50, 0, 1200, GUI_SAVE);
			//free_tree(dialog_window);
			break;
		case (SETTINGS_WINDOW) :
			last_window = cur_window;
			cur_window = create_settings_window(&game_settings);
			break;
		case (GAME_WINDOW) :
			last_window = cur_window;
			cur_window = create_game_window(&game_settings, board_tools);
			break;
		case (SET_BOARD_WINDOW) :
			last_window = cur_window;
			cur_window = create_set_board_window(&game_settings, board_tools);
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
	return 0;
}


// create the Main window
int create_main_window(settings *default_settings){
	gui_tree_node *panel, *new_game_button, *load_game_button, *quit_button, *last_window,
		*label;
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;

	if (!(create_panel(0, 0, 800, 600, window))){
		free_tree(last_window);
		return FALSE;
	}
	panel = window->children.last->data;
	if (!create_image(0, 0, 250, 0, 300, 100, buttons_img, panel)){
		free_tree(last_window);
		return FALSE;
	}
	label = panel->children.last->data;
	if (!create_button(0, 100, 300, 150, 200, 75, buttons_img, panel)){
		free_tree(last_window);
		return FALSE;
	}
	new_game_button = panel->children.last->data;
	if (!create_button(0, 175, 300, 250, 200, 75, buttons_img, panel)){
		free_tree(last_window);
		return FALSE;
	}
	load_game_button = panel->children.last->data;
	if (!create_button(0, 250, 300, 350, 200, 75, buttons_img, panel)){
		free_tree(last_window);
		return FALSE;
	}
	quit_button = panel->children.last->data;
	
	SDL_WM_SetCaption("Chess, The Game Of Kings", NULL);
	if (!draw_tree(window)){
		free_tree(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free_tree(last_window);
		return FALSE;
	}
	free_tree(last_window);
	return listener_to_main_window(default_settings, new_game_button, load_game_button, quit_button);
}

// the listener for the main window controls
int listener_to_main_window(settings *default_settings, gui_tree_node *new_game_button, 
							gui_tree_node *load_game_button, gui_tree_node *quit_button) {
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
				//return LOAD_WINDOW;
			//clicked_button can return: 0 = cancel was clicked, [1...] = the [i+1....]th save\load slots 
				int clicked_button = create_dialog(default_settings, dialog_window, NUM_OF_MEMORY_SLOTS,
																			200, 50, 0, 1200, GUI_LOAD);
				// error occured
				if (clicked_button == -1){
					free_tree(dialog_window);
					return FALSE;
				}
				if (clicked_button == 0){
					free_tree(dialog_window);
					return MAIN_WINDOW;
					break;
				}
				else{
					if (load_game(path[clicked_button - 1], default_settings)){
						free_tree(dialog_window);
						return PLAYER_SELECTION_WINDOW;
						break;
					}
					else{
						continue;
					}
				}
			}
			else if (is_inside_gui_tree_node(quit_button, eventt.button.x, eventt.button.y)) {
				return FALSE;
			}
		default:
			break;
		}
	}
	
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
	if (!create_panel(0, 0, 800, 600, window)){
		free(last_window);
		return FALSE;
	}
	panel = window->children.last->data;
	if (!create_image(0, 325, 250, 0, 300, 100, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	label = panel->children.last->data;
	if (!create_button(0, 425, 300, 150, 200, 75, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	player_vs_player_button = panel->children.last->data;
	if (!create_button(0, 500, 300, 250, 200, 75, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	player_vs_comp_button = panel->children.last->data;
	if (!create_button(0, 575, 300, 350, 200, 75, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	cancel_button = panel->children.last->data;
	SDL_WM_SetCaption("Player Selection", NULL);
	if (!draw_tree(window)){
		free(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free(last_window);
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
				return SET_BOARD_WINDOW;
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

	return FALSE;

}

// create the "Settings" window
int create_settings_window(settings *default_settings){
	gui_tree_node *panel, *color_white_button, *color_black_button, *next_playe_white_button,
		*next_playe_black_button,*cancel_button, *ok_button, *set_board_button,
		*label, *diff_label, *last_window;
	gui_tree_node *game_difficulty[5];
	int i, color_button_active, dif_button_active, next_player_button_active;
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 800, 600, window)){
		free(last_window);
		return FALSE;
	}
	panel = window->children.last->data;
	if (!create_image(0, 650, 250, 0, 300, 100, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	label = panel->children.last->data;
	if (!create_image(0, 1072, 50, 150, 200, 50, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	label = panel->children.last->data;

	//check if button should be active or not
	color_button_active = (default_settings->color == WHITE) ? ACTIVE : UNACTIVE;
	if (!create_button(color_button_active, 800, 300, 150, 100, 50, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	color_white_button = panel->children.last->data;

	//check if button should be active or not
	color_button_active = (default_settings->color == BLACK) ? ACTIVE : UNACTIVE;
	if (!create_button(color_button_active, 750, 400, 150, 100, 50, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	color_black_button = panel->children.last->data;
	if (!create_image(0, 1023, 50, 250, 200, 50, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	diff_label = panel->children.last->data;
	for (i = 0; i < 5; i++){
		if ((default_settings->minimax_depth == i + 1) || ((i == 4) && (default_settings->minimax_depth == 0)))
			dif_button_active = ACTIVE;
		else
			dif_button_active = UNACTIVE;
		if (!create_button(i * 50 + dif_button_active, 973, 290 + (i * 50), 250,
			50, 50, buttons_img, panel)){
			free(last_window);
			return FALSE;
		}
		game_difficulty[i] = panel->children.last->data;
	}
	if (!create_image(0, 1122, 50, 350, 200, 50, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	label = panel->children.last->data;

	//check if button should be active or not
	next_player_button_active = (default_settings->next == WHITE) ? ACTIVE : UNACTIVE;
	if (!create_button(next_player_button_active, 800, 300, 350, 100, 50, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	next_playe_white_button = panel->children.last->data;

	//check if button should be active or not
	next_player_button_active = (default_settings->next == BLACK) ? ACTIVE : UNACTIVE;
	if (!create_button(next_player_button_active, 750, 400, 350, 100, 50, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	next_playe_black_button = panel->children.last->data;
	if (!create_button(199, 899, 500, 500, 100, 75, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	set_board_button = panel->children.last->data;
	if (!create_button(100, 899, 200, 500, 100, 75, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	ok_button = panel->children.last->data;
	if (!create_button(0, 899, 350, 500, 100, 75, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	cancel_button = panel->children.last->data;
	SDL_WM_SetCaption("Settings Window", NULL);
	if (!draw_tree(window)){
		free(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free(last_window);
		return FALSE;
	}

	free_tree(last_window);
	return listener_to_Settings_window(default_settings, color_white_button, color_black_button,
		next_playe_black_button, next_playe_white_button,ok_button, cancel_button,
		set_board_button, game_difficulty);
}

// the listener for the player selection window controls
int listener_to_Settings_window(settings *default_settings, gui_tree_node *color_white_button,
	gui_tree_node *color_black_button, gui_tree_node *next_playe_black_button,
	gui_tree_node *next_playe_white_button, gui_tree_node *ok_button,gui_tree_node *cancel_button,
	gui_tree_node *set_board_button, gui_tree_node *game_difficulty[5]){
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
			else if (is_inside_gui_tree_node(next_playe_white_button, eventt.button.x, eventt.button.y)){
				default_settings->next = WHITE;
				return SETTINGS_WINDOW;
			}
			else if (is_inside_gui_tree_node(next_playe_black_button, eventt.button.x, eventt.button.y)){
				default_settings->next = BLACK;
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
					if (is_inside_gui_tree_node(game_difficulty[i], eventt.button.x, eventt.button.y)){
						default_settings->minimax_depth = (i == 4) ? 0 : i + 1;
						return SETTINGS_WINDOW;
					}
				}
			}
		default:
			break;
		}
	}
	return FALSE;

}

// create the "set board" window
int create_set_board_window(settings *game_settings, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE]){
	gui_tree_node *side_panel, *game_panel, *board_panel, *add_button, *move_button, *start_button, *remove_button, *cancel_button,
		/**tools_to_add,*/ * mask, *vertical_frame1, *vertical_frame2, *horizontal_frame1, *horizontal_frame2, *last_window;
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(866, 666);
	if (window == NULL)
		return FALSE;
	
	if (!create_panel(0, 0, 666, 666, window)){
		free_tree(last_window);
		return FALSE;
	}
	board_panel = window->children.last->data;
	if (!create_panel(666, 0, 200, 666, window)){
		free_tree(last_window);
		return FALSE;
	}
	side_panel = window->children.last->data;
	if (!create_button(0, 1800, 0, 0, 66, 75, buttons_img, side_panel)){
		free_tree(last_window);
		return FALSE;
	}
	add_button = side_panel->children.last->data;
	if (!create_button(66, 1800, 66, 0, 67, 75, buttons_img, side_panel)){
		free_tree(last_window);
		return FALSE;
	}
	remove_button = side_panel->children.last->data;
	if (!create_button(133, 1800, 133, 0, 67, 75, buttons_img, side_panel)){
		free_tree(last_window);
		return FALSE;
	}
	move_button = side_panel->children.last->data;
	if (!create_button(0, 1875, 0, 535, 100, 75, buttons_img, side_panel)){
		free_tree(last_window);
		return FALSE;
	}
	start_button = side_panel->children.last->data;
	if (!create_button(100, 1875, 100, 535, 100, 75, buttons_img, side_panel)){
		free_tree(last_window);
		return FALSE;
	}
	cancel_button = side_panel->children.last->data;
	//tools-bar on the side - should pop when add button is clicked
	/*if (!create_button(0, 1950, 25, 80, 150, 445, buttons_img, side_panel)){
		return FALSE;
	}
	tools_to_add = side_panel->children.last->data;*/

	if (!create_image(0, 330, 0, 0, 33, 666, tools_img, board_panel)){
		free_tree(last_window);
		return FALSE;
	}
	vertical_frame1 = board_panel->children.last->data;
	if (!create_image(0, 330, 633, 0, 33, 666, tools_img, board_panel)){
		free_tree(last_window);
		return FALSE;
	}
	vertical_frame2 = board_panel->children.last->data;
	if (!create_image(0, 1050, 33, 0, 600, 33, tools_img, board_panel)){
		free_tree(last_window);
		return FALSE;
	}
	horizontal_frame1 = board_panel->children.last->data;
	if (!create_image(0, 1050, 33, 633, 600, 33, tools_img, board_panel)){
		free_tree(last_window);
		return FALSE;
	}
	horizontal_frame2 = board_panel->children.last->data;
	if (!create_panel(33, 33, 600, 600, board_panel)){
		free_tree(last_window);
		return FALSE;
	}
	game_panel = board_panel->children.last->data;

	/*if (!create_button(150, 1875, 25, 80, 150, 445, buttons_img, side_panel)){
		return FALSE;
	}
	mask = side_panel->children.last->data;
	*/


	//draw the set window
	if (!draw_board(game_settings, game_panel, board_tools, empty_moves, FALSE, error_cord)){
		free(last_window);
		return FALSE;
	}
	SDL_WM_SetCaption("Game Window", NULL);
	if (!draw_tree(window)){
		free(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free(last_window);
		return FALSE;
	}

	//free_tree(last_window);
	//return listener_to_set_board_window(game_settings, game_panel, side_panel, add_button, move_button, start_button, remove_button, cancel_button, tools_to_add, board_tools, FALSE, error_cord,FALSE);// , &empty_moves, FALSE, &empty_moves);
	int next_window = listener_to_set_board_window(game_settings, game_panel, side_panel, add_button,
			move_button, start_button, remove_button, cancel_button, board_tools, FALSE, error_cord, FALSE);
	free_tree(last_window);
	return next_window;
}



// the listener for the set board window controls
// return the next window
int listener_to_set_board_window(settings *game_settings, gui_tree_node *game_panel,
		gui_tree_node *side_panel, gui_tree_node *add_button,gui_tree_node *move_button,
		gui_tree_node *start_button, gui_tree_node *remove_button,gui_tree_node *cancel_button,
		gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], int is_marked, cord marked_cord, int is_move){
	
	int frame_offset = 33;
	int i, j, piece_color, next_window;
	SDL_Event event;
	settings tmp_settings = { 0 };
	memcpy(&tmp_settings, game_settings, sizeof(settings));
	while (SDL_WaitEvent(&event)){
		switch (event.type){
		case(SDL_MOUSEBUTTONUP) :
			i = (event.button.x - frame_offset) / SQUARE_SIZE;
			j = 7 - ((event.button.y - frame_offset) / SQUARE_SIZE);
			cord piece_cord = { i, j };

			// the cancel button was clicked -> return to main window
			if (is_inside_gui_tree_node(cancel_button, event.button.x, event.button.y)){
				return MAIN_WINDOW;
			}
			
			// the start button was clicked -> go to game window
			else if (is_inside_gui_tree_node(start_button, event.button.x, event.button.y)){
				return GAME_WINDOW;
			}
			
			//there was a click on the game board
			if (is_inside_gui_tree_node(game_panel, event.button.x, event.button.y)){

				//check if there was a click inside the board controls
				if (is_inside_gui_tree_node(&board_tools[i][j], (event.button.x), (event.button.y))){
					
					//no piece is yet chosen in the board
					if (!is_marked){
						if (!mark_selected_cord(game_settings, game_panel, board_tools, piece_cord, FALSE)){
							return FALSE;
						}
						next_window = listener_to_set_board_window(game_settings, game_panel, side_panel,add_button,
							move_button, start_button, remove_button, cancel_button, board_tools, TRUE, piece_cord, FALSE);
					}
					
					//there is a chosen piece and move button was clicked 
					if (is_move){
						//if the target square in the board is not empty -> pop warning
						if (board_piece(game_settings->board, piece_cord) != EMPTY){
							if (!create_popup(&game_settings, 0, INVALID_SET_MASSAGE)){
								return FALSE;
							}
							return SET_BOARD_WINDOW;
						}
						char tool_to_move = tmp_settings.board[marked_cord.x][marked_cord.y];
						//check if no movement of pawn to rows 1 or 7
						if (((tool_to_move == 'm') || (tool_to_move == 'M')) && ((piece_cord.y == 0) || (piece_cord.y == 7))){
							if (!create_popup(&game_settings, 0, INVALID_SET_MASSAGE)){
								return FALSE;
							}
							return SET_BOARD_WINDOW;
						}
						board_piece(tmp_settings.board, marked_cord) = EMPTY;
						board_piece(tmp_settings.board, piece_cord) = tool_to_move;
						
						//check if this remove is valid
						if (!is_valid_for_set_board_window(tmp_settings.board)){
							if (!create_popup(&game_settings, 0, INVALID_SET_MASSAGE)){
								return FALSE;
							}
							return SET_BOARD_WINDOW;
						}
						//is valid -> remove piece
						else{ 
							board_piece(game_settings->board, marked_cord) = EMPTY;
							board_piece(game_settings->board, piece_cord) = tool_to_move;
							if (!draw_board(game_settings, game_panel, board_tools, empty_moves, FALSE, error_cord)){
								return FALSE;
							}
							SDL_WM_SetCaption("Set Board Window", NULL);
							if (!draw_tree(window)){
								return FALSE;
							}

							if (SDL_Flip(window->surface) != 0) {
								return FALSE;}
							return SET_BOARD_WINDOW;
						}
					}
					else{
						return SET_BOARD_WINDOW;}
				}
			}

			// move button was clicked
			else if ((is_inside_gui_tree_node(move_button, event.button.x, event.button.y)) && is_marked){
				if (!mark_selected_cord(game_settings, game_panel, board_tools, marked_cord, TRUE)){
					return FALSE;
				}
				next_window = listener_to_set_board_window(game_settings, game_panel, side_panel, add_button,
					move_button, start_button,remove_button, cancel_button, board_tools, TRUE, marked_cord, TRUE);
				return next_window;

			}

			// remove button was clicked
			else if ((is_inside_gui_tree_node(remove_button, event.button.x, event.button.y)) && is_marked){
				board_piece(tmp_settings.board, marked_cord) = EMPTY;
				//check if this remove is valid
				if (!is_valid_for_set_board_window(tmp_settings.board)){
					if (!create_popup(&game_settings, 0, INVALID_SET_MASSAGE)){
						return FALSE;
					}
				}
				else{ //is valid -> remove piece
					board_piece(game_settings->board, marked_cord) = EMPTY;
				}
				if (!draw_board(game_settings, game_panel, board_tools, empty_moves, FALSE, error_cord)){
					return FALSE;
				}
				SDL_WM_SetCaption("Set Board Window", NULL);
				if (!draw_tree(window)){
					return FALSE;
				}

				if (SDL_Flip(window->surface) != 0) {
					return FALSE;
				}
				return SET_BOARD_WINDOW;


			}

			// add button was clicked
			else if ((is_inside_gui_tree_node(add_button, event.button.x, event.button.y)) && is_marked){

				//if the target square in the board is not empty -> not valid
				if (board_piece(game_settings->board, marked_cord) != EMPTY){
					if (!create_popup(&game_settings, 0, INVALID_SET_MASSAGE)){
						return FALSE;
					}
					return SET_BOARD_WINDOW;
				}

				int clicked_button = create_dialog(game_settings, dialog_window, 12, SQUARE_SIZE, SQUARE_SIZE, 0, 1950, GUI_SET);
				// error occured
				if (clicked_button == -1){
					free_tree(dialog_window);
					dialog_window = NULL;
					return FALSE;
				}
				if (clicked_button == 0){
					free_tree(dialog_window);
					dialog_window = NULL;
					return MAIN_WINDOW;
					break;
				}
				else{
					char tool_type_to_add;
					switch (clicked_button){
						case(1) :
							tool_type_to_add = 'K';
							break; 
						case(2) :
							tool_type_to_add = 'Q';
							break;
						case(3) :
							tool_type_to_add = 'R';
							break;
						case(4) :
							tool_type_to_add = 'B';
							break;
						case(5) :
							tool_type_to_add = 'N';
							break;
						case(6) :
							tool_type_to_add = 'P';
							break;
						case(7) :
							tool_type_to_add = 'k';
							break;
						case(8) :
							tool_type_to_add = 'q';
							break;
						case(9) :
							tool_type_to_add = 'r';
							break;
						case(10) :
							tool_type_to_add = 'b';
							break;
						case(11) :
							tool_type_to_add = 'n';
							break;
						case(12) :
							tool_type_to_add = 'p';
							break;
					default:
						break;
					}

					board_piece(tmp_settings.board, marked_cord) = tool_type_to_add;
					//check if this add is valid
					if (!is_valid_for_set_board_window(tmp_settings.board)){
						if (!create_popup(&game_settings, 0, INVALID_SET_MASSAGE)){
							return FALSE;
						}
						return SET_BOARD_WINDOW;
					}
					else{ //is valid -> remove piece
						board_piece(game_settings->board, marked_cord) = tool_type_to_add;
						if (!draw_board(game_settings, game_panel, board_tools, empty_moves, FALSE, marked_cord)){
							return FALSE;
						}
						SDL_WM_SetCaption("Set Board Window", NULL);
						if (!draw_tree(window)){
							return FALSE;
						}

						if (SDL_Flip(window->surface) != 0) {
							return FALSE;
						}
						return SET_BOARD_WINDOW;
					}
				}

				/*
				//removing the mask
				gui_tree_node *curr_control = NULL;
				node * curr_node = side_panel->children.first;
				while (curr_node->next->next != NULL){
					curr_node = curr_node->next;
				}
				side_panel->children.last = curr_node;
				free(curr_node->next);
				curr_node->next = NULL;
				side_panel->children.len--;
				if (!draw_tree(window)){
					return FALSE;
				}

				if (SDL_Flip(window->surface) != 0) {
					return FALSE;
				}
				next_window = listener_to_set_board_window(game_settings, game_panel, side_panel, add_button, move_button, start_button, remove_button, cancel_button, tools_to_add, board_tools, TRUE, marked_cord, FALSE);
				return next_window;
			}
			else if (is_inside_gui_tree_node(tools_to_add, event.button.x, event.button.y)){
				char tool_type_to_add;
				int a = (event.button.x - 25 - 666) / SQUARE_SIZE;
				int b = (event.button.y - 80) / SQUARE_SIZE;
				switch (b)
				{
				case(0) :
					tool_type_to_add = (a == 0) ? 'K' : 'k';
					break;
				case(1) :
					tool_type_to_add = (a == 0) ? 'Q' : 'q';
					break;
				case(2) :
					tool_type_to_add = (a == 0) ? 'R' : 'r';
					break;
				case(3) :
					tool_type_to_add = (a == 0) ? 'B' : 'b';
					break;
				case(4) :
					tool_type_to_add = (a == 0) ? 'N' : 'n';
					break;
				case(5) :
					tool_type_to_add = (a == 0) ? 'P' : 'p';
					break;

				default:
					break;
				}
				settings tmp_settings = { 0 };
				memcpy(&tmp_settings, game_settings, sizeof(settings));
				board_piece(tmp_settings.board, marked_cord) = tool_type_to_add;
				//check if this remove is valid
				if (!is_valid_board(tmp_settings.board)){
					return SET_BOARD_WINDOW;
				}
				else{ //is valid -> remove piece
					board_piece(game_settings->board, marked_cord) = tool_type_to_add;
					if (!draw_board(game_settings, game_panel, board_tools, empty_moves, FALSE, error_cord)){
						return FALSE;
					}
					SDL_WM_SetCaption("Game Window", NULL);
					if (!draw_tree(window)){
						return FALSE;
					}

					if (SDL_Flip(window->surface) != 0) {
						return FALSE;
					}
					return SET_BOARD_WINDOW;
				}*/
			}
		default:
			break;
		}
	}
	return FALSE;

}

int create_popup(settings *default_settings, int popup_x_location, int popup_y_location){
	
	gui_tree_node *panel, *label, *last_window;
	last_window = window;
	//create panel
	if (!create_panel(125, 250, 400, 200, window)){
		free(last_window);
		return FALSE;
	}
	panel = window->children.last->data;
	//create the massage label
	if (!create_image(popup_x_location, popup_y_location, 0, 0, 400, 200, popup_img, panel)){
		free(last_window);
		return FALSE;
	}
	label = window->children.last->data;

	if (!draw_tree(window)){
		free(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free(last_window);
		return FALSE;
	}

	// set a delay of 3 sec
	SDL_Delay(3000);

	//free_tree(last_window);
	return TRUE;
}

//create a generic dialog
//it return -1 when error
//0 for click on cancel_button
//[i+1...] for the the [i....]th control
int create_dialog(settings *default_settings, gui_tree_node *dialog_window, int num_of_controls,
	int control_width, int control_hight, int label_location_x, int label_location_y,int special_dialog){
	


	gui_tree_node *panel, *controls_panel, *cancel_button, *label, *last_window;

	last_window = window;

	gui_tree_node **dialog_conrtol = (gui_tree_node**)malloc(num_of_controls*sizeof(gui_tree_node*));
	if (dialog_conrtol == NULL){
		return -1;
	}

	
	/*if (special_dialog == 1){
		num_of_controls = num_of_controls / 2;
	}*/
	int controls_panel_width = (special_dialog == 1) ? (control_width * 2) : control_width;
	int next_window, i, j;
	int special_dialog_active = 0;
	int special_set_active = 0;
	int label_hight = 100;
	int label_width = 300;
	int cancel_button_width = 75;
	int cancel_button_hight = 99;
	int window_width = label_width;
	int window_hight = label_hight + (num_of_controls*control_hight) + cancel_button_hight;
	//last_window = dialog_window;
	settings tmp_settings = { 0 };
	memcpy(&tmp_settings, default_settings, sizeof(settings));

	//create the dialog_window (panel)
	/*dialog_window = create_window(window_width, window_hight);
	if (dialog_window == NULL)
		return -1;
	gui_tree_node *panel, *label, *last_window;
	*/


	if (!create_panel(125, 50, window_width, window_hight, window)){
		free(last_window);
		free(dialog_conrtol);
		return FALSE;
	}
	/*dialog_panel = window->children.last->data;

	//  create a panel
	if (!create_panel(0, 0, window_width, window_hight, dialog_window)){
		free(last_window);
		free(dialog_conrtol);
		return -1;
	}*/
	panel = window->children.last->data;
	//create the dialog label
	if (!create_image(label_location_x, label_location_y, 0, 0, label_width, label_hight, buttons_img, panel)){
		free(last_window);
		free(dialog_conrtol);
		return -1;
	}
	label = panel->children.last->data;
	//create the cancel button
	if (!create_button(label_location_x, label_location_y + label_hight + (num_of_controls*control_hight), 100, 
		label_hight + (num_of_controls*control_hight), cancel_button_hight, cancel_button_width, buttons_img, panel)){
		free(last_window);
		free(dialog_conrtol);
		return -1;
	}
	cancel_button = panel->children.last->data;
	//create the controls panel (where the controls are located)
	if (!create_panel(108, label_hight, controls_panel_width, (num_of_controls*control_hight), panel)){
		free(last_window);
		free(dialog_conrtol);
		return -1;
	}
	controls_panel = panel->children.last->data;
	//create the controls
	/*if (special_dialog == 1){
		num_of_controls = num_of_controls * 2;
	}*/

	for (j = 0; j < num_of_controls; j++){
		i = j;
		/*if (special_dialog == 1){
			special_dialog_active = (j % 2) ? 75 : 0;
			i = (j / 2);
			special_set_active = special_dialog_active;
		}*/
		if (special_dialog>1){
			//check if button should be active or not
			special_dialog_active = (load_game(path[i], &tmp_settings)) ? ACTIVE : UNACTIVE;
			//save_load_button_active = ACTIVE;
		}
		if (!create_button(label_location_x + special_dialog_active, label_location_y + label_hight + (i * control_hight),
			special_set_active, (i * control_hight),control_width, control_hight, buttons_img, controls_panel)){
			free(last_window);
			free(dialog_conrtol);
			return -1;
		}
		gui_tree_node* control = (controls_panel->children.last->data);
		dialog_conrtol[i] = control;
	}


	
	//SDL_WM_SetCaption(dialog_header, NULL);
	

	// draw the GUI tree
	if (!draw_tree(window)){
		free(last_window);
		for (int i = 0; i < num_of_controls; i++)
			free(dialog_conrtol[i]);
		free(dialog_conrtol);
		return -1;
	}

	if (SDL_Flip(window->surface) != 0) {
		free(last_window);
		for (int i = 0; i < num_of_controls; i++)
			free(dialog_conrtol[i]);
		free(dialog_conrtol);
		return -1;
	}

	//free_tree(last_window);
	// call for the listener
	next_window = listener_to_dialog(dialog_conrtol, default_settings, cancel_button, controls_panel, num_of_controls, special_dialog);
	//free memory
	free_tree(last_window);
	/*for (int i = num_of_controls; i >0 ; i--)
		free(dialog_conrtol[i]);
	*/
	free(dialog_conrtol);

	return next_window;
}

int listener_to_dialog(gui_tree_node **dialog_conrtol, settings *default_settings,
	gui_tree_node *cancel_button, gui_tree_node *controls_panel, int num_of_controls, int special_dialog){
	SDL_Event eventt;
	int i;
	int special_dialog_active = 0;
	while (SDL_WaitEvent(&eventt)){
		switch (eventt.type){
		case(SDL_MOUSEBUTTONUP) :
			// if the cancel button was clicked - return 0
			if (is_inside_gui_tree_node(cancel_button, eventt.button.x, eventt.button.y)){
				return 0;
			}
			// if the "i"th button was clicked - return i+1
			if (is_inside_gui_tree_node(controls_panel, eventt.button.x, eventt.button.y)){
				for (i = 0; i < num_of_controls; i++){
					if (special_dialog == 1){
						//special_dialog_active = (i % 2) ? 75 : 0;
					}
					if (is_inside_gui_tree_node(dialog_conrtol[i], eventt.button.x - special_dialog_active,
																	eventt.button.y - special_dialog_active)){
						return i+1;
					}
				}
			}
		default:
			break;
		}
	}
	return -1;
}

// create the Save Load window
int create_load_save_window(settings *default_settings,int is_save){
	gui_tree_node *panel, *cancel_button, *set_board_button, *label, *last_window;
	gui_tree_node *slot[NUM_OF_MEMORY_SLOTS];
	int i, slot_button_active, is_save_window;
	last_window = window;
	settings tmp_settings = { 0 };
	memcpy(&tmp_settings, default_settings, sizeof(settings));

	
	// create the UI tree (all the controls) in the window
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 800, 600, window)){
		free(last_window);
		return FALSE;
	}
	panel = window->children.last->data;

	//check if this is a Save_Game window or a Load_Game window
	is_save_window = (is_save) ? ACTIVE : UNACTIVE;
	if (!create_image(is_save_window, 1200, 250, 0, 300, 100, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	label = panel->children.last->data;
	for (i = 0; i < NUM_OF_MEMORY_SLOTS; i++){
		//check if button should be active or not
		slot_button_active = (load_game(path[i], &tmp_settings)) ? ACTIVE : UNACTIVE;
		if (!create_button(slot_button_active, 1300 + (i * 50), 300, 125 + (i * 60),
			200, 50, buttons_img, panel))
			return FALSE;
		slot[i] = panel->children.last->data;
	}
	if (!create_button(0, 899, 550, 500, 100, 75, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	cancel_button = panel->children.last->data; 
	if (!create_button(199, 899, 150, 500, 100, 75, buttons_img, panel)){
		free(last_window);
		return FALSE;
	}
	set_board_button = panel->children.last->data;
	if (is_save_window){
		SDL_WM_SetCaption("Save Game Window", NULL);
	}
	if (!is_save_window){
		SDL_WM_SetCaption("Load Game Window", NULL);
	}
	if (!draw_tree(window)){
		free(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free(last_window);
		return FALSE;
	}

	free_tree(last_window);
	return listener_to_load_save_window(slot, default_settings, cancel_button, set_board_button, is_save);
}



// the listener for Save Load window controls
int listener_to_load_save_window(gui_tree_node *slot[4], settings *default_settings,
	gui_tree_node *cancel_button, gui_tree_node *set_board_button, int is_save){
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
				for (i = 0; i < NUM_OF_MEMORY_SLOTS; i++){
					if (is_inside_gui_tree_node(slot[i], eventt.button.x, eventt.button.y)){
						if (is_save){
							if (save_game(path[i], default_settings))
								return MAIN_WINDOW;
							else{
								continue;
							}
						}
						else{
							if (load_game(path[i], default_settings))
								return PLAYER_SELECTION_WINDOW;
							else{
								continue;
							}
						}
					}
				}
			}
		default:
			break;
		}
	}
	return FALSE;
}



// create the "Game window"
int create_game_window(settings *game_settings, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE]){
	
	gui_tree_node *side_panel, *game_panel, *board_panel, *save_button, *main_menu_button, 
		*get_best_move_button, *mask, *quit_button,*vertical_frame1, *vertical_frame2, *horizontal_frame1,
		*horizontal_frame2, *last_window;
	
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(888, 666);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 666, 666, window)){
		free(last_window);
		return FALSE;
	}
	board_panel = window->children.last->data;
	if (!create_panel(666, 0, 200, 666, window)){
		free(last_window);
		return FALSE;
	}
	side_panel = window->children.last->data;
	if (!create_button(0, 1725, 50, 10, 100, 75, buttons_img, side_panel)){
		free(last_window);
		return FALSE;
	}
	save_button = side_panel->children.last->data;
	if (!create_button(100, 1725, 50, 85, 100, 75, buttons_img, side_panel)){
		free(last_window);
		return FALSE;
	}
	main_menu_button = side_panel->children.last->data;
	if (!create_button(300, 1725, 50, 160, 100, 75, buttons_img, side_panel)){
		free(last_window);
		return FALSE;
	}
	get_best_move_button = side_panel->children.last->data;

	if (!create_button(200, 1725, 50, 500, 100, 75, buttons_img, side_panel)){
		free(last_window);
		return FALSE;
	}
	quit_button = side_panel->children.last->data;

	//create the frame around the game
	if (!create_image(0, 330, 0, 0, 33, 666, tools_img, board_panel)){
		free(last_window);
		return FALSE;
	}
	vertical_frame1 = board_panel->children.last->data;
	if (!create_image(0, 330, 633, 0, 33, 666, tools_img, board_panel)){
		free(last_window);
		return FALSE;
	}
	vertical_frame2 = board_panel->children.last->data;
	if (!create_image(0, 1050, 33, 0, 600, 33, tools_img, board_panel)){
		free(last_window);
		return FALSE;
	}
	horizontal_frame1 = board_panel->children.last->data;
	if (!create_image(0, 1050, 33, 633, 600, 33, tools_img, board_panel)){
		free(last_window);
		return FALSE;
	}
	horizontal_frame2 = board_panel->children.last->data;
	if (!create_panel(33, 33, 600, 600, board_panel)){
		free(last_window);
		return FALSE;
	}
	game_panel = board_panel->children.last->data;

	
	
	//draw the game window
	if (!draw_board(game_settings, game_panel, board_tools, empty_moves, FALSE, error_cord)){
		free(last_window);
		return FALSE;
	}
	SDL_WM_SetCaption("Game Window", NULL);
	if (!draw_tree(window)){
		free(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free(last_window);
		return FALSE;
	}

	//free_tree(last_window);
	 //game_flow(game_settings, game_panel, side_panel, save_button, main_menu_button, quit_button, board_tools, empty_moves);
		//before_listener(game_settings, game_panel, side_panel, save_button, main_menu_button, quit_button, board_tools, &empty_moves, FALSE, &empty_moves);
	int next_window = listener_to_game_window(game_settings, game_panel, side_panel, save_button, main_menu_button,
									quit_button, board_tools, get_best_move_button, &empty_moves, FALSE, &empty_moves);
	free_tree(last_window);
	return next_window;
}

// draw the current board state
int draw_board(settings *game_settings, gui_tree_node *panel,gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
												moves possible_moves, int is_comp, cord single_cord){
	int i, j;
	int board_frame = 0;
	int comp_turn = (is_comp) ? ACTIVE : UNACTIVE;
	move *curr_move = NULL;
	//free_tree_without_root(panel);
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			int is_active = UNACTIVE;
			int square_color = ((i + j) % 2 == 0) ? 0 : SQUARE_SIZE;
			gui_tree_node *cur_square = NULL;
			//if (possible_moves.len > 0){
			if (possible_moves.first != NULL){
				node * curr_node = possible_moves.first;
				// iterating over the possible moves to check what piece needs to be marked
				while (curr_node != NULL){
					curr_move = curr_node->data;
					// check if board_tools[i][j] == moves start\end
					if (((curr_move->end.x == i) && (curr_move->end.y == j)) ||
						((curr_move->start.x == i) && (curr_move->start.y == j)))
						is_active = ACTIVE + comp_turn;
					curr_node = curr_node->next;
				}
			}
			else if (single_cord.x != -1){
				if ((single_cord.x == i) && (single_cord.y == j)){
					is_active = ACTIVE + comp_turn;
				}
			}
			switch (game_settings->board[i][j]){
				/*make create_square*/
			case(WHITE_P) :
				if (!create_button(5 * SQUARE_SIZE + is_active, 3 * SQUARE_SIZE - square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(WHITE_B) :
				if (!create_button(3 * SQUARE_SIZE + is_active, 3 * SQUARE_SIZE - square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(WHITE_N) :
				if (!create_button(4 * SQUARE_SIZE + is_active, 3 * SQUARE_SIZE - square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(WHITE_R) :
				if (!create_button(2 * SQUARE_SIZE + is_active, 3 * SQUARE_SIZE - square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(WHITE_Q) :
				if (!create_button(1 * SQUARE_SIZE + is_active, 3 * SQUARE_SIZE - square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(WHITE_K) :
				if (!create_button(0 * SQUARE_SIZE + is_active, 3 * SQUARE_SIZE - square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_P) :
				if (!create_button(5 * SQUARE_SIZE + is_active, 0 * SQUARE_SIZE + square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_B) :
				if (!create_button(3 * SQUARE_SIZE + is_active, 0 * SQUARE_SIZE + square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_N) :
				if (!create_button(4 * SQUARE_SIZE + is_active, 0 * SQUARE_SIZE + square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_R) :
				if (!create_button(2 * SQUARE_SIZE + is_active, 0 * SQUARE_SIZE + square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_Q) :
				if (!create_button(1 * SQUARE_SIZE + is_active, 0 * SQUARE_SIZE + square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(BLACK_K) :
				if (!create_button(0 * SQUARE_SIZE + is_active, 0 * SQUARE_SIZE + square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
					return FALSE;
				cur_square = panel->children.last->data;
				board_tools[i][j] = *cur_square;
				break;
			case(EMPTY) :
				if (!create_button(6 * SQUARE_SIZE + is_active, square_color,
					board_frame + i * SQUARE_SIZE, board_frame + (7 * SQUARE_SIZE) - j * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, tools_img, panel))
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

int listener_to_game_window(settings *game_settings, gui_tree_node *game_panel, gui_tree_node *side_panel, gui_tree_node *save_button,
	gui_tree_node *main_menu_button, gui_tree_node *quit_button, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
	gui_tree_node *get_best_move_button, moves *all_piece_possible_moves, int to_move, moves *moves_of_piece){
	//gui_tree_node * last_window;
	SDL_Event event;
	int i, j, piece_color, next_window;
	int frame_offset = 33;
	//last_window = window;
	moves comp_moves = { 0 };

	check_castling_conditions(game_settings);
	int was_checked = FALSE;


	// comp turn
	if ((game_settings->mode == PLAYER_VS_COMP) && (game_settings->color != game_settings->next)){
		player_move = computer_turn(game_settings);
		if (player_move.promotion != NO_MOVE_CODE) {
			if (was_checked){
				if (!create_popup(&game_settings, 0, CHECK_MASSAGE)){
					return FALSE;
				}
			}
			
		}

		///next 2 lines needs to move to end with player_turn
		game_settings->next = other_player(game_settings->next);
		was_checked = game_settings->is_next_checked;
		//******************************************************//
		free_tree_without_root(game_panel);
		if (!add_node(&comp_moves, &player_move, sizeof(move))){
			free_list(&comp_moves, free);
			return FALSE;
		}
		if (comp_moves.len == -1){
			//free(last_window);
			return FALSE;
		}

		if (!draw_board(game_settings, game_panel, board_tools, comp_moves, TRUE, error_cord)){
			//free(last_window);
			return FALSE;
		}
		if (!draw_tree(window)){
			//free(last_window);
			return FALSE;
		}
		if (SDL_Flip(window->surface) != 0) {
			//free(last_window);
			return FALSE;
		}
		free_list(&comp_moves, free);
		//free_tree(last_window);
	}

	moves all_possible_moves = make_all_moves(game_settings);
	while (SDL_WaitEvent(&event)){
		switch (event.type){
		case(SDL_MOUSEBUTTONUP) :

			//check if there was a click on the game panel
			if (is_inside_gui_tree_node(game_panel, event.button.x, event.button.y)){
				i = (event.button.x - frame_offset) / SQUARE_SIZE;
				j = 7 - ((event.button.y - frame_offset) / SQUARE_SIZE);
				
				//check if there was a click inside the board controls
				if (is_inside_gui_tree_node(&board_tools[i][j], (event.button.x), (event.button.y))){
					cord piece_cord = { i, j };
					piece_color = (which_color(board_piece(game_settings->board, piece_cord)));
					
					//click on a piece inorder to start moving:
					// check that the click was on a relavant color-wise piece
					if (piece_color == game_settings->next){
						moves moves_for_piece = get_moves_for_piece(all_possible_moves, piece_cord);
						if (moves_for_piece.len == -1){
							free_list(&all_possible_moves,free);
							//free(last_window);
							return FALSE;
						}
						if (moves_for_piece.len == 0){ //there are no possible moves->clean markings
							if (!user_turn_mark_possible_moves(game_settings, game_panel, board_tools,
								piece_cord, moves_for_piece, FALSE)){

								free_list(&all_possible_moves, free);
								free_list(&moves_for_piece, free);
								//free_tree(last_window);
								return FALSE;
							}
							to_move = FALSE;
						}
						if (moves_for_piece.len > 0){ //mark possible moves
							if (!user_turn_mark_possible_moves(game_settings, game_panel, board_tools,
																	piece_cord, moves_for_piece, FALSE)){

								free_list(&all_possible_moves, free);
								free_list(&moves_for_piece, free);
								//free_tree(last_window);
								return FALSE;
							}
							to_move = TRUE;
						}
						next_window = listener_to_game_window(game_settings, game_panel, side_panel, save_button, main_menu_button, 
									quit_button, board_tools, get_best_move_button, &all_possible_moves, to_move, &moves_for_piece);

						//free all moves lists
						if (moves_of_piece->len>0){
							free_list(moves_of_piece, free);
						}
						if (all_piece_possible_moves->len>0){
							free_list(all_piece_possible_moves, free);
						}
						/*if (last_window->offset_x >= 0){
							free_tree(last_window);
							last_window->offset_x = -1;
						}*/

						return next_window;
					}

					//make a movement to piece_cord location:
					move *curr_move = get_selected_move(*moves_of_piece, piece_cord);
					if (curr_move != NULL){

						//promotion:
						if (curr_move->promotion == 1){
							int tool_color_shift = (piece_color == BLACK) ? 0 : 370;
							int clicked_button = create_dialog(&game_settings, dialog_window, 4, 75, 50, 0, 3024 + tool_color_shift, FALSE);
							// error occured
							if (clicked_button == -1){
								free_tree(dialog_window);
								return FALSE;
							}
							if (clicked_button == 0){
								free_tree(dialog_window);
								return MAIN_WINDOW;
								break;
							}
							char tool_type_to_promote ;
							switch (clicked_button){
							case(1) :
								tool_type_to_promote = (piece_color == BLACK) ? 'K' : 'k';
								break;
							case(2) :
								tool_type_to_promote = (piece_color == BLACK) ? 'Q' : 'q';
								break;
							case(3) :
								tool_type_to_promote = (piece_color == BLACK) ? 'R' : 'r';
								break;
							case(4) :
								tool_type_to_promote = (piece_color == BLACK) ? 'B' : 'b';
								break;
							case(5) :
								tool_type_to_promote = (piece_color == BLACK) ? 'N' : 'n';
								break;
							case(6) :
								tool_type_to_promote = (piece_color == BLACK) ? 'P' : 'p';
								break;
							default:
								break;
							}
						
							board_piece(curr_move->board, curr_move->end) = tool_type_to_promote;
						}
						// play
						move to_play = *curr_move;
						board_copy(to_play.board, game_settings->board);
						game_settings->is_next_checked = is_king_checked(other_player(game_settings->next), game_settings->board);

						free_tree_without_root(game_panel);
						if (!draw_board(game_settings, game_panel, board_tools, empty_moves, FALSE, error_cord)){
							//free(last_window);
							return FALSE;
						}
						if (!draw_tree(window)){
							//free(last_window);
							return FALSE;
						}
						if (SDL_Flip(window->surface) != 0) {
							//free(last_window);
							return FALSE;
						}

						//free_tree(last_window);
						//finishing settings
						game_settings->next = other_player(game_settings->next);
						was_checked = game_settings->is_next_checked;
						free_list(moves_of_piece, free);
						*moves_of_piece = empty_moves;

						next_window = listener_to_game_window(game_settings, game_panel, side_panel, save_button, main_menu_button, quit_button, board_tools, get_best_move_button, all_piece_possible_moves, to_move, moves_of_piece);
						/*if (moves_of_piece->len>0){
							free_list(moves_of_piece, free);
						}
						if (all_piece_possible_moves->len>0){
							free_list(all_piece_possible_moves, free);
						}
						if (last_window->offset_x > 0){
							free_tree(last_window);
						}*/

						
						return next_window;
					}

					//
					else {
						next_window = listener_to_game_window(game_settings, game_panel, side_panel, save_button, main_menu_button, quit_button, board_tools, get_best_move_button, all_piece_possible_moves, to_move, moves_of_piece);
						/*if (moves_of_piece->len>0){
							free_list(moves_of_piece, free);
						}
						if (all_piece_possible_moves->len>0){
							free_list(all_piece_possible_moves, free);
						}
						if (last_window->offset_x > 0){
							free_tree(last_window);
						}*/
						return next_window;
					}
				}
			}
			
				//click on side control
				else if (is_inside_gui_tree_node(main_menu_button, event.button.x, event.button.y)){
					/*if (moves_of_piece->len>0){
						free_list(moves_of_piece, free);
					}
					if (all_piece_possible_moves->len>0){
						free_list(all_piece_possible_moves, free);
					}
					if (last_window!=NULL){
						free_tree(last_window);
						last_window = NULL;
					}*/
					return MAIN_WINDOW;
				}
				//click on save control
				else if (is_inside_gui_tree_node(save_button, event.button.x, event.button.y)){
					//clicked_button can return: 0 = cancel was clicked, [1...] = the [i+1....]th save\load slots 
					int clicked_button = create_dialog(game_settings, dialog_window, NUM_OF_MEMORY_SLOTS, 200, 50, 600, 1200, GUI_SAVE);
					if (clicked_button == -1){// error occured
						//free_tree(window);
						//dialog_window = NULL;
						return FALSE;
					} 
					if (clicked_button == 0){// cancel button was clicked
						//free_tree(window);
						//dialog_window = NULL;
						return GAME_WINDOW;
						break;
					}
					else{ //one of the saving slots was clicked
						if (save_game(path[clicked_button - 1], game_settings)){
							//free_tree(window);
							//dialog_window = NULL;
							return GAME_WINDOW;
							break;
						}
						else{
							continue;
						}
					}
				}
				//click on best move control
				else if (is_inside_gui_tree_node(get_best_move_button, event.button.x, event.button.y)){
					moves best_moves = { 0 };
					move_node *node = NULL;
					node = (move_node*)malloc(sizeof(move_node));
					if (node == NULL){
						return FALSE;
					}
					move best_move = get_best_move(game_settings);
					node->data = &best_move;
					node->next = NULL;
					best_moves.first = node;
					best_moves.last = node;
					best_moves.len = 1;
					if (!user_turn_mark_possible_moves(game_settings, game_panel, board_tools, error_cord, best_moves, TRUE)){
						//free_tree(last_window);
						free(node);
						return FALSE;
					}

					free(node);
					/*
					//removing the mask
					gui_tree_node *curr_control = NULL;
					node * curr_node = side_panel->children.first;
					while (curr_node->next->next != NULL){
					curr_node = curr_node->next;
					}
					side_panel->children.last = curr_node;
					free(curr_node->next);
					curr_node->next = NULL;
					side_panel->children.len--;
					if (!draw_tree(window)){
					return FALSE;
					}

					if (SDL_Flip(window->surface) != 0) {
					return FALSE;
					}
					next_window = listener_to_set_board_window(game_settings, game_panel, side_panel, add_button, move_button, start_button, remove_button, cancel_button, tools_to_add, board_tools, TRUE, marked_cord, FALSE);
					return next_window;*/
				}
				else if (is_inside_gui_tree_node(quit_button, event.button.x, event.button.y)){
					return QUIT_WINDOW;
				}
		default:
			break;
			}
		}
		return FALSE;
}

int mark_selected_cord(settings *game_settings, gui_tree_node *game_panel, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], cord piece_cord, int move_mark){
	free_tree_without_root(game_panel);
	if (!draw_board(game_settings, game_panel, board_tools, error_moves, move_mark, piece_cord)){
		return FALSE;
	}
	if (!draw_tree(window)){
		return FALSE;
	}
	if (SDL_Flip(window->surface) != 0) {
		return FALSE;
	}

	return TRUE;
}


int user_turn_mark_possible_moves(settings *game_settings, gui_tree_node *game_panel, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], cord piece_cord, moves moves_for_piece, int blue_mark){
	free_tree_without_root(game_panel);
	if (!draw_board(game_settings, game_panel, board_tools, moves_for_piece, blue_mark, error_cord)){
		free_list(&moves_for_piece, free);
		return FALSE;
	}
	if (!draw_tree(window)){
		free_list(&moves_for_piece, free);
		return FALSE;
	}
	if (SDL_Flip(window->surface) != 0) {
		//printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		free_list(&moves_for_piece, free);
		return FALSE;
	}
	//free_tree(window); outside
	//free_list(&moves_for_piece, free);
	
	return TRUE;
}

move* get_selected_move(moves moves_of_piece, cord piece_cord){
	move *curr_move = NULL;
	node * curr_node = moves_of_piece.first;
	// iterating over the possible moves to check what piece needs to be marked
	while (curr_node != NULL){
		curr_move = curr_node->data;
		if (is_same_cord(curr_move->end, piece_cord))
			return curr_move;
		curr_node = curr_node->next;
	}
	return NULL;
}

moves set_comp_moves(move player_move){
	moves comp_moves = { 0 };
	node* comp_node = (node*)malloc(sizeof(node));
	if (comp_node == NULL){
		return error_moves;
	}
	comp_moves.first = comp_node;
	comp_node->next = NULL;
	comp_node->data = &player_move;
	comp_moves.last = comp_moves.first;
	comp_moves.len = 1;
	return comp_moves;
}

move* get_relevent_marked_move(cord piece_cord,moves *moves_for_piece){
	move *curr_move = NULL;
	node * curr_node = moves_for_piece->first;
	// iterating over the possible moves to check what piece needs to be marked
	while (curr_node != NULL){
		curr_move = curr_node->data;
		if (is_same_cord(curr_move->end, piece_cord))
			return curr_move;
		curr_node = curr_node->next;
	}
	free(curr_node);
	free(curr_move);
	return NULL;
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

void reset_game(settings *game_settings){
	int user_color = game_settings->color;
	game_settings->color = WHITE;
	game_settings->next = user_color;
	game_settings->minimax_depth = 1;
	clear(game_settings->board);
	init_board(game_settings->board);

}
