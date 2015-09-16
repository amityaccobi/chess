#include "Gui.h"

char *path[NUM_OF_MEMORY_SLOTS] = { "chess1.xml","chess2.xml","chess3.xml","chess4.xml","chess5.xml","chess6.xml","chess7.xml" };
moves empty_moves = { 0 };
extern moves error_moves;
extern move error_move;
extern cord error_cord;
move empty_move = { 0, 0 };
move player_move = { -1, -1 };
int game_over = FALSE;
int is_castling_now = FALSE;
int checking_player = -1;
int check_recipient = WHITE;

gui_tree_node* window = NULL;
SDL_Surface* buttons_img = NULL;
SDL_Surface* popup_img = NULL;
SDL_Surface* board_img = NULL;
SDL_Surface* tools_img = NULL;
SDL_Surface* dialog_frame = NULL;


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
	dialog_frame = SDL_LoadBMP("sprites/dialog_frame.bmp");
	if (dialog_frame == NULL){
		exit(0);
	}
	cur_window = MAIN_WINDOW;
	last_window = MAIN_WINDOW;
	gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE];


	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		exit(0);

	cur_window = create_main_window(&game_settings);
	while (cur_window != QUIT_PROGRAM){
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
	gui_tree_node *panel, *new_game_button, *load_game_button, *quit_button, *last_window;
	default_settings->black_king_moved = 0;
	default_settings->black_rook_1_moved = 0;
	default_settings->black_rook_2_moved = 0;
	default_settings->color = WHITE;
	default_settings->is_next_checked = 0;
	default_settings->minimax_depth = 1;
	default_settings->mode = 1;
	default_settings->white_king_moved = 0;
	default_settings->white_rook_1_moved = 0;
	default_settings->white_rook_2_moved = 0;
	

	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	// create the panel
	if (!(create_panel(0, 0, 800, 600, window))){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	panel = window->children.last->data;
	//create the window label "main window"
	if (!create_image(0, 0, 250, 0, 300, 100, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	//create new game button 
	if (!create_button(0, 100, 300, 150, 200, 75, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	new_game_button = panel->children.last->data;
	// create load game button
	if (!create_button(0, 175, 300, 250, 200, 75, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	load_game_button = panel->children.last->data;
	// create quit game button
	if (!create_button(0, 250, 300, 350, 200, 75, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	quit_button = panel->children.last->data;
		
	SDL_WM_SetCaption("!Chessky, The Game Of Kings!", NULL);
	if (!draw_tree(window)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free_tree(last_window);
		//free_tree(last_window);
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
				int clicked_mode_button = create_dialog(default_settings, 2,
					200, 75, 0, 325, GUI_SET);
				// error occured
				if (clicked_mode_button == -1){
					return FALSE;
				}
				if (clicked_mode_button == 0){
					return MAIN_WINDOW;
					break;
				}
				switch (clicked_mode_button){
				case(1) :
					default_settings->mode = PLAYER_VS_PLAYER;
					break;
				case(2) :
					default_settings->mode = PLAYER_VS_COMP;
					break;
				default:
					break;
				}
				return SETTINGS_WINDOW;
			
			}
			else if (is_inside_gui_tree_node(load_game_button, eventt.button.x, eventt.button.y)) {
				//return LOAD_WINDOW;
			//clicked_button can return: 0 = cancel was clicked, [1...] = the [i+1....]th save\load slots 
				int clicked_button = create_dialog(default_settings, NUM_OF_MEMORY_SLOTS,
																			200, 50, 0, 1200, GUI_LOAD);
				// error occured
				if (clicked_button == -1){
					return FALSE;
				}
				if (clicked_button == 0){
					return MAIN_WINDOW;
					break;
				}
				else{
					if (load_game(path[clicked_button - 1], default_settings)){
						int clicked_mode_button = create_dialog(default_settings, 2,
							200, 75, 0, 325, GUI_SET);
						// error occured
						if (clicked_mode_button == -1){
							return FALSE;
						}
						if (clicked_mode_button == 0){
							return MAIN_WINDOW;
							break;
						}
						switch (clicked_mode_button){
						case(1) :
							default_settings->mode = PLAYER_VS_PLAYER;
							break;
						case(2) :
							default_settings->mode = PLAYER_VS_COMP;
							break;
						default:
							break;
						}
						return SETTINGS_WINDOW;
						break;
					}
					else{
						continue;
					}
				}
			}
			else if (is_inside_gui_tree_node(quit_button, eventt.button.x, eventt.button.y)) {
				int next_window = create_quit_dialog(default_settings, MAIN_WINDOW);
				return next_window;
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
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	panel = window->children.last->data;
	if (!create_image(0, 325, 250, 0, 300, 100, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	label = panel->children.last->data;
	if (!create_button(0, 425, 300, 150, 200, 75, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	player_vs_player_button = panel->children.last->data;
	if (!create_button(0, 500, 300, 250, 200, 75, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	player_vs_comp_button = panel->children.last->data;
	if (!create_button(0, 575, 300, 350, 200, 75, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	cancel_button = panel->children.last->data;
	SDL_WM_SetCaption("Player Selection", NULL);
	if (!draw_tree(window)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free_tree(last_window);
		//free_tree(last_window);
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
	gui_tree_node *panel, *next_player_button,
		*cancel_button, *ok_button, *set_board_button, *last_window;
	gui_tree_node *color_button = NULL;
	gui_tree_node *diff_button = NULL;


	//gui_tree_node *game_difficulty[5];
	int color, difficulty, next_player_color;
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(800, 600);
	if (window == NULL)
		return FALSE;
	if (!create_panel(0, 0, 800, 600, window)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	panel = window->children.last->data;
	//create the window header ("setting window")
	if (!create_image(0, 650, 250, 0, 300, 100, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	if (default_settings->mode == PLAYER_VS_COMP){
		//creating the label "user color"
		if (!create_image(0, 1072, 300, 200, 200, 50, buttons_img, panel)){
			free_tree(last_window);
			//free_tree(last_window);
			return FALSE;
		}

		//old version - check if button should be active or not
		//color_button_active = (default_settings->color == WHITE) ? ACTIVE : UNACTIVE;
		color = default_settings->color;
		/*if (!create_image(0, 750 + (50 * color), 300, 250, 100, 50, buttons_img, panel)){
			free_tree(last_window);
			return FALSE;
		}*/
		//create the color change button
		if (!create_button(0, 750 + (50 * color), 350, 250, 100, 50, buttons_img, panel)){
			free_tree(last_window);
			return FALSE;
		}
		color_button = panel->children.last->data;

		//check if button should be active or not
		/*color_button_active = (default_settings->color == BLACK) ? ACTIVE : UNACTIVE;
		if (!create_button(color_button_active, 750, 400, 150, 100, 50, buttons_img, panel)){
		free_tree(window);
		//free_tree(last_window);
		return FALSE;
		}
		color_black_button = panel->children.last->data;*/
		// create difficulty label
		if (!create_image(0, 1023, 300, 300, 200, 50, buttons_img, panel)){
			free_tree(last_window);
			//free_tree(last_window);
			return FALSE;
		}
		/*for (i = 0; i < 5; i++){
			if ((default_settings->minimax_depth == i + 1) || ((i == 4) && (default_settings->minimax_depth == 0)))
			dif_button_active = ACTIVE;
			else
			dif_button_active = UNACTIVE;
			if (!create_button(i * 50 + dif_button_active, 973, 290 + (i * 50), 250,
			50, 50, buttons_img, panel)){
			free_tree(window);
			//free_tree(last_window);
			return FALSE;
			}
			game_difficulty[i] = panel->children.last->data;
			}*/
		difficulty = default_settings->minimax_depth;
		// create difficulty buttonfree_tree(last_window);
		/*if (!create_image(difficulty * 50, 973, 350, 350,
			50, 50, buttons_img, panel)){
			free_tree(last_window);
			//free_tree(last_window);
			return FALSE;
		}*/
		// create difficulty change button
		if (!create_button(difficulty * 50, 973, 375, 350,
			50, 50, buttons_img, panel)){
			free_tree(last_window);
			//free_tree(last_window);
			return FALSE;
		}
		diff_button = panel->children.last->data;
	}
	//create next_player label
	if (!create_image(0, 1122, 300, 100, 200, 50, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}

	//check if button should be active or not
	next_player_color = default_settings->next;
	//next_player_button_active = (default_settings->next == WHITE) ? ACTIVE : UNACTIVE;
	/*if (!create_image(0, 750 + (50 * next_player_color), 300, 150, 100, 50, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}*/

	//create next player change button
	if (!create_button(0, 750 + (50 * next_player_color), 350, 150, 100, 50, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	next_player_button = panel->children.last->data;

	/*//check if button should be active or not
	next_player_button_active = (default_settings->next == BLACK) ? ACTIVE : UNACTIVE;
	if (!create_button(next_player_button_active, 750, 400, 350, 100, 50, buttons_img, panel)){
		free_tree(window);
		//free_tree(last_window);
		return FALSE;
	}
	next_playe_black_button = panel->children.last->data;
	*/
	//create "set board" button
	if (!create_button(199, 899, 500, 500, 100, 75, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	set_board_button = panel->children.last->data;
	//create ok button
	if (!create_button(100, 899, 200, 500, 100, 75, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	ok_button = panel->children.last->data;
	//create cancel button
	if (!create_button(0, 899, 350, 500, 100, 75, buttons_img, panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	cancel_button = panel->children.last->data;
	SDL_WM_SetCaption("Settings Window", NULL);
	if (!draw_tree(window)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}

	free_tree(last_window);
	return listener_to_Settings_window(default_settings, diff_button,
		ok_button, cancel_button, next_player_button, color_button,
		set_board_button);
}

// the listener for the player selection window controls
int listener_to_Settings_window(settings *default_settings, gui_tree_node *diff_button,
	gui_tree_node *ok_button, gui_tree_node *cancel_button, gui_tree_node *next_player_button,
	gui_tree_node *color_button, gui_tree_node *set_board_button){
	SDL_Event eventt;
	while (SDL_WaitEvent(&eventt)){
		switch (eventt.type){
		case(SDL_MOUSEBUTTONUP) :
			/*if (is_inside_gui_tree_node(color_button, eventt.button.x, eventt.button.y)){
				default_settings->color = WHITE;
				return SETTINGS_WINDOW;
			}*/
			if (is_inside_gui_tree_node(color_button, eventt.button.x, eventt.button.y)){
				int clicked_color_change_button = create_dialog(default_settings, 2,
					100, 50, 0, 3767, GUI_SET);
				// error occured
				if (clicked_color_change_button == -1){
					return FALSE;
				}
				if (clicked_color_change_button == 0){
					return SETTINGS_WINDOW;
					break;
				}
				switch (clicked_color_change_button){
				case(1) :
					default_settings->color = BLACK;
					break;
				case(2) :
					default_settings->color = WHITE;
					break;
				default:
					break;
				}
				return SETTINGS_WINDOW;
			}
			/*else if (is_inside_gui_tree_node(color_black_button, eventt.button.x, eventt.button.y)){
				default_settings->color = BLACK;
				return SETTINGS_WINDOW;
			}
			else if (is_inside_gui_tree_node(next_player_button, eventt.button.x, eventt.button.y)){
				default_settings->next = WHITE;
				return SETTINGS_WINDOW;
			}*/
			else if (is_inside_gui_tree_node(next_player_button, eventt.button.x, eventt.button.y)){
				int clicked_next_change_button = create_dialog(default_settings, 2,
					100, 50, 0, 3767, GUI_SET);
				// error occured
				if (clicked_next_change_button == -1){
					return FALSE;
				}
				if (clicked_next_change_button == 0){
					return SETTINGS_WINDOW;
					break;
				}
				switch (clicked_next_change_button){
				case(1) :
					default_settings->next = BLACK;
					break;
				case(2) :
					default_settings->next = WHITE;
					break;
				default:
					break;
				}
				return SETTINGS_WINDOW;
			}
			/*else if (is_inside_gui_tree_node(next_playe_black_button, eventt.button.x, eventt.button.y)){
				default_settings->next = BLACK;
				return SETTINGS_WINDOW;
			}*/
			else if (is_inside_gui_tree_node(cancel_button, eventt.button.x, eventt.button.y)){
				//return all settings to default
				default_settings->minimax_depth = 1;
				default_settings->color = WHITE;
				return MAIN_WINDOW;
			}
			else if (is_inside_gui_tree_node(ok_button, eventt.button.x, eventt.button.y)){
				check_castling_conditions(default_settings);
				return GAME_WINDOW;
			}
			else if (is_inside_gui_tree_node(set_board_button, eventt.button.x, eventt.button.y)){
				return SET_BOARD_WINDOW;
			}
			/*else if{
				for (i = 0; i < 5; i++){
					if (is_inside_gui_tree_node(game_difficulty, eventt.button.x, eventt.button.y)){
						default_settings->minimax_depth = (i == 4) ? 0 : i + 1;
						return SETTINGS_WINDOW;
					}
				}
			}*/
			else if (is_inside_gui_tree_node(diff_button, eventt.button.x, eventt.button.y)){
				int clicked_diff_change_button = create_dialog(default_settings, 5,
					50, 50, 0, 4042, GUI_SET);
				// error occured
				if (clicked_diff_change_button == -1){
					return FALSE;
				}
				if (clicked_diff_change_button == 0){
					return SETTINGS_WINDOW;
					break;
				}
				switch (clicked_diff_change_button){
				case(1) :
					default_settings->minimax_depth = 0;
					break;
				case(2) :
					default_settings->minimax_depth = 1;
					break;
				case(3) :
					default_settings->minimax_depth = 2;
					break;
				case(4) :
					default_settings->minimax_depth = 3;
					break;
				case(5) :
					default_settings->minimax_depth = 4;
					break;
				default:
					break;
				}
				return SETTINGS_WINDOW;
			}
		default:
			break;
		}
	}
	return FALSE;

}

// create the "set board" window
int create_set_board_window(settings *game_settings, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE]){
	gui_tree_node *side_panel, *game_panel, *board_panel, *add_button, *move_button, *remove_button, *clear_button,
		*start_button, *cancel_button, *last_window;
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(866, 666);
	if (window == NULL)
		return FALSE;
	//create the board panel
	if (!create_panel(0, 0, 666, 666, window)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	board_panel = window->children.last->data;
	//create the side panel
	if (!create_panel(666, 0, 200, 666, window)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	side_panel = window->children.last->data;
	//create the add button
	if (!create_button(0, 1800, 0, 0, 100, 75, buttons_img, side_panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	add_button = side_panel->children.last->data;
	//create the remove button
	if (!create_button(100, 1800, 100, 0, 100, 75, buttons_img, side_panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	remove_button = side_panel->children.last->data;
	// create the move button
	if (!create_button(200, 1800, 0, 75, 100, 75, buttons_img, side_panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	move_button = side_panel->children.last->data;
	//create the clear button
	if (!create_button(300, 1800, 100, 75, 100, 75, buttons_img, side_panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	clear_button = side_panel->children.last->data;
	//create start button
	if (!create_button(0, 1875, 0, 535, 100, 75, buttons_img, side_panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	start_button = side_panel->children.last->data;
	//create the cancel button
	if (!create_button(100, 1875, 100, 535, 100, 75, buttons_img, side_panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	cancel_button = side_panel->children.last->data;
	//tools-bar on the side - should pop when add button is clicked
	/*if (!create_button(0, 1950, 25, 80, 150, 445, buttons_img, side_panel)){
		return FALSE;
	}
	tools_to_add = side_panel->children.last->data;*/
	//create the board frames (4 frames - 2 vertical+2 horizontal)
	if (!create_image(0, 330, 0, 0, 33, 666, tools_img, board_panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	if (!create_image(0, 330, 633, 0, 33, 666, tools_img, board_panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	if (!create_image(0, 1050, 33, 0, 600, 33, tools_img, board_panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	if (!create_image(0, 1050, 33, 633, 600, 33, tools_img, board_panel)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	//create the game board
	if (!create_panel(33, 33, 600, 600, board_panel)){
		free_tree(last_window);
		//free_tree(last_window);
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
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}
	SDL_WM_SetCaption("Set Board Window", NULL);
	if (!draw_tree(window)){
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free_tree(last_window);
		//free_tree(last_window);
		return FALSE;
	}

	free_tree(last_window);
	//return listener_to_set_board_window(game_settings, game_panel, side_panel, add_button, move_button, start_button, remove_button, cancel_button, tools_to_add, board_tools, FALSE, error_cord,FALSE);// , &empty_moves, FALSE, &empty_moves);
	int next_window = listener_to_set_board_window(game_settings, game_panel, side_panel, add_button, move_button, 
				remove_button, clear_button, start_button, cancel_button, board_tools, FALSE, error_cord, FALSE);
	
	return next_window;
}



// the listener for the set board window controls
// return the next window
int listener_to_set_board_window(settings *game_settings, gui_tree_node *game_panel,
	gui_tree_node *side_panel, gui_tree_node *add_button, gui_tree_node *move_button, gui_tree_node *remove_button, gui_tree_node *clear_button,
		gui_tree_node *start_button, gui_tree_node *cancel_button,
		gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], int is_marked, cord marked_cord, int is_move){
	
	int frame_offset = 33;
	int i, j, next_window;
	// int piece_color;
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
				check_castling_conditions(game_settings);
				return GAME_WINDOW;
			}
			// the clear button was clicked -> clear the board
			else if (is_inside_gui_tree_node(clear_button, event.button.x, event.button.y)){
				clear(game_settings->board);
				cord black_king = { 4, 7 };
				cord white_king = { 4, 0 };
				board_piece(game_settings->board, black_king) = 'K';
				board_piece(game_settings->board, white_king) = 'k';
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
			
			//there was a click on the game board
			if (is_inside_gui_tree_node(game_panel, event.button.x, event.button.y)){

				//check if there was a click inside the board controls
				if (is_inside_gui_tree_node(&board_tools[i][j], (event.button.x), (event.button.y))){
					
					//no piece is yet chosen in the board
					if (!is_marked){
						if (!mark_selected_cord(game_settings, game_panel, board_tools, piece_cord, FALSE)){
							return FALSE;
						}
						next_window = listener_to_set_board_window(game_settings, game_panel, side_panel, add_button, move_button,
							remove_button, clear_button, start_button, cancel_button, board_tools, TRUE, piece_cord, FALSE);
						return next_window;
					}
					
					//there is a chosen piece and move button was clicked 
					if (is_move){
						//if the target square in the board is not empty -> pop warning
						/*if (board_piece(game_settings->board, piece_cord) != EMPTY){
							if (!create_popup(game_settings, 0, INVALID_SET_MASSAGE)){
								return FALSE;
							}
							return SET_BOARD_WINDOW;
						}*/
						char tool_to_move = tmp_settings.board[marked_cord.x][marked_cord.y];
						//check if no movement of pawn to rows 1 or 7
						if (((tool_to_move == 'm') || (tool_to_move == 'M')) && ((piece_cord.y == 0) || (piece_cord.y == 7))){
							if (!create_popup(game_settings, 0, INVALID_SET_MASSAGE)){
								return FALSE;
							}
							return SET_BOARD_WINDOW;
						}
						board_piece(tmp_settings.board, marked_cord) = EMPTY;
						board_piece(tmp_settings.board, piece_cord) = tool_to_move;
						
						//check if this remove is valid
						if (!is_valid_for_set_board_window(tmp_settings.board)){
							if (!create_popup(game_settings, 0, INVALID_SET_MASSAGE)){
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
				next_window = listener_to_set_board_window(game_settings, game_panel, side_panel, add_button, move_button,
					remove_button, clear_button, start_button, cancel_button, board_tools, TRUE, marked_cord, TRUE);
				return next_window;

			}

			// remove button was clicked
			else if ((is_inside_gui_tree_node(remove_button, event.button.x, event.button.y)) && is_marked){
				board_piece(tmp_settings.board, marked_cord) = EMPTY;
				//check if this remove is valid
				if (!is_valid_for_set_board_window(tmp_settings.board)){
					if (!create_popup(game_settings, 0, INVALID_SET_MASSAGE)){
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

				char tool_type_to_add;
				//if the target square in the board is not empty -> not valid
				/*if (board_piece(game_settings->board, marked_cord) != EMPTY){
					if (!create_popup(game_settings, 0, INVALID_SET_MASSAGE)){
						return FALSE;
					}
					return SET_BOARD_WINDOW;
				}*/

				//selecting the tool color you wish to add (dialog)
				int clicked_color_button = create_dialog(game_settings, 2,
					100, 50, 0, 3767, GUI_SET);
				// error occured
				if (clicked_color_button == -1){
					return FALSE;
				}
				if (clicked_color_button == 0){
					return SET_BOARD_WINDOW;
					break;
				}
				else{
					//selecting the tool type you wish to add (dialog)
					int clicked_tool_type_button = create_dialog(game_settings, 6, SQUARE_SIZE, 
						SQUARE_SIZE, (300 * (clicked_color_button-1)), 1949, GUI_SET);
					// error occured
					if (clicked_tool_type_button == -1){
						return FALSE;
					}
					if (clicked_tool_type_button == 0){
						return SET_BOARD_WINDOW;
						break;
					}
					switch (clicked_tool_type_button){
						case(1) :
							tool_type_to_add = (clicked_color_button == BLACK) ? 'K' : 'k';
							break;
						case(2) :
							tool_type_to_add = (clicked_color_button == BLACK) ? 'Q': 'q';
							break;
						case(3) :
							tool_type_to_add = (clicked_color_button == BLACK) ? 'R' : 'r';
							break;
						case(4) :
							tool_type_to_add = (clicked_color_button == BLACK) ? 'B' : 'b';
							break;
						case(5) :
							tool_type_to_add = (clicked_color_button == BLACK) ? 'N' : 'n';
							break;
						case(6) :
							tool_type_to_add = (clicked_color_button == BLACK) ? 'M' : 'm';
							break;
						default:
							break;
						}
					if (((tool_type_to_add == 'm') || (tool_type_to_add == 'M')) && ((marked_cord.y == 0) || (marked_cord.y == 7))){
						if (!create_popup(game_settings, 0, INVALID_SET_MASSAGE)){
							return FALSE;
						}
						return SET_BOARD_WINDOW;
					}
					board_piece(tmp_settings.board, marked_cord) = tool_type_to_add;
					//check if this add is valid
					if (!is_valid_for_set_board_window(tmp_settings.board)){
						if (!create_popup(game_settings, 0, INVALID_SET_MASSAGE)){
							return FALSE;
						}
						return SET_BOARD_WINDOW;
					}
					else{ //is valid -> remove piece
						board_piece(game_settings->board, marked_cord) = tool_type_to_add;
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
	
	gui_tree_node *panel, *last_window;
	last_window = window;
	//create panel
	if (!create_panel(125, 250, 400, 200, window)){
		free(last_window);
		return FALSE;
	}
	panel = window->children.last->data;
	//create the massage label
	if (!create_image(popup_x_location, popup_y_location, 0, 0, 400, 200, popup_img, panel)){
		free_tree(last_window);
		return FALSE;
	}
	//draw the popup
	if (!draw_tree(window)){
		free_tree(last_window);
		return FALSE;
	}

	if (SDL_Flip(window->surface) != 0) {
		free_tree(last_window);
		return FALSE;
	}

	// set a delay of 1 sec
	SDL_Delay(1000);


	gui_tree_node *curr_control = NULL;
	node * curr_node = window->children.first;
	while (curr_node->next->next != NULL){
		curr_node = curr_node->next;
	}
	window->children.last = curr_node;
	free_tree(curr_node->next->data);
	curr_node->next = NULL;
	window->children.len--;

	// re-draw tree
	if (!draw_tree(window)){
		return FALSE;
	}
	if (SDL_Flip(window->surface) != 0) {
		free_tree(last_window);
		return FALSE;
	}
	//free_tree(last_window);
	return TRUE;
}

//create a generic dialog
//it return -1 when error
//0 for click on cancel_button
//[i+1...] for the the [i....]th control
int create_dialog(settings *default_settings, int num_of_controls,
	int control_width, int control_hight, int label_location_x, int label_location_y,int special_dialog){
	


	gui_tree_node *panel, *dialog_panel, *controls_panel, *cancel_button, *label, *last_window;

	last_window = window;

	gui_tree_node **dialog_conrtol = (gui_tree_node**)malloc(num_of_controls*sizeof(gui_tree_node*));
	if (dialog_conrtol == NULL){
		return -1;
	}

	int controls_panel_width = (special_dialog == 1) ? (control_width * 2) : control_width;
	int selected_slot, i, j;
	int special_dialog_active = 0;
	int special_set_active = 0;
	int label_hight = 100;
	int label_width = 300;
	int cancel_button_width = 75;
	int cancel_button_hight = 99;
	int window_width = label_width;
	int window_hight = label_hight + (num_of_controls*control_hight) + cancel_button_hight -10;
	int different_control = TRUE;
	//last_window = dialog_window;
	settings tmp_settings = { 0 };
	memcpy(&tmp_settings, default_settings, sizeof(settings));

	
	// creating the dialog panel
	if (!create_panel((((window->surface->w)/2)-(label_width/2)) , 20, window_width+2, window_hight+2, window)){
		//free(last_window);
		free(dialog_conrtol);
		return FALSE;
	}
	dialog_panel = window->children.last->data;

	int mid_panel = (dialog_panel->surface->w) / 2;

	//creating the dialog frame
	if (!create_image(0, 0, 0, 0, window_width + 2, window_hight + 2, dialog_frame, dialog_panel)){
		free(last_window);
		free(dialog_conrtol);
		return -1;
	}
	// creating the dialog inner (useful) panel
	if (!create_panel(1, 1, window_width, window_hight , dialog_panel)){
		//free(last_window);
		free(dialog_conrtol);
		return FALSE;
	}
	panel = dialog_panel->children.last->data;
	//create the dialog label
	if (!create_image(label_location_x, label_location_y, 1, 1, label_width, label_hight, buttons_img, panel)){
		free(last_window);
		free(dialog_conrtol);
		return -1;
	}
	label = panel->children.last->data;
	//create the cancel button
	if (!create_button(0, 576, (mid_panel-48),
		label_hight + (num_of_controls*control_hight), cancel_button_hight, cancel_button_width, buttons_img, panel)){
		free(last_window);
		free(dialog_conrtol);
		return -1;
	}
	cancel_button = panel->children.last->data;
	//create the controls panel (where the controls are located)
	if (!create_panel(mid_panel - (controls_panel_width/2), label_hight, controls_panel_width, (num_of_controls*control_hight), panel)){
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
			special_dialog_active = (load_game(path[i], &tmp_settings)) ? 203 : 0;
			different_control = FALSE;

			//save_load_button_active = ACTIVE;
		}
		if (!create_button(label_location_x + special_dialog_active, label_location_y + label_hight + ((i * control_hight)*different_control),
			special_set_active + (((controls_panel_width) / 2) - (control_width / 2)), (i * control_hight), control_width, control_hight, buttons_img, controls_panel)){
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
	selected_slot = listener_to_dialog(dialog_conrtol, default_settings, cancel_button, controls_panel, num_of_controls, special_dialog);
	//free memory
	/*for (int i = num_of_controls; i >0 ; i--)
		free(dialog_conrtol[i]);
	*/
	free(dialog_conrtol);
	gui_tree_node *curr_control = NULL;
	node * curr_node = window->children.first;
	while (curr_node->next->next != NULL){
		curr_node = curr_node->next;
	}
	window->children.last = curr_node;
	free_tree(curr_node->next->data);
	curr_node->next = NULL;
	window->children.len--;

	// re-draw tree
	if (!draw_tree(window)){
		return FALSE;
	}
	if (SDL_Flip(window->surface) != 0) {
		free_tree(last_window);
		return FALSE;
	}

	return selected_slot;
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
		*get_best_move_button,/* *mask,*/ *quit_button,*vertical_frame1, *vertical_frame2, *horizontal_frame1,
		*horizontal_frame2, *turn_protocol_panel, *last_window;
	
	last_window = window;
	// create the UI tree (all the controls) in the window
	window = create_window(888, 666);
	if (window == NULL)
		return FALSE;
	//create the board panel
	if (!create_panel(0, 0, 666, 666, window)){
		free(last_window);
		return FALSE;
	}
	board_panel = window->children.last->data;
	// creathe the side panel
	if (!create_panel(666, 0, 200, 666, window)){
		free(last_window);
		return FALSE;
	}
	side_panel = window->children.last->data;
	// create the save button
	if (!create_button(0, 1725, 50, 10, 100, 75, buttons_img, side_panel)){
		free(last_window);
		return FALSE;
	}
	save_button = side_panel->children.last->data;
	//create the main menu button
	if (!create_button(100, 1725, 50, 85, 100, 75, buttons_img, side_panel)){
		free(last_window);
		return FALSE;
	}
	main_menu_button = side_panel->children.last->data;
	// create the get best moves button
	if (!create_button(300, 1725, 50, 160, 100, 75, buttons_img, side_panel)){
		free(last_window);
		return FALSE;
	}
	get_best_move_button = side_panel->children.last->data;
	//create the quit button
	if (!create_button(200, 1725, 50, 500, 100, 75, buttons_img, side_panel)){
		free(last_window);
		return FALSE;
	}
	quit_button = side_panel->children.last->data;
	
	//create turn protocol panel
	if (!create_panel(25, 250, 150, 150, side_panel)){
		free(last_window);
		return FALSE;
	}
	turn_protocol_panel = side_panel->children.last->data;
	int player_color_shift = (game_settings->next == BLACK) ? 0 : 150;
	if (!create_image(player_color_shift, 2636, 0, 0, 150, 150, buttons_img, turn_protocol_panel)){
		free(last_window);
		return FALSE;
	}

	//create the frame around the game (4 frames=2vertical+2horizontal)
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
	int next_window = listener_to_game_window(game_settings, game_panel, side_panel, turn_protocol_panel, save_button, main_menu_button,
		quit_button, board_tools, get_best_move_button, &empty_moves, FALSE, &empty_moves, error_cord);
	free_tree(last_window);
	return next_window;
}

// check how many castling moves does the possible_moves linked-list contains
int how_many_castling(moves possible_moves){
	int number_of_castling = 0;
	move *curr_move = NULL;
	node * curr_node = possible_moves.first;
	// iterating over the possible moves to check what piece needs to be marked
	while (curr_node != NULL){
		curr_move = curr_node->data;
		if (curr_move->is_castle == 1)
			number_of_castling++;
	curr_node = curr_node->next;
	}
	return number_of_castling;
}

// draw the current board state
int draw_board(settings *game_settings, gui_tree_node *panel, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
												moves possible_moves, int is_comp, cord single_cord){
	int i, j;
	int number_of_castling = how_many_castling(possible_moves);
	int board_frame = 0;
	int comp_turn = (is_comp) ? ACTIVE : UNACTIVE;
	cord casteling_DL_rook = { 0, 0 };
	cord casteling_D_king = { 4, 0 };
	cord casteling_DR_rook = { 7, 0 };
	cord casteling_UL_rook = { 0, 7 };
	cord casteling_U_king = { 4, 7 };
	cord casteling_UR_rook = { 7, 7 };
	move *curr_move = NULL;
	//free_tree_without_root(panel);
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			int is_active = UNACTIVE;
			int square_color = ((i + j) % 2 == 0) ? 0 : SQUARE_SIZE;
			gui_tree_node *cur_square = NULL;
			if (possible_moves.first != NULL){
				node * curr_node = possible_moves.first;
				// iterating over the possible moves to check what piece needs to be marked
				while (curr_node != NULL){
					curr_move = curr_node->data;
					// check if board_tools[i][j] == moves start\end
					if (((curr_move->end.x == i) && (curr_move->end.y == j)) ||
						((curr_move->start.x == i) && (curr_move->start.y == j))){
						if (curr_move->is_castle == 1){	//handle castling
							if (!mark_castle(panel->parent, curr_move, number_of_castling)){
								return FALSE;
							}
							//unmarking the rook
							if ((curr_move->end.x == 0) || (curr_move->end.x == 4) || (curr_move->end.x == 7)){
								if ((curr_move->start.x == 0) || (curr_move->start.x == 7))
									is_active = UNACTIVE;
							}
						}
						else{
							if (!mark_castle(panel->parent, &empty_move, number_of_castling)){
								return FALSE;
							}
							is_active = ACTIVE + comp_turn;
						}
					}
					
					curr_node = curr_node->next;
				}
			}
			//handle cases of single cord marking (board set)
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

//listener to game window
int listener_to_game_window(settings *game_settings, gui_tree_node *game_panel, gui_tree_node *side_panel, gui_tree_node *turn_protocol_panel, gui_tree_node *save_button,
	gui_tree_node *main_menu_button, gui_tree_node *quit_button, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
	gui_tree_node *get_best_move_button, moves *all_piece_possible_moves, int to_move, moves *moves_of_piece, cord marked_cord){

	SDL_Event event;
	int i, j, piece_color, promotion_color, next_window;
	int frame_offset = 33;
	moves comp_moves = { 0 };

	check_castling_conditions(game_settings);
	int was_checked = is_king_checked(other_player(game_settings->next), game_settings->board);


	// comp turn
	if ((game_settings->mode == PLAYER_VS_COMP) && (game_settings->color != game_settings->next) && (!game_over)){
		player_move = computer_turn(game_settings);
		if (player_move.promotion == NO_MOVE_CODE) {
			if (was_checked){
				if (!create_popup(game_settings, 0, MATE_MESSAGE)){
					game_over = TRUE;
					return FALSE;
				}
			}
			else{
				if (!create_popup(game_settings, 0, TIE_MASSAGE)){
					return FALSE;
				}
			}
			game_over = TRUE;
			check_castling_conditions(game_settings);
			return GAME_WINDOW;
		}
		else if (was_checked){
			if (!create_popup(game_settings, 0, CHECK_MASSAGE)){
				return FALSE;
			}
		}
		///next 2 lines needs to move to end with player_turn
		game_settings->next = other_player(game_settings->next);
		was_checked = game_settings->is_next_checked;
		//re-render turn protocol
		int player_color_shift = (game_settings->next == BLACK) ? 0 : 150;
		if (!create_image(player_color_shift, 2636, 0, 0, 150, 150, buttons_img, turn_protocol_panel)){
			return FALSE;
		}
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
	if (all_possible_moves.len == -1){
		/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++FREEEEEEE STUUUUUUUUUUUUUUFFFFFFFFFFFFFFFFF+++++++++++++++*/
	}
	//check for mate/tie/check
	if ((all_possible_moves.len == 0) && (!game_over)){
		if (game_settings->is_next_checked){
			if (!create_popup(game_settings, 0, MATE_MESSAGE)){
				free_list(&all_possible_moves, free);
				game_over = TRUE;
				return FALSE;
			}
			else{
				free_list(&all_possible_moves, free);
				game_over = TRUE;
				return GAME_WINDOW;
			}
		}
		else{
			if (!create_popup(game_settings, 0, TIE_MASSAGE)){
				free_list(&all_possible_moves, free);
				return FALSE;
			}
			free_list(&all_possible_moves, free);
			game_over = TRUE;
			check_castling_conditions(game_settings);
			return GAME_WINDOW;
		}
	}
	else if ((game_settings->is_next_checked)){
		if (checking_player == -1){
			checking_player = other_player(game_settings->next);
			if (!create_popup(game_settings, 0, CHECK_MASSAGE)){
				free_list(&all_possible_moves, free);
				return FALSE;
			}
		}
		if (checking_player == game_settings->next){
			if (!create_popup(game_settings, 0, CHECK_MASSAGE)){
				free_list(&all_possible_moves, free);
				return FALSE;
			}
		}
	}
	while (SDL_WaitEvent(&event)){
		switch (event.type){
		case(SDL_MOUSEBUTTONUP) :
			//check if there was a click on the game panel
			if ((is_inside_gui_tree_node(game_panel, event.button.x, event.button.y)) && (!game_over)){
				i = (event.button.x - frame_offset) / SQUARE_SIZE;
				j = 7 - ((event.button.y - frame_offset) / SQUARE_SIZE);

				//check if there was a click inside the board controls
				if (is_inside_gui_tree_node(&board_tools[i][j], (event.button.x), (event.button.y))){
					cord piece_cord = { i, j };
					if (marked_cord.x != -1){ //for promotion - to know the promoted tool's color
						promotion_color = (which_color(board_piece(game_settings->board, marked_cord)));
					}
					piece_color = (which_color(board_piece(game_settings->board, piece_cord)));

					//click on a piece inorder to start moving:
					// check that the click was on a relavant color-wise piece
					if (piece_color == game_settings->next){
						moves moves_for_piece = get_moves_for_piece(all_possible_moves, piece_cord);
						if (moves_for_piece.len == -1){
							free_list(&all_possible_moves, free);
							return FALSE;
						}
						if (moves_for_piece.len == 0){ //there are no possible moves->clean markings
							if (!user_turn_mark_possible_moves(game_settings, game_panel, board_tools,
								piece_cord, moves_for_piece, FALSE)){
								free_list(&all_possible_moves, free);
								free_list(&moves_for_piece, free);
								return FALSE;
							}

							free_list(&all_possible_moves, free);
							free_list(&moves_for_piece, free);
							to_move = FALSE;
							return GAME_WINDOW;
						}
						else if (moves_for_piece.len > 0){ //mark possible moves
							if (!user_turn_mark_possible_moves(game_settings, game_panel, board_tools,
								piece_cord, moves_for_piece, FALSE)){

								free_list(&all_possible_moves, free);
								free_list(&moves_for_piece, free);
								return FALSE;
							}
							to_move = TRUE;
						}
						next_window = listener_to_game_window(game_settings, game_panel, side_panel, turn_protocol_panel, save_button, main_menu_button,
							quit_button, board_tools, get_best_move_button, &all_possible_moves, to_move, &moves_for_piece, piece_cord);

						if (all_possible_moves.len > 0){
							free_list(&all_possible_moves, free);
						}
						/*if (moves_for_piece.len > 0){
							free_list(&moves_for_piece, free);
						}*/

						return next_window;
					}

					//second click on board was made
					//make a movement to piece_cord location:
					checking_player = -1;
					move *curr_move = get_selected_move(*moves_of_piece, piece_cord);
					if (curr_move != NULL){
						//promotion case:
						if (curr_move->promotion == 1){
							char tool_type_to_promote;
							int tool_color_shift = (promotion_color == BLACK) ? 0 : 370;
							int clicked_button = create_dialog(game_settings, 4, 75, 50, 0, 3021 + tool_color_shift, FALSE);
							// error occured
							if (clicked_button == -1){
								free(curr_move);
								free_list(&all_possible_moves, free);
								return FALSE;
							}
							// click on "cancel" button
							if (clicked_button == 0){
								free(curr_move);
								free_list(&all_possible_moves, free);
								game_over = FALSE;
								return GAME_WINDOW;
								break;
							}
							switch (clicked_button){
							case(1) :
								tool_type_to_promote = (promotion_color == BLACK) ? 'Q' : 'q';
								break;
							case(2) :
								tool_type_to_promote = (promotion_color == BLACK) ? 'R' : 'r';
								break;
							case(3) :
								tool_type_to_promote = (promotion_color == BLACK) ? 'B' : 'b';
								break;
							case(4) :
								tool_type_to_promote = (promotion_color == BLACK) ? 'N' : 'n';
								break;
							default:
								break;
							}

							board_piece(curr_move->board, curr_move->end) = tool_type_to_promote;
						}
						// play - move piece to target location
						move to_play = *curr_move;
						board_copy(to_play.board, game_settings->board);
						game_settings->is_next_checked = is_king_checked(other_player(game_settings->next), game_settings->board);

						free_tree_without_root(game_panel);
						if (!mark_castle(game_panel->parent, &empty_move, 0))
							return FALSE;
						if (!draw_board(game_settings, game_panel, board_tools, empty_moves, FALSE, error_cord)){
							free(curr_move);
							free_list(&all_possible_moves, free);
							return FALSE;
						}
						if (!draw_tree(window)){
							free(curr_move);
							free_list(&all_possible_moves, free);
							return FALSE;
						}
						if (SDL_Flip(window->surface) != 0) {
							free(curr_move);
							free_list(&all_possible_moves, free);
							return FALSE;
						}

						//finishing settings
						game_settings->is_next_checked = is_king_checked(other_player(game_settings->next), game_settings->board);
						game_settings->next = other_player(game_settings->next);
						free(curr_move);
						free_list(&all_possible_moves, free);
						
						return GAME_WINDOW;
					}

					else { // the selected tool is not the right player tool
						//next_window = listener_to_game_window(game_settings, game_panel, side_panel, save_button, main_menu_button,
						//	quit_button, board_tools, get_best_move_button, all_piece_possible_moves, to_move, moves_of_piece, error_cord);
						free(curr_move);
						if (all_possible_moves.len>0){
							free_list(&all_possible_moves, free);
						}
						return GAME_WINDOW;
					}
				}
			}
			
				//click on side control
				else if (is_inside_gui_tree_node(main_menu_button, event.button.x, event.button.y)){
					game_over = FALSE;
					return MAIN_WINDOW;
				}
				//checking if there was a cilck on castling arrows
				else if ((is_inside_gui_tree_node(game_panel->parent, event.button.x, event.button.y)) && is_castling_now){
					int frame_offset = 33;
					//up left castling
					if ((event.button.x >= ((SQUARE_SIZE * 2) + frame_offset)) && (event.button.x <= ((SQUARE_SIZE * 3) + frame_offset)) &&
						(event.button.y >= 0) && (event.button.y <= frame_offset)){
						cord king_start_cord = { 4, 7 };
						cord king_end_cord = { 2, 7 };
						cord rook_start_cord = { 0, 7 };
						cord rook_end_cord = { 3, 7 };
						move_from_to(game_settings->board, king_start_cord, king_end_cord);
						move_from_to(game_settings->board, rook_start_cord, rook_end_cord);
					}
					//up right castling
					if ((event.button.x >= ((SQUARE_SIZE * 4) + frame_offset)) && (event.button.x <= ((SQUARE_SIZE * 6) + frame_offset)) &&
						(event.button.y >= 0) && (event.button.y <= frame_offset)){
						cord king_start_cord = { 4, 7 };
						cord king_end_cord = { 6, 7 };
						cord rook_start_cord = { 7, 7 };
						cord rook_end_cord = { 5, 7 };
						move_from_to(game_settings->board, king_start_cord, king_end_cord);
						move_from_to(game_settings->board, rook_start_cord, rook_end_cord);
					}
					//down left castling
					if ((event.button.x >= ((SQUARE_SIZE * 2) + frame_offset)) && (event.button.x <= ((SQUARE_SIZE * 3) + frame_offset)) &&
						(event.button.y >= 630) && (event.button.y <= 666)){
						cord king_start_cord = { 4, 0 };
						cord king_end_cord = { 2, 0 };
						cord rook_start_cord = { 0, 0 };
						cord rook_end_cord = { 3, 0 };
						move_from_to(game_settings->board, king_start_cord, king_end_cord);
						move_from_to(game_settings->board, rook_start_cord, rook_end_cord);
					}
					//down right castling
					if ((event.button.x >= ((SQUARE_SIZE * 4) + frame_offset)) && (event.button.x <= ((SQUARE_SIZE * 7) + frame_offset)) &&
						(event.button.y >= 630) && (event.button.y <= 666)){
						cord king_start_cord = { 4, 0 };
						cord king_end_cord = { 6, 0 };
						cord rook_start_cord = { 7, 0 };
						cord rook_end_cord = { 5, 0 };
						move_from_to(game_settings->board, king_start_cord, king_end_cord);
						move_from_to(game_settings->board, rook_start_cord, rook_end_cord);

					}

					//finishing settings
					game_settings->is_next_checked = is_king_checked(other_player(game_settings->next), game_settings->board);
					game_settings->next = other_player(game_settings->next);
					is_castling_now = FALSE;
					
					return GAME_WINDOW;
				}
				//click on save control
				else if (is_inside_gui_tree_node(save_button, event.button.x, event.button.y)){
					//clicked_button can return: 0 = cancel was clicked, [1...] = the [i+1....]th save\load slots 
					int clicked_button = create_dialog(game_settings, NUM_OF_MEMORY_SLOTS, 200, 50, 600, 1200, GUI_SAVE);
					if (clicked_button == -1){// error occured
						return FALSE;
					} 
					if (clicked_button == 0){// cancel button was clicked
						return GAME_WINDOW;
						break;
					}
					else{ //one of the saving slots was clicked
						if (save_game(path[clicked_button - 1], game_settings)){
							return GAME_WINDOW;
							break;
						}
						else{
							continue;
						}
					}
				}
				//click on best move control
				else if ((is_inside_gui_tree_node(get_best_move_button, event.button.x, event.button.y))&&(!game_over)){
					moves best_moves = { 0 };
					move_node *node = NULL;
					node = (move_node*)malloc(sizeof(move_node));
					if (node == NULL){
						return FALSE;
					}
					if (game_settings->mode == PLAYER_VS_PLAYER){
						int clicked_diff_change_button = create_dialog(game_settings, 5,
							50, 50, 0, 4042, GUI_SET);
						// error occured
						if (clicked_diff_change_button == -1){
							return FALSE;
						}
						if (clicked_diff_change_button == 0){
							return SETTINGS_WINDOW;
							break;
						}
						switch (clicked_diff_change_button){
						case(1) :
							game_settings->minimax_depth = 0;
							break;
						case(2) :
							game_settings->minimax_depth = 1;
							break;
						case(3) :
							game_settings->minimax_depth = 2;
							break;
						case(4) :
							game_settings->minimax_depth = 3;
							break;
						case(5) :
							game_settings->minimax_depth = 4;
							break;
						default:
							break;
						}
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
				}
				else if (is_inside_gui_tree_node(quit_button, event.button.x, event.button.y)){
					int next_window = create_quit_dialog(game_settings, GAME_WINDOW);
					return next_window;
				}
		default:
			break;
			}
		}
		return FALSE;
}

int create_quit_dialog(settings *game_settings, int prev_window){

	int clicked_quit_button = create_dialog(game_settings,1,99,75,0,4467,GUI_SET);
	// error occured
	if (clicked_quit_button == -1){
		return FALSE;
	}
	//"No" button was clicked
	if (clicked_quit_button == 0){
		return prev_window;
	}
	switch (clicked_quit_button){
	case(1) :
		return QUIT_PROGRAM;
		break;
	default:
		break;
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


int user_turn_mark_possible_moves(settings *game_settings, gui_tree_node *game_panel,
	gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], cord piece_cord, moves moves_for_piece, int blue_mark){
	free_tree_without_root(game_panel);

	/*move *curr_move = NULL;
	node * prev_node = NULL;
	node * curr_node = moves_for_piece.first;
	// iterating over the possible moves to check what piece needs to be marked
	while (curr_node != NULL){
		curr_move = curr_node->data;
		if (curr_move->is_castle == 1){ // disconnecting the castle move (in order to special-mark it)
			prev_node->next = curr_node->next;
			if (!mark_castle(board_panel,curr_move)){
				return FALSE;
			}
			break;
		}
		prev_node = curr_node;
		curr_node = curr_node->next;
	}*/

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

int mark_castle(gui_tree_node *board_panel, move *curr_move, int number_of_castling){
	int frame_offset = 33;
	int castling_frame = 0;
	if (curr_move->is_castle == FALSE)
		return TRUE;
	if (curr_move->start.y == 0){
		//right side castle
		if (curr_move->start.x == 7)
			castling_frame = 1;
		//left side castle
		if (curr_move->start.x == 0)
			castling_frame = 2;
		//both sides castle
		if (number_of_castling == 2)
			castling_frame = 3;
		if (!create_image(0, 1050 + (frame_offset * castling_frame), 33, 633, 600, 33, tools_img, board_panel)){
			return FALSE;
		}
	}
	else{
		if (curr_move->start.x == 7)
			castling_frame = 4;
		if (curr_move->start.x == 0)
			castling_frame = 5;
		if (number_of_castling == 2)
			castling_frame = 6;
		if (!create_image(0, 1050 + (frame_offset * castling_frame), 33, 0, 600, 33, tools_img, board_panel)){
			return FALSE;
		}
	}
	is_castling_now = TRUE;
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
