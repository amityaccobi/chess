#include "Console.h"

int console_mode()
{
	settings game_settings = settings_state();
	move next_move;
	int was_checked = FALSE;
	//game state
	while (TRUE) {
		// user turn
		if ((game_settings.mode == PLAYER_VS_PLAYER) || (game_settings.next == game_settings.color)) {
			next_move = user_turn(&game_settings, was_checked);
			if (next_move.promotion == -1) {
				exit(0);
			}
		}
		//computer turn
		else {
			next_move = computer_turn(&game_settings);
			if (next_move.promotion == -1) {
				exit(0);
			}
			else if (next_move.promotion != NO_MOVE_CODE) {
				if (was_checked)
					print_message(CHECK);
				if (printf("Computer: move ") < 0) {
					perror_message("printf");
					exit(0);
				}

				if ((print_move(&next_move) < 0)) {
					exit(0);
				}
			}

		}
		int game_code = next_move.promotion;

		if (game_code == NO_MOVE_CODE) {
			if (game_settings.is_next_checked)
				print_win(color_string(other_player((game_settings.next))));
			else
				print_message(TIE);
			exit(0);
		}
		else {
			if (print_board(next_move.board) < 0)
				exit(0);
		}
		//switch next player
		game_settings.next = other_player(game_settings.next);
		was_checked = game_settings.is_next_checked;
	}
	return 0;
}


/***************/
/**** board ****/
/***************/

/*
*  prints board's line. returns 1 if succeeded, otherwise returns -1
*/
int print_line(){
	int i;
	if (printf(" |") < 0) {
		perror_message("printf");
		return -1;
	}
	for (i = 1; i < BOARD_SIZE * 4; i++){
		if (printf("-") < 0) {
			perror_message("printf");
			return -1;
		};
	}
	if (printf("|\n") < 0) {
		perror_message("printf");
		return -1;
	}
	return 1;
}


/*
*  prints the layout of board. returns 1 if succeded, otherwise returns -1.
*/

int print_board(char board[BOARD_SIZE][BOARD_SIZE])
{
	char c;
	int i, j;
	if (print_line() < 0) {
		return -1;
	}
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		if (printf((j < 9 ? "%d" : "%d"), j + 1) < 0) {
			perror_message("printf");
			return -1;
		}
		for (i = 0; i < BOARD_SIZE; i++){
			c = (board[i][j] == EMPTY) ? ' ' : board[i][j];
			if (printf("| %c ", c) < 0) {
				perror_message("printf");
				return -1;
			}
		}
		if (printf("|\n") < 0) {
			perror_message("printf");
			return -1;
		};
		if (print_line() < 0)
			return -1;
	}
	if (printf("  ") < 0) {
		perror_message("printf");
		return -1;
	}

	for (j = 0; j < BOARD_SIZE; j++){
		if (printf(" %c  ", (char)('a' + j)) < 0){
			perror_message("printf");
			return -1;
		}
	}
	if (printf("\n") < 0) {
		perror_message("printf");
		return -1;
	}
	return 1;
}

/****************************************************************/
/*************************** parsing ****************************/
/****************************************************************/

