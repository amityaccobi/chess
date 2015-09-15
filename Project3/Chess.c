#include "Chess.h"
#include "Console.h"

moves error_moves = { -1, 0, 0 };
cord error_cord = { -1, -1 };
move error_move = { 0, 0, 0, 0, 0, -1, -1 };

/***************/
/**** board ****/
/***************/

// initialize board to start positions
void init_board(char board[BOARD_SIZE][BOARD_SIZE]){
	clear(board);
	int i;
	for (i = 0; i < BOARD_SIZE; i++){  //set pawns
		board[i][1] = WHITE_P;
		board[i][6] = BLACK_P;
	}
	board[0][0] = board[7][0] = WHITE_R;
	board[0][7] = board[7][7] = BLACK_R;
	board[1][0] = board[6][0] = WHITE_N;
	board[1][7] = board[6][7] = BLACK_N;
	board[2][0] = board[5][0] = WHITE_B;
	board[2][7] = board[5][7] = BLACK_B;
	board[3][0] = WHITE_Q;
	board[3][7] = BLACK_Q;
	board[4][0] = WHITE_K;
	board[4][7] = BLACK_K;
}



//clear board of pieces
void clear(char board[BOARD_SIZE][BOARD_SIZE]) {
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			board[i][j] = EMPTY;
		}
	}
}

//check if board is valid for "set board window"
// check 3 things: 1. only one king, 2. no more than allowed pieces
// for every piece, 3.no pawns in row 0 and 7
int is_valid_for_set_board_window(char board[BOARD_SIZE][BOARD_SIZE]){
	char piece[12] = { 'm', 'b', 'n', 'r', 'q', 'k', 'M', 'B', 'N', 'R', 'Q', 'K' };
	int i, j;
	int first_row = 0;
	int last_row = 7;
	//1. only one king
	if (!is_valid_board(board))
		return FALSE;
	//2. no more than allowed pieces for every piece
	for (i = 0; i < 12; i++){
		if (is_over_max(board, piece[i]))
			return FALSE;
	}
	//3.no pawns in row 0 and 7
	for (j = 0; j < BOARD_SIZE; j++){
		if ((board[j][first_row] == 'p') || (board[j][first_row] == 'P')
			|| (board[j][last_row] == 'p') || (board[j][last_row] == 'p'))
			return FALSE;
	}
	return TRUE;

}
//check if board is valid for starting the game
int is_valid_board(char board[BOARD_SIZE][BOARD_SIZE]) {
	return ((piece_count(board, WHITE_K) == 1) &&
		(piece_count(board, BLACK_K) == 1));
}

// piece count
int piece_count(char board[BOARD_SIZE][BOARD_SIZE], char piece) {
	int count = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (piece == board[i][j])
				count++;
		}
	}
	return count;
}


//check there are no more than allowed pieces of type 'piece'
int is_over_max(char board[BOARD_SIZE][BOARD_SIZE], char piece) {
	char piece_type = tolower(piece);
	int count = piece_count(board, piece);
	switch (piece_type)
	{
	case PAWN:
		return (count > 8);
	case KING:
		return (count > 1);
	case QUEEN:
		return (count > 1);
	default: //rest of the pieces
		return (count > 2);
	}
}

// copy original bord content to copy board
void board_copy(char original[BOARD_SIZE][BOARD_SIZE], char copy[BOARD_SIZE][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			copy[i][j] = original[i][j];
		}
	}
}

/*
*  a method that checkes castling conditions in the end of each turn
*/
void check_castling_conditions(settings * game_settings){
	game_settings->white_king_moved = (game_settings->white_king_moved | (game_settings->board[4][0] != WHITE_K));
	game_settings->black_king_moved = (game_settings->black_king_moved || (game_settings->board[4][7] != BLACK_K));
	game_settings->white_rook_1_moved = (game_settings->white_rook_1_moved || (game_settings->board[0][0] != WHITE_R));
	game_settings->white_rook_2_moved = (game_settings->white_rook_2_moved || (game_settings->board[7][0] != WHITE_R));
	game_settings->black_rook_1_moved = (game_settings->black_rook_1_moved || (game_settings->board[0][7] != BLACK_R));
	game_settings->black_rook_2_moved = (game_settings->black_rook_2_moved || (game_settings->board[7][7] != BLACK_R));
}


/************************/
/* coordinates & pieces */
/************************/

//check if cordinate is valid
int is_valid_cord(cord c) {
	if ((c.x >= 0) && (c.x < BOARD_SIZE) &&	// x coordinate is within board
		(c.y >= 0) && (c.y < BOARD_SIZE))// y coordinate is within board
		return TRUE;
	else
		return FALSE;
}

