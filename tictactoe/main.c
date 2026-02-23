#include "logic.h"
#include "ui.h"
#include "bot_logic.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const int DIM = 3;

int main(void)
{
	srand(time(NULL));

	char board[DIM][DIM];
	bool current_player = 0;
	int level = 0;
	bool player;

	bool game_mode = select_game_mode();

	//enum mode m = select_mode();

	//enum difficulty d = select_difficulty();

	if(!game_mode){
		level = select_level();
		current_player = select_player();
	}
	
	init_board(board);
	while (!is_full(board) && !is_winner(board)) {
		print_board(board);
		if(!game_mode){
			if(current_player){
				make_move_bot(board, current_player, level);
			}else{
				make_move(board, current_player);
			}
		}else{
			make_move(board, current_player);
		}
		current_player = !current_player;
	}
	if (is_winner(board)) {
		printf("Game is over: Player %c has won.\n",
		       !current_player ? 'X' : 'O');
	} else {
		puts("Board is full. Game is drawn.");
	}
	print_board(board);
}

//gcc *.c -o main.exe; if ($?) { ./main.exe }