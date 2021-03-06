#ifndef CHESS_
#define CHESS_

#include "List.h"
#include <ctype.h>
#include <mxml.h>
#include <limits.h>
#include <time.h> //for srand()

#define DEBUG FALSE

/*****************/
/****CONTSANTS****/
/*****************/
#define PAWN 'm'
#define BISHOP 'b'
#define KNIGHT 'n'
#define ROOK 'r'
#define QUEEN 'q'
#define KING 'k'

#define WHITE_P 'm'
#define WHITE_B 'b'
#define WHITE_N 'n'
#define WHITE_R 'r'
#define WHITE_Q 'q'
#define WHITE_K 'k'

#define BLACK_P 'M'
#define BLACK_B 'B'
#define BLACK_N 'N'
#define BLACK_R 'R'
#define BLACK_Q 'Q'
#define BLACK_K 'K'

#define EMPTY 0

#define BOARD_SIZE 8

#define WHITE 0
#define BLACK 1

#define PAWN_SCORE 1
#define BISHOP_SCORE 3
#define KNIGHT_SCORE 3
#define ROOK_SCORE 5
#define QUEEN_SCORE 9
#define KING_SCORE 400
#define SCORE_ERROR 4242
#define WIN_SCORE INT_MAX
#define LOSE_SCORE INT_MIN
// tie score is only better than lose score
#define TIE_SCORE LOSE_SCORE + 42

#define PLAYER_VS_PLAYER 1
#define PLAYER_VS_COMP 2

#define BEST_DIFFICULTY 0

#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name))

/*****************/
/*****STRUCTS*****/
/*****************/
typedef struct {
	int x;
	int y;
} cord; //coordinate on the board

typedef struct {
	int color;
	int minimax_depth;
	int mode;
	int next;
	int is_next_checked;
	char board[BOARD_SIZE][BOARD_SIZE];
	//for castling:
	int white_king_moved;
	int black_king_moved;
	int white_rook_1_moved;
	int white_rook_2_moved;
	int black_rook_1_moved;
	int black_rook_2_moved;
} settings; //game settings

typedef struct {
	cord start;
	cord end;
	char board[BOARD_SIZE][BOARD_SIZE]; // board layout in the end of the move
	int promotion; //boolean: is promotion move
	int is_castle; //boolean: is castling move
}move; //struct representing a move on the board

typedef linked_list moves;
typedef node move_node;

#define NO_MOVE_CODE 42


/*********************/
/*****MACRO FUNCS*****/
/*********************/

#define is_same_cord(a,b) (((a).x == (b).x) && ((a).y == (b).y))

// check if command is valid (no args)
#define is_command(command) (strcmp(input, (command)) == 0)

// check if command is valid (with args)
#define is_command_with_args(command) (strncmp(input, (command), strlen((command))) == 0) 

//check if piece is man
#define is_man(ch) ((ch) == 'm' || (ch) == 'M') 

// retrurn the piece in cord c on board
#define board_piece(board,c) ((board)[(c).x][(c).y])

// check the promotion row of certain color
#define promotion_row(color) (((color) == WHITE) ? (BOARD_SIZE - 1) : 0)

// check the jumping-row (pawn's first step) of certain color
#define jumping_row(color) (((color) == WHITE) ? 1 : BOARD_SIZE - 2)

// crown a piece. 'k' - 'm' == 'K' - 'M'
#define crown(new_board, new_cord) (board_piece((new_board), (new_cord)) += 'k' - 'm')

#define is_valid_direction(piece, y) ((!is_man((piece)))||((which_color((piece))==WHITE) && (y==1))||((which_color((piece))==BLACK) && (y==-1)))

// get other player color
#define other_player(player) (1 - (player))

#define mini(a,b) (((a)<(b)) ? (a):(b))

#define maxi(a,b) (((a)>(b)) ? (a):(b))

#define string_to_color(color) ((strcmp((color), "White") == 0) ? WHITE : BLACK)

#define color_string(color) (((color) == WHITE) ? "White" : "Black")


/***********************/
/**** LIST OF FUNCS ****/
/***********************/
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
void copy_move(move * single_move, move* to_copy_move);
int save_game(char * path, settings * game_settings);
int load_game(char * path, settings * game_settings);
linked_list make_all_moves(settings * set);
linked_list get_simple_moves(settings * set, cord curr);
int score(settings * set, int scoring_player, int current_player, int is_best);
int is_valid_move(linked_list all_valid_moves, move new_move);
void board_copy(char original[BOARD_SIZE][BOARD_SIZE], char copy[BOARD_SIZE][BOARD_SIZE]);
int is_valid_cord(cord c);
void clear(char board[BOARD_SIZE][BOARD_SIZE]);
int is_valid_board(char board[BOARD_SIZE][BOARD_SIZE]);
settings settings_state();
move computer_turn(settings * game_settings);
double minimax(settings set, double alpha, double beta, int is_maxi_player, int depth, int is_best_difficulty, int can_prune);
moves best_next_moves(settings set, int maximizer);
void move_cords(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int max_move, int color, int move_x, int move_y, cord move_cords[32]);
moves cords_to_moves(cord start_cord, cord move_cords[32], char board[BOARD_SIZE][BOARD_SIZE], int color);
linked_list pawn_moves(settings * set, cord curr, int color);
linked_list promote(move* single_move);
linked_list bishop_moves(settings * set, cord curr, int color, int is_king);
linked_list rook_moves(settings * set, cord curr, int color, int is_king);
linked_list queen_moves(settings * set, cord curr, int color, int is_king);
linked_list king_moves(settings * set, cord curr, int color);
linked_list knight_moves(settings * set, cord curr, int color);
void knight_cords(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int color, cord move_cords[32]);
void move_from_to(char board[BOARD_SIZE][BOARD_SIZE], cord start, cord end);
int get_piece_score(char player_piece);
int is_over_max(char board[BOARD_SIZE][BOARD_SIZE], cord c);
int piece_count(char board[BOARD_SIZE][BOARD_SIZE], cord c);
int is_king_checked(int colour, char board[BOARD_SIZE][BOARD_SIZE]);
int get_best_depth(settings * set, int player);
moves get_moves_for_piece(moves all_possible_moves, cord c);
move get_castling_move(settings * set, cord curr, int color);
void check_castling_conditions(settings * game_settings);
move get_best_move(settings * game_settings);
cord find_piece(char piece, char board[BOARD_SIZE][BOARD_SIZE]);
int which_color(char piece);

#endif