//add a piece to given coordinate on board
void add_piece(char board[BOARD_SIZE][BOARD_SIZE], cord piece, int color, char type) {
	char to_add;
	if (color == WHITE) {
		to_add = type;
	}
	else //color is black
		to_add = type - 'a' + 'A';
	board_piece(board, piece) = to_add;
}

//return the color of a piece or -1 if square is empty
int which_color(char piece) {
	if (piece == EMPTY)
		return -1; //epmty square
	else if (isupper(piece)) {
		return BLACK;
	}
	else
		return WHITE;
}

/****************/
/***** move *****/
/****************/

//copy a single move
void copy_move(move * original_move, move* new_copy){
	new_copy->start = original_move->start;
	new_copy->end = original_move->end;
	new_copy->promotion = original_move->promotion;
	new_copy->is_castle = original_move->is_castle;
	board_copy(original_move->board, new_copy->board);
}

/*
* return all possible moves for piece in c
*/
moves get_moves_for_piece(moves all_possible_moves, cord c) {
	moves moves_for_piece = { 0 };
	node * curr_node = all_possible_moves.first;
	move *curr_move;
	int king_y;

	while (curr_node != NULL){
		curr_move = curr_node->data;
		king_y = (which_color(board_piece(curr_move->board, curr_move->end)) == WHITE) ? 0 : 7;
		if (is_same_cord((curr_move->start), c) || (curr_move->is_castle && ((c.x == 4) && (c.y == king_y)))) {
			if (!add_node(&moves_for_piece, curr_move, sizeof(move))){
				free_list(&moves_for_piece, free);
				return error_moves;
			}
		}
		curr_node = curr_node->next;
	}
	return moves_for_piece;
}

/*
* check if a move is valid (is it one of the possible moves)
* if move is valid, return its index in all_vaild_moves. else, return -1.
* start = first coordinate of the move
* steps_len = no. of total steps in move
* steps = all coordinate the move passes through
*/
int is_valid_move(moves all_valid_moves, move new_move){
	move_node* curr_node = all_valid_moves.first;
	move * curr_move;
	cord start = new_move.start;
	cord end = new_move.end;
	for (unsigned i = 0; i < all_valid_moves.len; i++){
		curr_move = curr_node->data;
		if (DEBUG && curr_move->promotion) {
			printf("MY PROMOTION: %c Vs. THEIR PRO: %c\n", board_piece(curr_move->board, end), board_piece(new_move.board, end));
		}
		if ((is_same_cord(curr_move->start, start)) &&
			(is_same_cord(curr_move->end, end)) &&
			(board_piece(curr_move->board, end) == board_piece(new_move.board, end))) //it's the same promotion
			return i;
		curr_node = curr_node->next;
	}
	return -1;
}


int load_game(char * path, settings * game_settings){
	FILE * fp;
	mxml_node_t *xml_tree;
	mxml_node_t *xml_node;
	char row_node[17] = "game/board/row_x";
	char row[9] = { 0 };
	cord c;

	fp = fopen(path, "r");
	if (fp == NULL)
		return FALSE;
	xml_tree = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
	fclose(fp);

	xml_node = mxmlFindPath(xml_tree, "game/next_turn");
	game_settings->next = (xml_node == NULL) ? WHITE : string_to_color(mxmlGetOpaque(xml_node));

	xml_node = mxmlFindPath(xml_tree, "game/game_mode");
	game_settings->mode = (xml_node == NULL) ? PLAYER_VS_COMP : atoi(mxmlGetOpaque(xml_node));

	if (game_settings->mode == PLAYER_VS_COMP){
		xml_node = mxmlFindPath(xml_tree, "game/difficulty");
		game_settings->minimax_depth = (xml_node == NULL) ? 1 : atoi(mxmlGetOpaque(xml_node));

		xml_node = mxmlFindPath(xml_tree, "game/user_color");
		game_settings->color = (xml_node == NULL) ? WHITE : string_to_color(mxmlGetOpaque(xml_node));
	}

	for (int y = 8; y > 0; y--) {
		row_node[15] = '0' + y;
		xml_node = mxmlFindPath(xml_tree, row_node);
		strcpy(row, mxmlGetOpaque(xml_node));
		for (int x = 0; x < 8; x++) {
			c.x = x;
			c.y = y - 1;
			board_piece(game_settings->board, c) = (row[x] == '_') ? EMPTY : row[x];
		}
	}
	//mxmlDelete(xml_tree);
	return TRUE;
}


