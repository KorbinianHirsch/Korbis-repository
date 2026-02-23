enum difficulty {BOT_RANDOM = 1, BOT_MEDIUM = 2, BOT_HARD = 3};

int make_move_bot_level1(char board[3][3]);

int make_move_bot_level2(char board[3][3], bool player);

int return_weight(char board[3][3], int depth, bool player);

int make_move_bot_level3(char board[3][3], bool player);

int make_move_bot(char board[3][3], bool player, enum difficulty level);