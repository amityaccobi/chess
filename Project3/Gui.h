#ifndef GUI_
#define GUI

#include "GuiUtils.h"

/*****************/
/****CONTSANTS****/
/*****************/
#define NUM_OF_SLOTS 1
#define PATH_LENGTH 10
#define SQUARE_SIZE 75


#define MAIN_WINDOW 1
#define PLAYER_SELECTION_WINDOW 2
#define SETTINGS_WINDOW 3
#define SAVE_WINDOW 4
#define LOAD_WINDOW 5
#define GAME_WINDOW 6
#define SET_BOARD_WINDOW 7
#define QUIT_WINDOW 8


/***********************/
/**** LIST OF FUNCS ****/
/***********************/
int main_gui();
int create_main_window();
int create_load_window(settings *default_settings);
int create_settings_window(settings *default_settings);
listener_to_Settings_window(settings *default_settings, gui_tree_node *color_white_button,
	gui_tree_node *color_black_button, gui_tree_node *ok_button,
	gui_tree_node *cancel_button, gui_tree_node *set_board_button, gui_tree_node *game_difficulty[5]);
int create_player_selection_window(settings *default_settings);
int listener_to_player_selection_window(settings *default_settings, gui_tree_node *player_vs_player_button, gui_tree_node *player_vs_comp_button, gui_tree_node *cancel_button);
int create_main_window();
int listener_to_main_window(gui_tree_node *new_game_btn, gui_tree_node *load_game_btn, gui_tree_node *quit_btn);
listener_to_load_window(gui_tree_node *slot[4], settings *default_settings,
	gui_tree_node *cancel_button, gui_tree_node *set_board_button);
int create_load_window(settings *default_settings);
int draw_board(settings *game_settings, gui_tree_node *panel, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], moves possible_moves);
int listener_to_game_window(settings *game_settings, gui_tree_node *game_panel, gui_tree_node *side_panel, gui_tree_node *save_button,
	gui_tree_node *main_menu_button, gui_tree_node *quit_button, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], moves all_possible_moves);
int mark_all_possible_moves(moves possible_moves, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
	settings *game_settings, gui_tree_node *panel);
int create_game_window(settings *game_settings, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE]);
void set_square_selected(gui_tree_node *board_tools);
void get_player_next_moves(gui_tree_node *board_tools);
int piece_type(char c);

#endif GUI