int save_game(char * path, settings * game_settings) {
	FILE * fp;
	mxml_node_t * tree;
	mxml_node_t * game_node;
	mxml_node_t * board_node;
	cord c;
	char row_name[6] = "row_x";
	char row[9] = { 0 };
	fp = fopen(path, "w");
	if (fp == NULL)
		return FALSE;

	tree = mxmlNewXML("1.0");
	game_node = mxmlNewElement(tree, "game");
	mxmlNewInteger(mxmlNewElement(game_node, "game_mode"), game_settings->mode);
	mxmlNewOpaque(mxmlNewElement(game_node, "user_color"), color_string(game_settings->color));
	mxmlNewOpaque(mxmlNewElement(game_node, "next_turn"), color_string(game_settings->next));

	if (game_settings->minimax_depth == BEST_DIFFICULTY)
		mxmlNewOpaque(mxmlNewElement(game_node, "user_color"), "best");
	else
		mxmlNewInteger(mxmlNewElement(game_node, "difficulty"), game_settings->minimax_depth);

	board_node = mxmlNewElement(game_node, "board");

	for (int y = 8; y > 0; y--) {
		for (int x = 0; x < 8; x++) {
			c.x = x;
			c.y = y - 1;
			row[x] = (board_piece(game_settings->board, c) == EMPTY) ? '_' : board_piece(game_settings->board, c);
		}
		row_name[4] = '0' + y;
		mxmlNewOpaque(mxmlNewElement(board_node, row_name), row);
	}

	mxmlSaveFile(tree, fp, MXML_NO_CALLBACK);
	fclose(fp);
	mxmlDelete(tree);

	return TRUE;


}




/*******************************************************************/
/*************************** game state ****************************/
/*******************************************************************/

/*
*	compute computer's next move and play.
*	return move in case of success, in case of lose/tie - return LOSE_CODE / TIE_CODE.
*	in case of an error - exit
*/
move computer_turn(settings * game_settings) {
	moves best_moves = best_next_moves(*game_settings, game_settings->next); //find next move
	move_node * random_best_move;
	move best_move;
	if (best_moves.len == -1) { // there was an error
		return error_move;
	}
	if (best_moves.len == 0) { // no possible moves - player wins
		best_move.promotion = NO_MOVE_CODE;
		return best_move;
	}
	else { // play best_move
		int r = rand() % best_moves.len;
		random_best_move = best_moves.first;
		for (int i = 0; i < r; i++) {
			random_best_move = random_best_move->next;
		};
		best_move = *(move*)random_best_move->data;
		board_copy(best_move.board, game_settings->board);
		free_list(&best_moves, &free);
		game_settings->is_next_checked = (is_king_checked(other_player(game_settings->next), game_settings->board));
		return best_move;
	}
}


/* generating all valid moves on board, return all moves as a linked list
* player = color of the for whom we want to get moves
* in case of an error - return error_moves
*/
moves make_all_moves(settings * set){
	check_castling_conditions(set);
	int player = set->next;
	moves all_moves = { 0 };
	cord piece;
	int piece_color;
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			piece.x = i;
			piece.y = j;
			piece_color = (which_color(board_piece(set->board, piece)));
			if (piece_color == player) { // this cord contains a relevant-color piece
				moves simple_moves = get_simple_moves(set, piece);
				if (simple_moves.len == -1){ // get_simple_moves had an error
					free_list(&all_moves, &free);
					return simple_moves;
				}
				else{// if (simple_moves.len != 0) 
					concat(&all_moves, simple_moves);
				}
			}
		}
	}
	return all_moves;
}

/*
* return the non-eat (simple) moves.
* curr = starting coordinate of the move
* max_step = length of maximal possible step (1 for man, BOARD_SIZE for king)
* in case of an error - return error_moves
*/
moves get_simple_moves(settings * set, cord curr){
	char piece = board_piece(set->board, curr);
	int color = which_color(piece);
	int type = tolower(piece);
	moves all_simple_moves = { 0 };
	switch (type)
	{
	case PAWN:
		return pawn_moves(set, curr, color);
	case BISHOP:
		return bishop_moves(set, curr, color, FALSE);
	case ROOK:
		return rook_moves(set, curr, color, FALSE);
	case KNIGHT:
		return knight_moves(set, curr, color);
	case QUEEN:
		return queen_moves(set, curr, color, FALSE);
	case KING:
		return king_moves(set, curr, color);
	default:
		return all_simple_moves;
	}
}

/*
*	return the first location on the board of piece of kind 'piece'
*/

cord find_piece(char piece, char board[BOARD_SIZE][BOARD_SIZE]) {
	int stop = FALSE;
	cord c = error_cord;
	// find piece's location
	for (int i = 0; (i < BOARD_SIZE) && !stop; i++){
		for (int j = 0; (j < BOARD_SIZE) && !stop; j++){
			c.x = i;
			c.y = j;
			if (board_piece(board, c) == piece) {
				stop = TRUE; //break both loops
			}
		}
	}
	return c;
}

