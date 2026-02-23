#include "ui.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void print_board(char board[3][3])
{
	printf("\n");

	printf("  ");
	for (int c = 0; c < 3; c++) {
		printf(" %d ", c);
		if (c < 2)
			printf(" ");
	}
	printf("\n");

	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			if (c == 0)
				printf("%d ", r);
			printf(" %c ", board[r][c]);
			if (c < 2)
				printf("|");
		}
		printf("\n");
		if (r < 2)
			printf("  -----------\n");
	}
	printf("\n");
}


int select_level(){
	int level, result;
	do {
		printf("Please enter the difficulty (1 = easy, 2 = medium, 3 = hard): ");
		result = scanf("%d", &level);
		if (result == EOF) {
			puts("end of input file reached, terminate program");
			exit(1);
		}

		// empty buffer
		int character;
		while ((character = getchar()) != '\n' && character != EOF)
			;

		// to prevent undefined behavior
		if (result != 1) {
			puts("input invalid, please enter one number.");
			// result bleibt != 2 → Schleife wiederholt sich
			continue;
		}

		if (level < 1 || level > 3) {
			puts("input invalid, two numbers between 1 and 3 must "
			     "be specified, please repeat input");
			result = 0;
		}
	} while (result != 1);

	return level;
}

bool select_game_mode(){
	bool game_mode, result;
	do {
		printf("Please enter the Gamemode (0 = SinglePlayer (vs Bot), 1 = Mulitplayer: ");
		result = scanf("%d", &game_mode);
		if (result == EOF) {
			puts("end of input file reached, terminate program");
			exit(1);
		}

		// empty buffer
		int character;
		while ((character = getchar()) != '\n' && character != EOF)
			;

		// to prevent undefined behavior
		if (result != 1) {
			puts("input invalid, please enter one number.");
			// result bleibt != 2 → Schleife wiederholt sich
			continue;
		}

		if (game_mode != 0 && game_mode != 1) {
			puts("input invalid, two numbers between 1 and 3 must "
			     "be specified, please repeat input");
			result = 0;
		}
	} while (result != 1);

	return game_mode;
}

bool select_player(){
	bool player, result;
	do {
		printf("Please enter the first Player (0 = YOU START, 1 = BOT STARTS: ");
		result = scanf("%d", &player);
		if (result == EOF) {
			puts("end of input file reached, terminate program");
			exit(1);
		}

		// empty buffer
		int character;
		while ((character = getchar()) != '\n' && character != EOF)
			;

		// to prevent undefined behavior
		if (result != 1) {
			puts("input invalid, please enter one number.");
			// result bleibt != 2 → Schleife wiederholt sich
			continue;
		}

		if (player != 0 && player != 1) {
			puts("input invalid, two numbers between 1 and 3 must "
			     "be specified, please repeat input");
			result = 0;
		}
	} while (result != 1);

	return player;
}

void make_move(char board[3][3], bool player)
{
	int r, c, result;
	do {
		printf("Player %c, enter row and column (0-2 0-2): ", player ? 'X' : 'O');
		result = scanf("%d %d", &r, &c);
		if (result == EOF) {
			puts("end of input file reached, terminate program");
			exit(1);
		}

		// empty buffer
		int character;
		while ((character = getchar()) != '\n' && character != EOF)
			;

		// to prevent undefined behavior
		if (result != 2) {
			puts("input invalid, please enter two numbers.");
			// result bleibt != 2 → Schleife wiederholt sich
			continue;
		}

		if (r < 0 || r > 2 || c < 0 || c > 2 || board[r][c] != ' ') {
			puts("input invalid, two numbers between 0 and 2 must "
			     "be specified, please repeat input");
			result = 0;
		}
	} while (result != 2);
	board[r][c] = player ? 'X' : 'O';
}