/*
* setting state. returns struct containing the settings of the game.
* changes game-board in place.
* in case of an error - exit
*/
settings settings_state() {
	int depth = 1;
	int mode = 2;
	int next = WHITE;
	int user_color = WHITE;
	char input[51];
	moves possible_moves;
	settings game_settings;


	//default settings:
	game_settings.color = user_color;
	game_settings.minimax_depth = depth;
	game_settings.mode = mode;
	game_settings.next = next;
	clear(game_settings.board);
	init_board(game_settings.board);
	if (!print_board(game_settings.board))
		exit(0);
	while (TRUE) {
		print_message(ENTER_SETTINGS);
		if (!read_input(input)) {
			exit(0);
		};
		// commands arguments are after ' ' char
		char * args = strchr(input, ' ') + 1;
		cord new_cord;

		// parse: game_mode x
		if (is_command_with_args(GAME_MODE)) {
			mode = atoi(args);
			if ((mode == PLAYER_VS_PLAYER) || (mode == PLAYER_VS_COMP)) {
				game_settings.mode = mode;
				char * mode_message = (mode == PLAYER_VS_PLAYER) ? TWO_PLAYERS_GAME_MODE : PLAYER_VS_AI_GAME_MODE;
				print_message(mode_message);
			}
			else
				print_message(WRONG_GAME_MODE);
		}
		// parse: rm <x,y>
		else if is_command_with_args(REMOVE_SETTING) {
			new_cord = parse_cord(args);
			if (is_valid_cord(new_cord)) {
				board_piece(game_settings.board, new_cord) = EMPTY;
			}
			else
				print_message(WRONG_POSITION);
		}
		// parse: set <x,y> color type
		else if is_command_with_args(SET_SETTING) {
			new_cord = parse_cord(args);
			if (is_valid_cord(new_cord)) {
				char tmp_piece = game_settings.board[new_cord.x][new_cord.y];
				char * piece_type = strrchr(input, ' ') + 1;
				int color = (strstr(input, "white") != NULL) ? WHITE : BLACK;
				char piece_to_add = piece_from_string(piece_type, color);
				board_piece(game_settings.board, new_cord) = piece_to_add;
				if (is_over_max(game_settings.board, piece_to_add)){
					print_message(NO_PIECE);
					// return board to former state
					board_piece(game_settings.board, new_cord) = tmp_piece;
				}
			}
			else
				print_message(WRONG_POSITION);
		}
		// parse: difficulty d
		else if ((is_command_with_args(DIFFICULTY)) && (game_settings.mode == PLAYER_VS_COMP)) {
			if (strcmp(args, BEST) == 0)
				depth = 0;
			else if (strstr(input, DEPTH)) {
				depth = atoi(args + 6);
				if ((depth < 1) || (depth > 4)) {
					print_message(WRONG_MINIMAX_DEPTH);
				}
			}
			game_settings.minimax_depth = depth;
		}
		// parse: user_color color
		else if (is_command_with_args(COLOR_SETTING) && (game_settings.mode == PLAYER_VS_COMP)) {
			if (strstr(input, "white") != NULL)
				game_settings.color = WHITE;
			else if (strstr(input, "black") != NULL)
				game_settings.color = BLACK;
		}
		// parse : next_player a
		else if (is_command_with_args(NEXT_PLAYER)) {
			if (strstr(input, "white") != NULL)
				game_settings.next = WHITE;
			else if (strstr(input, "black") != NULL)
				game_settings.next = BLACK;
		}
		// parse: load x
		else if is_command_with_args(LOAD) {
			if (!load_game(args, &game_settings))
				print_message(WRONG_FILE_NAME);
			else if (!print_board(game_settings.board))
				exit(0);
		}

		// parse: clear
		else if is_command(CLEAR_SETTING) {
			clear(game_settings.board);
		}
		// parse: print
		else if is_command(PRINT_SETTING) {
			if (print_board(game_settings.board) < 0) { //error printing board
				exit(0);
			}
		}
		// parse: quit
		else if is_command(QUIT) {
			exit(0);
		}
		// parse: start
		else if is_command(START_SETTING){
			// start game only if board is valid
			if (is_valid_board(game_settings.board) &&
				((possible_moves = make_all_moves(game_settings)).len != 0)) {
				if (possible_moves.len > 0) {
					free_list(&possible_moves, free);
					break;
				}
				else if (possible_moves.len == -1) //error creating possible moves
					exit(0);
			}
			else
				print_message(WRONG_BOARD_INITIALIZATION);
		}
		else
			print_message(ILLEGAL_COMMAND);
	}
	return game_settings;
}