/*
*	check if c is checked by other color
*/
int is_cord_checked(cord c, int color, char board[BOARD_SIZE][BOARD_SIZE]){
	cord temp;
	int i;
	char other_king = (color == WHITE) ? BLACK_K : WHITE_K;
	char other_pawn = (color == WHITE) ? BLACK_P : WHITE_P;
	char other_bishop = (color == WHITE) ? BLACK_B : WHITE_B;
	char other_queen = (color == WHITE) ? BLACK_Q : WHITE_Q;
	char other_knight = (color == WHITE) ? BLACK_N : WHITE_N;
	char other_rook = (color == WHITE) ? BLACK_R : WHITE_R;
	cord dangerous_diags[32];
	cord dangerous_vertical[32];
	cord dangerous_horizontal[32];
	cord dangerous_knight[32];

	move_cords(board, c, BOARD_SIZE, color, TRUE, TRUE, dangerous_diags); //find diagnal dangerous pieces
	move_cords(board, c, BOARD_SIZE, color, TRUE, FALSE, dangerous_horizontal); //find horizontal dangerous pieces
	move_cords(board, c, BOARD_SIZE, color, FALSE, TRUE, dangerous_vertical); //find vertical dangerous pieces
	knight_cords(board, c, color, dangerous_knight);

	// check if enemy's pieces that are not pawns or king threatening c
	for (i = 0; i < 32; i++){
		if ((is_valid_cord(dangerous_diags[i]) && ((board_piece(board, dangerous_diags[i]) == other_bishop) ||
			(board_piece(board, dangerous_diags[i]) == other_queen))) ||
			(is_valid_cord(dangerous_horizontal[i]) && ((board_piece(board, dangerous_horizontal[i]) == other_rook) ||
			(board_piece(board, dangerous_horizontal[i]) == other_queen))) ||
			(is_valid_cord(dangerous_vertical[i]) && ((board_piece(board, dangerous_vertical[i]) == other_rook) ||
			(board_piece(board, dangerous_vertical[i]) == other_queen))) ||
			(is_valid_cord(dangerous_knight[i]) && ((board_piece(board, dangerous_knight[i]) == other_knight))))
			return TRUE;
	}
	// check if emeny's pawn is threatening c
	temp.y = c.y;
	temp.y += (color == WHITE) ? 1 : -1;
	for (i = -1; i <= 1; i += 2){
		temp.x = c.x + i;
		if ((is_valid_cord(temp) && board_piece(board, temp) == other_pawn))
			return TRUE;
	}
	//check if emeny's king is threatening c
	for (i = -1; i <= 1; i += 1){
		for (int j = -1; j <= 1; j += 1){
			temp.x = c.x + i;
			temp.y = c.y + j;
			if (is_valid_cord(temp) && (board_piece(board, temp) == other_king)) {
				return TRUE;
			}
		}
	}

	return FALSE;
}

/*
*	check if the color-king is checked (assuming there is a king on the board)
*/
int is_king_checked(int color, char board[BOARD_SIZE][BOARD_SIZE]) {
	char piece = (color == BLACK) ? BLACK_K : WHITE_K;
	cord c = find_piece(piece, board);
	return is_cord_checked(c, color, board);
}


moves pawn_moves(settings * set, cord curr, int color) {
	moves piece_simple_moves;
	piece_simple_moves.len = 0;
	cord dest;
	move * single_move;
	moves promotions;
	int dest_color;
	int y_dir = (color == WHITE) ? 1 : -1;

	dest.y = curr.y + y_dir;

	for (int x = -1; x <= 1; x++) {
		single_move = malloc(sizeof(move));
		if (single_move == NULL) {
			free_list(&piece_simple_moves, &free);
			return error_moves;
		}
		single_move->start = curr;
		single_move->promotion = FALSE;
		single_move->is_castle = FALSE;
		dest.x = curr.x + x;
		if (is_valid_cord(dest)) {
			dest_color = which_color(board_piece(set->board, dest));
			//logical XOR: check if pawn can eat XOR move up
			if (color == dest_color) {
				free(single_move);
				continue;
			}
			if ((x == 0) != (dest_color == other_player(color))) {
				single_move->end = dest;
				board_copy(set->board, single_move->board);
				move_from_to(single_move->board, curr, dest);
				// check if psaudo-legal move is legal
				if (is_king_checked(color, single_move->board)) {
					free(single_move);
					continue;
				}
				if (dest.y == promotion_row(color)) {
					promotions = promote(single_move);
					if (promotions.len == -1){
						free(single_move);
						free_list(&piece_simple_moves, &free);
						return promotions;
					}
					concat(&piece_simple_moves, promotions);
				}
				else if (!add_node(&piece_simple_moves, single_move, sizeof(move))) { //could not add node to linked list
					free(single_move);
					free_list(&piece_simple_moves, &free);
					return error_moves;
				}
				free(single_move);
			}
		}
	}
	return piece_simple_moves;
}

