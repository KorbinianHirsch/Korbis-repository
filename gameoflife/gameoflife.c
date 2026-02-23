#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ROWS 100
#define COLS 100
#define SCALE 8
#define EPOCHS 1000

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)


bool is_valid(int curr_row, int curr_col){
    if(curr_row < 0 || curr_row >= ROWS) return false;
    if(curr_col < 0 || curr_col >= COLS) return false;
}

bool toggle(bool curr_state, int active_neighbors){
    if(curr_state){
        if(active_neighbors == 2 || active_neighbors == 3) return true;
        return false;
    }else{
        if(active_neighbors == 3) return true;
        return false;
    }
}

void update_state(int curr_row, int curr_col, bool grid[ROWS][COLS], bool buffer[ROWS][COLS]){
    int start_row = curr_row - 1;
    int start_col = curr_col - 1;
    int active_neighbors = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(i == 1 && j == 1) continue;
            int idx_y = (start_row + i) % ROWS;
            int idx_x = (start_col + j) % COLS;
            active_neighbors += grid[idx_y][idx_x];
        }
    }
    buffer[curr_row][curr_col] = toggle(grid[curr_row][curr_col], active_neighbors);
}


void new_epoch(bool grid[ROWS][COLS], bool buffer[ROWS][COLS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            update_state(i, j, grid, buffer);
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid[i][j] = buffer[i][j];
        }
    }
}

void init_grid(bool grid[ROWS][COLS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            grid[i][j] = rand() % 2;
        }
    }
}

void print_grid(bool grid[ROWS][COLS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void draw_board(SDL_Renderer *renderer, bool grid[ROWS][COLS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            int x = CENTER_X - (COLS*SCALE / 2) + (i*SCALE);
            int y = CENTER_Y + (ROWS*SCALE / 2) - (j*SCALE) - SCALE;
            if(grid[i][j]){
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect pointRect = {x, y, SCALE, SCALE};
                SDL_RenderFillRect(renderer, &pointRect);
            }else{
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
                SDL_Rect pointRect = {x, y, SCALE, SCALE};
                SDL_RenderFillRect(renderer, &pointRect);
            }
        }
    }
}


void run_animation(bool grid[ROWS][COLS],bool buffer[ROWS][COLS]) {
    // 1. Initialisierung (NUR EINMAL)
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL konnte nicht initialisiert werden! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    SDL_Window* window = SDL_CreateWindow("Cluster-Animation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    init_grid(grid);
    int quit = 0;
    SDL_Event e;
    int current_epoch = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        new_epoch(grid, buffer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        draw_board(renderer, grid);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char** argv){
    SDL_Init(SDL_INIT_TIMER); 
    srand((unsigned int)SDL_GetPerformanceCounter());

    bool grid[ROWS][COLS];
    bool buffer[ROWS][COLS];

    run_animation(grid, buffer);


    return 0;
}