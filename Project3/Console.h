#ifndef CONSOLE_
#define CONSOLE_

#include "Chess.h"
#include <string.h>

#define GAME_MODE "game_mode "
#define DIFFICULTY "difficulty "
#define COLOR_SETTING "user_color "
#define NEXT_PLAYER "next_player "
#define LOAD "load "
#define CLEAR_SETTING "clear"
#define REMOVE_SETTING "rm <"
#define SET_SETTING "set <"
#define PRINT_SETTING "print"
#define START_SETTING "start"
#define MOVE "move <"
#define CASTLE "castle <"
#define GET_MOVES "get_moves <"
#define BEST_MOVES "get_best_moves "
#define GET_SCORE "get_score "
#define SAVE "save "
#define QUIT "quit"

#define ENTER_YOUR_MOVE " player - enter your move:\n" 

#define BEST "best"
#define DEPTH "depth "

// board initialization
#define ENTER_SETTINGS "Enter game settings:\n" 
#define WRONG_GAME_MODE "Wrong game mode\n"
#define TWO_PLAYERS_GAME_MODE "Running game in 2 players mode\n"
#define PLAYER_VS_AI_GAME_MODE "Running game in player vs. AI mode\n"
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_FILE_NAME "Wrong file name\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_PIECE "Setting this piece creates an invalid board\n"  
#define WRONG_BOARD_INITIALIZATION "Wrong board initialization\n"

#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
#define NO_DICS "The specified position does not contain your piece\n"

#define WRONG_ROOK_POSITION "Wrong position for a rook\n" 
#define ILLEGAL_CALTLING_MOVE "Illegal castling move\n"  

#define TIE "The game ends in a tie\n"
#define CHECK "Check!\n"

#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name))
#define print_message(message) (printf("%s", message))

#define print_cord(c) (printf("<%c,%d>",(c).x + 'a', (c).y+1))
#define print_win(color) (printf("Mate! %s player wins the game\n", (color)))

int console_mode();
int print_line();
int print_board(char board[BOARD_SIZE][BOARD_SIZE]);
int read_input(char input[51]);
cord parse_cord(char * cord_str);
int print_move(move * move0);
int print_type(char ch);
move parse_move(char * movestr, char board[BOARD_SIZE][BOARD_SIZE]);
char piece_from_string(char * piece_char, int color);
move user_turn(settings * game_settings, int was_checked);

#endif CONSOLE_