moves promote(move * single_move) {
	moves all_promotions;
	all_promotions.len = 0;
	move * new_promotion;
	char all_pieces[7] = "bnrq";

	for (int i = 0; i < 4; i++){
		new_promotion = malloc(sizeof(move));
		if (single_move == NULL) {
			free_list(&all_promotions, &free);
			return error_moves;
		}

		copy_move(single_move, new_promotion);
		new_promotion->promotion = TRUE;
		new_promotion->is_castle = FALSE;
		char promoted = islower(board_piece(single_move->board, single_move->end)) ? all_pieces[i] : toupper(all_pieces[i]);
		board_piece(new_promotion->board, new_promotion->end) = promoted;
		if (!add_node(&all_promotions, new_promotion, sizeof(move))) { //could not add node to linked list
			free(new_promotion);
			free_list(&all_promotions, &free);
			return error_moves;
		}
		free(new_promotion);
	}
	return all_promotions;
}


/*
*	return moves. in each move:
*			start = start_cord
*			end = one of the cords in end_cords (there can be at most 31 end cords)
*			board = currend board + piece moved from start to end.
*	this method is not relevant for pawns.
*/
moves cords_to_moves(cord start_cord, cord end_cords[32], char board[BOARD_SIZE][BOARD_SIZE], int color) {
	cord end_cord;
	int i = 0;
	move * new_move;
	moves new_moves = { 0 };
	while (is_valid_cord(end_cord = end_cords[i++])) {
		new_move = malloc(sizeof(move));
		if (new_move == NULL)
		{
			free_list(&new_moves, &free);
			return error_moves;
		}

		new_move->start = start_cord;
		new_move->end = end_cord;
		new_move->promotion = FALSE;
		new_move->is_castle = FALSE;
		board_copy(board, new_move->board);
		move_from_to(new_move->board, start_cord, end_cord);
		// check if psaudo-legal move is legal
		if (is_king_checked(color, new_move->board)){
			//	free(new_move);
			continue;
		}
		else if (!add_node(&new_moves, new_move, sizeof(move))){
			free(new_move);
			free_list(&new_moves, &free);
			return error_moves;
		}
		free(new_move);
	}
	return new_moves;
}

void move_cords(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int max_move, int color, int move_x, int move_y, cord end_cords[32]) {
	cord dest;
	int dest_color;

	// initialize end_cords
	if (DEBUG1){
		if (curr.x < -42) {
			printf("%s\n", "NO KING!");
		}
	}
	int i;
	for (i = 0; i < 32; i++){
		end_cords[i] = error_cord;
	}

	i = 0;
	for (int x_direction = (-move_x); x_direction <= move_x; x_direction += 2){
		for (int y_directrion = (-move_y); y_directrion <= move_y; y_directrion += 2) {
			for (int k = 1; k <= max_move; k++) {
				dest.x = curr.x + (k*x_direction);
				dest.y = curr.y + (k*y_directrion);
				// check if piece can walk to this destination
				if (!is_valid_cord(dest))
					break;
				dest_color = which_color(board_piece(board, dest));
				if (dest_color == color) //if destination is same color piece - cannot move this waty
					break;
				else
					end_cords[i++] = dest;
				if (dest_color == other_player(color)) //if ate - cannot continue moving in the same direction
					break;
			}

		}
	}
}

moves bishop_moves(settings  * set, cord curr, int color, int is_king){
	cord diag_cords[32];
	int max_move = is_king ? 1 : BOARD_SIZE;
	move_cords(set->board, curr, max_move, color, TRUE, TRUE, diag_cords);
	return cords_to_moves(curr, diag_cords, set->board, color);
}


moves rook_moves(settings  * set, cord curr, int color, int is_king) {
	int max_move = is_king ? 1 : BOARD_SIZE;
	cord horizontal_cords[32];
	cord vertical_cords[32];
	move castle_move;

	move_cords(set->board, curr, max_move, color, TRUE, FALSE, horizontal_cords);
	moves horizontal_moves = cords_to_moves(curr, horizontal_cords, set->board, color);
	if (horizontal_moves.len == -1)
		return error_moves;
	move_cords(set->board, curr, max_move, color, FALSE, TRUE, vertical_cords);
	moves vertical_moves = cords_to_moves(curr, vertical_cords, set->board, color);
	if (vertical_moves.len == -1) {
		free_list(&horizontal_moves, &free);
		return error_moves;
	}
	else
		concat(&horizontal_moves, vertical_moves);
	if (tolower(board_piece(set->board, curr)) == ROOK) {//check if need to castle
		castle_move = get_castling_move(set, curr, color);
		if (castle_move.is_castle == TRUE)
			if (!add_node(&horizontal_moves, &castle_move, sizeof(move))) {
				free_list(&horizontal_moves, &free);
				return error_moves;
			}
	}
	return horizontal_moves;
}

