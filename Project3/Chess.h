#ifndef CHESS_
#define CHESS_


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "List.h"
#include <ctype.h>
#include <mxml.h>
#include <limits.h>
#include <time.h> //for srand()

#define DEBUG FALSE
#define DEBUG1 TRUE

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
char make_board(char board[BOARD_SIZE][BOARD_SIZE], cord* moves);
void copy_move(move * single_move, move* to_copy_move);
int save_game(char * path, settings * game_settings);
int load_game(char * path, settings * game_settings);
linked_list make_all_moves(settings set);
linked_list get_simple_moves(char board[BOARD_SIZE][BOARD_SIZE], cord curr);
int score(char board[BOARD_SIZE][BOARD_SIZE], int scoring_player, int current_playerint, int is_best_difficulty);
int is_valid_move(linked_list all_valid_moves, move new_move);
linked_list eat(char board[BOARD_SIZE][BOARD_SIZE], cord eater, int x_direction, int y_direction, int k, int is_chain);
void board_copy(char original[BOARD_SIZE][BOARD_SIZE], char copy[BOARD_SIZE][BOARD_SIZE]);
int is_valid_cord(cord c);
void clear(char board[BOARD_SIZE][BOARD_SIZE]);
int which_color(char c);
int is_valid_board(char board[BOARD_SIZE][BOARD_SIZE]);
settings settings_state();
void remove_piece(char board[BOARD_SIZE][BOARD_SIZE], cord piece);
void add_piece(char board[BOARD_SIZE][BOARD_SIZE], cord piece, int color, char type);
move computer_turn(settings * game_settings);
linked_list get_max_eat(char board[BOARD_SIZE][BOARD_SIZE], cord start, int max_eat, int is_chain);
int minimax(settings set, int alpha, int beta, int is_maxi_player, int depth, int is_best_difficulty);
moves best_next_moves(settings set, int maximizer);
void move_cords(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int max_move, int color, int move_x, int move_y, cord move_cords[32]);
moves cords_to_moves(cord start_cord, cord move_cords[32], char board[BOARD_SIZE][BOARD_SIZE], int color);
linked_list pawn_moves(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int color);
linked_list promote(move* single_move);
linked_list bishop_moves(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int color, int is_king);
linked_list rook_moves(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int color, int is_king);
linked_list queen_moves(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int color, int is_king);
linked_list king_moves(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int color);
linked_list knight_moves(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int color);
void knight_cords(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int color, cord move_cords[32]);
void move_from_to(char board[BOARD_SIZE][BOARD_SIZE], cord start, cord end);
int get_piece_score(char player_piece);
int is_over_max(char board[BOARD_SIZE][BOARD_SIZE], char piece);
int piece_count(char board[BOARD_SIZE][BOARD_SIZE], char piece);
int is_king_checked(int color, char board[BOARD_SIZE][BOARD_SIZE]);
int get_best_depth(char board[BOARD_SIZE][BOARD_SIZE], int player);
moves get_moves_for_piece(moves all_possible_moves, cord c);
moves get_castling_moves(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int color);

#endif CHESS