/*
* read user's input.
* in case of an error - return 0
*/
int read_input(char input[51]) {
	char c;
	int input_size = 0;
	while ((c = (char)getchar()) != '\n'){
		if (c == EOF) {
			perror_message("getchar");
			return 0;
		}
		input[input_size++] = c;
	}
	input[input_size] = '\0';
	return 1;
}

/* parse string into cord.
* in case of an error - return {-1, -1}
*/
cord parse_cord(char * cord_str) {
	cord c;
	int x = -1;
	int y = -1;
	if (cord_str != NULL) {

		if (cord_str[0] == '<'){
			x = cord_str[1] - 'a';
			y = atoi(cord_str + 3) - 1;
		}
	}
	c.x = x;
	c.y = y;
	return c;
}

move parse_move(char * movestr, char board[BOARD_SIZE][BOARD_SIZE]){
	move new_move;
	// parse start coordinate of move
	char * cord_ptr = strchr(movestr, '<');
	new_move.start = parse_cord(cord_ptr);
	cord_ptr = strchr(cord_ptr + 1, '<');
	// parse end coordinate of move
	new_move.end = parse_cord(cord_ptr);
	new_move.promotion = (strchr(cord_ptr + 1, ' ') != NULL);
	board_copy(board, new_move.board);
	move_from_to(new_move.board, new_move.start, new_move.end);
	return new_move;
}


/*
* prints description of a move.
* returns 1 if succeded, otherwise return -1.
*/
int print_move(move * move0) {
	if (print_cord(move0->start) < 0) {
		perror_message("printf");
		return -1;
	}
	if (printf(" to ") < 0) {
		perror_message("printf");
		return -1;
	}
	if (print_cord(move0->end) < 0) {
		perror_message("printf");
		return -1;
	}
	if (move0->promotion) {
		if (print_type(board_piece(move0->board, move0->end)) < 0) {
			perror_message("printf");
			return -1;
		}
	}
	if (printf("\n") < 0) {
		perror_message("printf");
		return -1;
	}
	return 1;
}

char piece_from_string(char * piece_char, int color) {
	char piece;
	if (strstr("pawn", piece_char) != NULL)
		piece = PAWN;
	else if (strstr("bishop", piece_char) != NULL)
		piece = BISHOP;
	else if (strstr("knight", piece_char) != NULL)
		piece = KNIGHT;
	else if (strstr("rook", piece_char) != NULL)
		piece = ROOK;
	else if (strstr("queen", piece_char) != NULL)
		piece = QUEEN;
	else
		piece = KING;
	piece = (color == WHITE) ? piece : toupper(piece);
	return piece;
}

int print_type(char ch) {
	char c = tolower(ch);
	char str[10] = { 0 };
	switch (c)
	{
	case QUEEN:
		strcpy(str, "queen");
		break;
	case BISHOP:
		strcpy(str, "bishop");
		break;
	case ROOK:
		strcpy(str, "rook");
		break;
	case KNIGHT:
		strcpy(str, "knight");
		break;
	default:
		break;
	}
	return printf(" %s", str);
}