moves queen_moves(settings  * set, cord curr, int color, int is_king){
	moves all_simple_moves = bishop_moves(set, curr, color, is_king);
	if (all_simple_moves.len == -1)
		return error_moves;
	moves more_moves = rook_moves(set, curr, color, is_king);
	if (more_moves.len == -1) {
		free_list(&all_simple_moves, &free);
		return error_moves;
	}
	else
		concat(&all_simple_moves, more_moves);
	return all_simple_moves;
}

moves king_moves(settings  * set, cord curr, int color) {
	return queen_moves(set, curr, color, TRUE);
}

move get_castling_move(settings  * set, cord curr, int color) {
	cord c0 = { 0, curr.y };
	cord c1 = { 1, curr.y };
	cord c2 = { 2, curr.y };
	cord c3 = { 3, curr.y };
	cord c5 = { 5, curr.y };
	cord c6 = { 6, curr.y };
	cord c7 = { 7, curr.y };
	move castle = error_move;
	cord king_dest;
	cord king_loc = { 4, (color == WHITE) ? 0 : 7 };

	int king_moved = (color == WHITE) ? set->white_king_moved : set->black_king_moved;
	int rook1_moved = (color == WHITE) ? set->white_rook_1_moved : set->black_rook_1_moved;
	int rook2_moved = (color == WHITE) ? set->white_rook_2_moved : set->black_rook_2_moved;

	int relevant_rook_moved = ((rook1_moved && (curr.x == 0)) || (rook2_moved && (curr.x == 7)));

	if ((is_same_cord(curr, c0) || is_same_cord(curr, c7)) &&	// if rook is not on one of these cords - cannot castle
		!king_moved &&											//if king moved - cannot castle
		!is_king_checked(color, set->board) &&					//if king is checked - cannot castle
		!relevant_rook_moved) {									//if relevant rook moved - cannot castle

		if (curr.x == 0) { //relevant rook is rook1 (big castle)
			if (((board_piece(set->board, c1) == EMPTY) &&
				(board_piece(set->board, c2) == EMPTY) &&
				(board_piece(set->board, c3) == EMPTY) && !is_cord_checked(c3, color, set->board))) {
				castle.end = c3;
				king_dest = c6;
			}
			else // relevant rook cannot castle
				return castle;
		}
		else if (curr.x == 7){ //relevanot rook is rook2 (small castle)
			if ((board_piece(set->board, c5) == EMPTY) &&
				(board_piece(set->board, c6) == EMPTY) && !is_cord_checked(c6, color, set->board)) {
				castle.end = c5;
				king_dest = c6;
			}
			else // relevant rook cannot castle
				return castle;
		}
		castle.start = curr;
		castle.is_castle = TRUE;
		castle.promotion = FALSE;
		board_copy(set->board, castle.board);
		move_from_to(castle.board, castle.start, castle.end);
		move_from_to(castle.board, king_loc, king_dest);
	}
	return castle;
}

/*
*	return all possible moves for knight in location curr.
*/

moves knight_moves(settings * set, cord curr, int color){
	cord jump_cords[32];
	knight_cords(set->board, curr, color, jump_cords);
	return cords_to_moves(curr, jump_cords, set->board, color);
}


/*
*	return possible destination for knight in curr position.
*		-board: current borad layout
*		-curr: knight's location
*		-color: knight's color
*		-jump_cords: array of cords in which the cords will be returned
(the method changes the array and returns void value)
*/

void knight_cords(char board[BOARD_SIZE][BOARD_SIZE], cord curr, int color, cord jump_cords[32]) {
	int i, j, k, l, x, y;
	for (i = 0; i < 32; i++){
		jump_cords[i] = error_cord;
	}
	l = 0;
	for (i = -1; i <= 1; i += 2) {
		for (j = -2; j <= 2; j += 4){
			x = i, y = j;
			for (k = 0; k < 2; k++){
				x ^= y;
				y ^= x;
				x ^= y;
				cord jump_to = { curr.x + x, curr.y + y };
				if (is_valid_cord(jump_to) && (which_color(board_piece(board, jump_to)) != color))
					jump_cords[l++] = jump_to;
			}
		}
	}
}

/*
*	move piece in start to end.
*/


void move_from_to(char board[BOARD_SIZE][BOARD_SIZE], cord start, cord end) {
	if (is_valid_cord(start) && is_valid_cord(end)) {
		board_piece(board, end) = board_piece(board, start);
		board_piece(board, start) = EMPTY;
	}
}
/****************************************************************/
/********************** Minimax Algorithm ***********************/
/****************************************************************/

