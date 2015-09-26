#ifndef GUI_
#define GUI

#include "GuiUtils.h"
#include <math.h>

/*****************/
/****CONTSANTS****/
/*****************/
#define NUM_OF_MEMORY_SLOTS 7
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
#define QUIT_PROGRAM 0

#define CHECK_MESSAGE 0
#define MATE_MESSAGE 200
#define INVALID_SET_MESSAGE 400
#define TIE_MESSAGE 600
#define ERROR_LOADING 800

#define GUI_SAVE 3
#define GUI_LOAD 2
#define GUI_SET 1

/***********************/
/**** LIST OF FUNCS ****/
/***********************/
int gui_mode();
//int create_main_window();
int create_main_window(settings *default_settings);
int create_load_save_window(settings *default_settings, int is_save);
int create_settings_window(settings *default_settings);
int listener_to_Settings_window(settings *default_settings, gui_tree_node *diff_change_button,
	gui_tree_node *ok_button, gui_tree_node *cancel_button, gui_tree_node *next_player_change_button,
	gui_tree_node *color_change_button, gui_tree_node *set_board_button);
int create_player_selection_window(settings *default_settings);
int listener_to_player_selection_window(settings *default_settings, gui_tree_node *player_vs_player_button,
	gui_tree_node *player_vs_comp_button, gui_tree_node *cancel_button);
int create_main_window();
//int listener_to_main_window(gui_tree_node *new_game_btn, gui_tree_node *load_game_btn, gui_tree_node *quit_btn);
int listener_to_main_window(settings *default_settings, gui_tree_node *new_game_button, gui_tree_node *load_game_button, gui_tree_node *quit_button);
int listener_to_load_save_window(gui_tree_node *slot[4], settings *default_settings,
	gui_tree_node *cancel_button, gui_tree_node *set_board_button, int is_save);
int create_load_window(settings *default_settings);
int draw_board(settings *game_settings, gui_tree_node *panel,
	gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], moves possible_moves, int is_comp, cord single_cord);
int listener_to_game_window(settings *game_settings, gui_tree_node *game_panel, gui_tree_node *side_panel, gui_tree_node *turn_protocol_panel, gui_tree_node *save_button,
	gui_tree_node *main_menu_button, gui_tree_node *quit_button, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
	gui_tree_node *get_best_move_button, moves *all_piece_possible_moves, int to_move, moves *moves_of_piece, cord marked_cord);

int mark_all_possible_moves(moves possible_moves, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
	settings *game_settings, gui_tree_node *panel);
int game_flow(settings *game_settings, gui_tree_node *game_panel, gui_tree_node *side_panel, gui_tree_node *save_button,
	gui_tree_node *main_menu_button, gui_tree_node *quit_button, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
	moves all_possible_moves);
int create_game_window(settings *game_settings, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE]);
void set_square_selected(gui_tree_node *board_tools);
void get_player_next_moves(gui_tree_node *board_tools);
int piece_type(char c);


move* get_relevent_marked_move(cord piece_cord, moves *moves_for_piece);
int user_turn_mark_possible_moves(settings *game_settings, gui_tree_node *game_panel,
	gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], cord piece_cord, moves moves_for_piece, int blue_mark);
moves set_comp_moves(move player_move);
move* get_selected_move(moves moves_of_piece, cord piece_cord);
void reset_game(settings *game_settings);
int before_listener(settings *game_settings, gui_tree_node *game_panel, gui_tree_node *side_panel, gui_tree_node *save_button,
	gui_tree_node *main_menu_button, gui_tree_node *quit_button, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE],
	moves *all_piece_possible_moves, int to_move, moves *moves_of_piece);
int listener_to_set_board_window(settings *game_settings, gui_tree_node *game_panel,
	gui_tree_node *side_panel, gui_tree_node *add_button, gui_tree_node *move_button, gui_tree_node *remove_button, gui_tree_node *clear_button,
	gui_tree_node *start_button, gui_tree_node *cancel_button,
	gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], int is_marked, cord marked_cord, int is_move);
int create_set_board_window(settings *game_settings, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE]);
int mark_selected_cord(settings *game_settings, gui_tree_node *game_panel, gui_tree_node board_tools[BOARD_SIZE][BOARD_SIZE], cord piece_cord, int move_mark);
int create_dialog(settings *default_settings, int num_of_controls, int control_width, int control_hight, int label_location_x, int label_location_y,
	int is_save_load_dialog);
int listener_to_dialog(gui_tree_node **dialog_conrtol, settings *default_settings,
	gui_tree_node *cancel_button, gui_tree_node *controls_panel, int num_of_controls, int is_save_load_dialog);
int create_popup(settings *default_settings, int popup_x_location, int popup_y_location);
int mark_castle(gui_tree_node *board_panel, move *curr_move, int number_of_castling);
int create_quit_dialog(settings *game_settings, int prev_window);

#endif