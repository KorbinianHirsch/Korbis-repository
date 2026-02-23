#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum difficulty {BOT_RANDOM = 1, BOT_MEDIUM = 2, BOT_HARD = 3};


int make_move_bot_level1(char board[3][3]){
    while(true){
        int index = rand() % 9;
        if(board[index/3][index%3] == ' ') return index;
    }
}

int make_move_bot_level2(char board[3][3], bool player){
    //Zuerst überprüfen ob bot mit seinem Zug gewinnen kann
    for(int i = 0; i < 9; i++){
        if(board[i / 3][i % 3] != ' ') continue;
        board[i / 3][i % 3] = player ? 'X' : 'O';
        if(is_winner(board)){
            board[i / 3][i % 3] = ' ';
            return i;
        }
        board[i / 3][i % 3] = ' ';
    }
    //Wenn nicht, kann er einen Gewinnzug des Gegners verhindern 
    for(int i = 0; i < 9; i++){
        if(board[i / 3][i % 3] != ' ') continue;
        board[i / 3][i % 3] = player ? 'O' : 'X';
        if(is_winner(board)){
            board[i / 3][i % 3] = ' ';
            return i;
        }
        board[i / 3][i % 3] = ' ';
    }
    //Wenn keine Gefahr oder Gewinn möglich --> random move
    return make_move_bot_level1(board);
}

int return_weight(char board[3][3], int depth, bool player){
    int maximum = -10000;
    int minimum = 10000;
    int curr_val;
    if(is_winner(board)) return !player ? 10 - depth : -10 + depth;
    if(is_full(board)) return 0;
    for(int i = 0; i < 9; i++){
        if(board[i / 3][i % 3] != ' ') continue;
        if(player){
            board[i / 3][i % 3] = 'X';
            curr_val = return_weight(board, depth + 1, !player);
            if(curr_val > maximum) maximum = curr_val;
        }else{
            board[i / 3][i % 3] = 'O';
            curr_val = return_weight(board, depth + 1, !player);
            if(curr_val < minimum) minimum = curr_val;
        }
        board[i / 3][i % 3] = ' ';
    }
    return player ? maximum : minimum;
}

int make_move_bot_level3(char board[3][3], bool player){
    int best_index;
    int curr_val;
    int maximum = -10000;
    int minimum = 10000;
    for(int i = 0; i < 9; i++){
        if(board[i / 3][i % 3] != ' ') continue;
        board[i / 3][i % 3] = player ? 'X' : 'O';
        if(is_winner(board)){
            board[i / 3][i % 3] = ' ';
            return i;
        }  
        curr_val = return_weight(board, 0, !player);
        if(player){
            if(curr_val > maximum){
                maximum = curr_val;
                best_index = i;
            }
        }else{
            if(curr_val < minimum){
                minimum = curr_val;
                best_index = i;
            }
        }
        board[i / 3][i % 3] = ' ';
    }
    return best_index;
}

void make_move_bot(char board[3][3], bool player, enum difficulty level){
    //Zug abhängig vom Skilllevel ausführen 
    int index;
    switch (level){
        case 1: 
            index = make_move_bot_level1(board);
            break;
        case 2: 
            index = make_move_bot_level2(board, player);
            break;
        case 3: 
            index = make_move_bot_level3(board, player);
    }
    board[index / 3][index % 3] = player ? 'X' : 'O';
}