/* find and return the best computer's next move acording to minimax algorithm.
* in case of an error - return a move with .steps_len = -1
* (envelope function that calls minimax())
*/
moves best_next_moves(settings set, int maximizer) {
	// minimax function
	moves best_moves = { 0 };
	move * temp_best_move;
	moves possible_moves = make_all_moves(&set);
	int best_score = INT_MIN;
	int curr_alpha = INT_MIN;
	if (possible_moves.len == -1) { //error
		return possible_moves;
	}

	int is_best_difficulty = (set.minimax_depth == BEST_DIFFICULTY);
	int depth = (!is_best_difficulty) ? set.minimax_depth : get_best_depth(&set, maximizer);

	move_node * curr = possible_moves.first;
	while (curr != NULL) {
		//get the score of each next possible moves, according to minimax algorithm
		settings next_set;
		memcpy(&next_set, &set, sizeof(settings));
		next_set.next = other_player(set.next);
		board_copy(((move*)curr->data)->board, next_set.board);
		check_castling_conditions(&next_set);
		int curr_score = minimax(next_set, curr_alpha, INT_MAX, FALSE, depth - 1, is_best_difficulty);
		if (curr_score == SCORE_ERROR) {
			free_list(&possible_moves, &free);
			free_list(&best_moves, &free);
			return error_moves;
		}
		if (curr_score > curr_alpha)
			curr_alpha = curr_score;
		temp_best_move = curr->data;
		if (curr_score > best_score) { // if new score is higher - free previous best moves
			free_list(&best_moves, &free);
			best_score = curr_score;
		}
		if (!(curr_score < best_score)) {
			if (!add_node(&best_moves, temp_best_move, sizeof(move))) {
				free_list(&best_moves, &free);
				free_list(&possible_moves, &free);
				return error_moves;
			}
		}
		curr = curr->next;
	}
	free_list(&possible_moves, &free);
	if (DEBUG)
		printf("CHOSEN SCORE: %d\n", best_score);
	return best_moves;
}

/* minimax algorithm with alpha-beta prunning.
* the function returns score (int) according to the minimax algorithm
* and the score() function. returns ERROR_SCORE in case of an error.
*/
int minimax(settings set, int alpha, int beta, int is_maxi_player, int depth, int is_best_difficulty){
	if (DEBUG){
		printf("WK: %d WR1: %d WR2: %d ||||||| BK: %d BR1: %d BR2: %d\n",
			set.white_king_moved, set.white_rook_1_moved, set.white_rook_2_moved,
			set.black_king_moved, set.black_rook_1_moved, set.black_rook_2_moved);
		print_board(set.board);
	}
	int player = set.next;
	if (depth == 0) {
		int scorrer = is_maxi_player ? player : other_player(player);
		int board_score = score(&set, scorrer, player, is_best_difficulty); //return score according to maximizing player
		return board_score;
	}
	moves possible_moves = make_all_moves(&set); // create all possible moves for player
	if (possible_moves.len == -1) { //there was an error, return an error score
		return SCORE_ERROR;
	}
	else if (possible_moves.len == 0){ // reached a "leaf" at the minimax tree (no more possible moves from here)
		if (is_king_checked(player, set.board))
			return is_maxi_player ? INT_MIN : INT_MAX;
		else // tie
			return TIE_SCORE;
	}
	int minimax_score = is_maxi_player ? INT_MIN : INT_MAX;
	move_node* curr = possible_moves.first;
	while (curr != NULL){ // continue recursively for each possible move
		move * cur_move = curr->data;
		settings next_set;
		memcpy(&next_set, &set, sizeof(settings));
		next_set.next = other_player(player);
		check_castling_conditions(&next_set)
		int cur_score = minimax(next_set, alpha, beta, !is_maxi_player, depth - 1, is_best_difficulty);
		if (cur_score == SCORE_ERROR) { //there was an error, return an error score
			free_list(&possible_moves, &free);
			return SCORE_ERROR;
		}
		// get maximum\minimum v_score according to is_maxi_player
		minimax_score = is_maxi_player ? maxi(minimax_score, cur_score) : mini(minimax_score, cur_score);
		if (is_maxi_player)
			alpha = maxi(alpha, minimax_score);
		else
			beta = mini(beta, minimax_score);
		if (beta <= alpha)
			break; //pruning
		curr = curr->next;
	}
	free_list(&possible_moves, &free);
	return minimax_score;
}

int get_best_depth(settings * set, int player) {
	int board_score = score(set, player, player, TRUE);
	if (board_score == 0)
		return 3;
	else if (board_score > 0)
		return 4;
	else
		return 2;
}