/* parse user's input and play their move
*
* * in case of an error - exit.
*/
move user_turn(settings * game_settings, int was_checked) {
	extern move error_move;
	char input[51];
	char * args;
	move temp_move;
	char* cord_ptr;
	moves possible_moves = make_all_moves(*game_settings);
	moves moves_for_piece;
	settings temp_settings;
	int move_score;
	if (possible_moves.len == -1){ // there was an error
		exit(0);
	}
	else if (possible_moves.len == 0) { // no possible moves - computer wins
		temp_move.promotion = NO_MOVE_CODE;
		return temp_move;
	}
	else if (was_checked)
		print_message(CHECK);
	while (TRUE) {
		temp_settings = *game_settings;
		print_message(color_string(game_settings->next));
		print_message(ENTER_YOUR_MOVE);
		move_node * cur;
		if (!read_input(input)) { //error reading input
			free_list(&possible_moves, free);
			return error_move;
		}
		cur = possible_moves.first;
		cord_ptr = strchr(input, '<');
		args = strchr(input, ' ') + 1;

		// parse: move <x,y> to <i,j> [promotion]
		if (is_command_with_args(MOVE)){
			temp_move = parse_move(input, game_settings->board);
			// check if coordinates are valid
			if (!is_valid_cord(temp_move.start) || !is_valid_cord(temp_move.end)) {
				print_message(WRONG_POSITION);
				continue;
			}
			//check if starting cord contains player's piece
			if (which_color(board_piece(game_settings->board, temp_move.start)) != game_settings->next) {
				print_message(NO_DICS);
				continue;
			}
			board_copy(game_settings->board, temp_move.board);
			move_from_to(temp_move.board, temp_move.start, temp_move.end);
			if (temp_move.promotion) {
				cord_ptr = strrchr(input, ' ') + 1;
				board_piece(temp_move.board, temp_move.end) = piece_from_string(cord_ptr, game_settings->next);
			}
			else temp_move.promotion = 0;
			// check if move is valid. if so, get its index in possible_moves
			int valid_move = is_valid_move(possible_moves, temp_move);
			if (valid_move == -1) {
				print_message(ILLEGAL_MOVE);
				continue;
			}
			else // play 
			{
				cur = possible_moves.first;
				for (int i = 0; i < valid_move; i++) {
					cur = cur->next;
				}
				move to_play = *(move *)(cur->data);
				board_copy(to_play.board, game_settings->board);
				free_list(&possible_moves, free);
				game_settings->is_next_checked = is_king_checked(other_player(game_settings->next), game_settings->board);
				return to_play;
			}
		}
		// parse: get_moves <x,y>
		else if (is_command_with_args(GET_MOVES)) {
			cord c = parse_cord(cord_ptr);
			if (!is_valid_cord(c)) {
				print_message(WRONG_POSITION);
				continue;
			}
			else if (which_color(board_piece(game_settings->board, c)) != game_settings->next) {
				print_message(NO_DICS);
			}
			while (cur != NULL) {
				if (is_same_cord(((move*)(cur->data))->start, c)) {
					if (print_move(cur->data) < 0) {
						free_list(&possible_moves, free);
						return error_move;
					}
				}
				cur = cur->next;
			}
		}
		// parse: get_best_moves d
		else if (is_command_with_args(BEST_MOVES)) {
			temp_settings.minimax_depth = atoi(args);
			moves best_moves = best_next_moves(temp_settings, temp_settings.next);
			if (best_moves.len == -1){
				free_list(&possible_moves, free);
				return error_move;
			}
			move_node * curr_move = best_moves.first;
			while (curr_move != NULL) {
				if (print_move(curr_move->data) < 0){
					free_list(&possible_moves, free);
					free_list(&best_moves, free);
					return error_move;
				}
				curr_move = curr_move->next;
			}
		}
		// parse: get_score d m
		else if (is_command_with_args(GET_SCORE)){
			int depth = atoi(args);
			temp_move = parse_move(input, game_settings->board);
			board_copy(temp_move.board, temp_settings.board);
			temp_settings.next = other_player(game_settings->next);
			if (depth != 0) 
				move_score = minimax(temp_settings, INT_MIN, INT_MAX, FALSE, depth - 1, FALSE);
			else // depth == 0 if best [since: atoi("best") = 0]
				move_score = minimax(temp_settings, INT_MIN, INT_MAX, FALSE, get_best_depth(game_settings->board, game_settings->next) - 1, TRUE);

			if (move_score == SCORE_ERROR){
				free_list(&possible_moves, free);
				return error_move;;
			}
			printf("%d\n", move_score);
		}
		else if (is_command_with_args(SAVE)) {
			if (!save_game(args, game_settings)) {
				print_message(WRONG_FILE_NAME);
			}
		}
		// parse: quit
		else if (is_command(QUIT)) {
			free_list(&possible_moves, free);
			exit(0);
		}
		else
			print_message(ILLEGAL_COMMAND);
	}
}