int can_piece_move(settings * set, cord piece) {
	moves possible_moves;
	int num_of_moves;

	possible_moves = get_simple_moves(set, piece);
	num_of_moves = possible_moves.len;
	free_list(&possible_moves, &free);
	if (num_of_moves == -1) //there was an error, return an error score
		return -1;
	// once one of the current_player can move, they do not lose,
	// no need to preform this check again.
	else if (num_of_moves == 0)
		return FALSE;
	else
		return TRUE;
}

/* calculates the board's score.
* scoring_player = color of the minimax-maximizer player
* current_player = color of the player whose turn it is now.
*
* the function returns:
*				WIN_SCORE if scoring_player wins this board
*				LOSE_SCORE if scoring_player loses this board
*				scoring_player's total pieces value minus other player's total pieces value otherwise
*				ERROR_SCORE in case of an error
*/
int score(settings * set, int scoring_player, int current_player, int is_best){
	int total_score;
	cord piece;
	int no_player_moves = TRUE;
	int no_other_moves = TRUE;
	int no_scoring_king = TRUE;
	int no_other_king = TRUE;
	int can_move;
	int player_score = 0;
	int other_player_score = 0;
	int is_scoring_piece;
	int is_scoring_checked = is_king_checked(current_player, set->board);
	int is_other_checked = is_king_checked(other_player(current_player), set->board);

	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			piece.x = i;
			piece.y = j;
			char cur_piece = board_piece(set->board, piece);
			if (cur_piece == EMPTY)
				continue;

			int piece_color = which_color(cur_piece);
			is_scoring_piece = (piece_color == scoring_player);

			int piece_score = get_piece_score(cur_piece);
			if (piece_score == KING_SCORE) {
				no_scoring_king = (no_scoring_king && !is_scoring_piece);
				no_other_king = (no_other_king && is_scoring_piece);
			}
			if (is_scoring_piece)
				player_score += piece_score;
			else //piece_color is other player's color
				other_player_score += piece_score;

			// we have to check that current_player has at least one piece that
			// can move, otherwise current_player loses or it's a tie. also, 
			// we have to check that other player can move (for the same reason)
			if (no_player_moves && (piece_color == current_player) ||
				(no_other_moves && (piece_color == other_player(current_player)))) {
				can_move = can_piece_move(set, piece);
				if (can_move == -1)
					return SCORE_ERROR;
				else if (can_move) {
					if (piece_color == current_player)
						no_player_moves = FALSE;
					else
						no_other_moves = FALSE;
				}
			}
		}
	}
	// current_player cannot play.
	if (no_player_moves) {
		if (is_scoring_checked) // checkmate
			total_score = (scoring_player == current_player) ? LOSE_SCORE : WIN_SCORE;
		else // tie
			total_score = is_best ? 0 : TIE_SCORE; // in best difficulty: 0, otherwise: better only than LOSE_SCORE
	}
	// check if other player can move
	else if (no_other_moves){
		if (is_other_checked)
			total_score = (scoring_player == current_player) ? WIN_SCORE : LOSE_SCORE;
		else // tie
			total_score = is_best ? 0 : TIE_SCORE; // in best difficulty: 0, otherwise: better only than LOSE_SCORE
	}
	// scoring_player's king has ceased to be
	else if (no_scoring_king)
		total_score = LOSE_SCORE;
	// other player's king is pushing up the daisies
	else if (no_other_king)
		total_score = WIN_SCORE;

	//RELEVANT ONLY FOR best DIFFICULTY!
	else if (is_best && is_scoring_checked) {
		if (is_scoring_checked)
			return LOSE_SCORE + 42;
		else if (is_other_checked)
			return WIN_SCORE - 42;
	}

	// both players' kings are alive and pining for the fjords
	else
		total_score = player_score - other_player_score;
	return total_score;
}

int get_piece_score(char player_piece) {
	char piece = tolower(player_piece);
	switch (piece)
	{
	case PAWN:
		return PAWN_SCORE;
	case KNIGHT:
		return KNIGHT_SCORE;
	case BISHOP:
		return BISHOP_SCORE;
	case ROOK:
		return ROOK_SCORE;
	case QUEEN:
		return QUEEN_SCORE;
	case KING:
		return KING_SCORE;
	default:
		return 0;
	}
}

move get_best_move(settings * game_settings) {
	moves best_moves = best_next_moves(*game_settings, game_settings->next); //find next move
	move_node * random_best_move;
	move best_move;
	if (best_moves.len == -1) { // there was an error
		return error_move;
	}
	else { // play best_move
		int r = rand() % best_moves.len;
		random_best_move = best_moves.first;
		for (int i = 0; i < r; i++) {
			random_best_move = random_best_move->next;
		};
		best_move = *(move*)random_best_move->data;
		//board_copy(best_move->board, game_settings->board);
		free_list(&best_moves, &free);
		//game_settings->is_next_checked = (is_king_checked(other_player(game_settings->next), game_settings->board));
		return best_move;
	}
}
