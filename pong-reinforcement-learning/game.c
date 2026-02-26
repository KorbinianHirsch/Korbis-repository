#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    float x;      
    float y;      
    float dx;     
    float dy;     
    float size;   
} Ball;

typedef struct {
    float x;      
    float y;      
    float width;  
    float height; 
    float speed;  
} Paddle;

typedef struct {
    Ball ball;
    Paddle player;
    int score;
    int done;
} PongEnvironment;

PongEnvironment game;

void reset() {
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }

    game.ball.x = 0.5f;

    game.ball.y = 0.2f + ((float)rand() / RAND_MAX) * 0.6f;
    
    game.ball.dx = -0.015f - (((float)rand() / RAND_MAX) * 0.020f); 
    
    game.ball.dy = -0.020f + (((float)rand() / RAND_MAX) * 0.040f); 
   
    game.ball.size = 0.02f; 

    game.player.x = 0.05f;  
    game.player.y = 0.5f;   
    game.player.width = 0.02f;
    game.player.height = 0.05f;
    game.player.speed = 0.03f; 

    game.score = 0;
    game.done = 0;
}

float step(int action) {
    float reward = 0.0f;
    game.done = 0;

    float p_half_h = game.player.height / 2.0f;
    float b_half_s = game.ball.size / 2.0f;

    if (action == 0) {
        game.player.y -= game.player.speed;
        reward -= 0.001f;
    } else if (action == 1) {
        game.player.y += game.player.speed;
        reward -= 0.001f;
    }

    if (game.player.y - p_half_h < 0.0f) {
        game.player.y = p_half_h;
    } else if (game.player.y + p_half_h > 1.0f) {
        game.player.y = 1.0f - p_half_h;
    }

    float abs_dx = game.ball.dx > 0 ? game.ball.dx : -game.ball.dx;
    float abs_dy = game.ball.dy > 0 ? game.ball.dy : -game.ball.dy;
    float max_speed_this_frame = abs_dx > abs_dy ? abs_dx : abs_dy;

    float safe_step_size = b_half_s; 
    int steps = (int)(max_speed_this_frame / safe_step_size) + 1;

    float step_dx = game.ball.dx / steps;
    float step_dy = game.ball.dy / steps;

    for (int i = 0; i < steps; i++) {
        game.ball.x += step_dx;
        game.ball.y += step_dy;

        if (game.ball.y - b_half_s <= 0.0f) {
            game.ball.y = b_half_s;  
            game.ball.dy *= -1.0f;
            step_dy *= -1.0f;
        } else if (game.ball.y + b_half_s >= 1.0f) {
            game.ball.y = 1.0f - b_half_s;
            game.ball.dy *= -1.0f;
            step_dy *= -1.0f; 
        }

        if ((game.ball.x - b_half_s <= game.player.x + game.player.width) && 
            (game.ball.x + b_half_s >= game.player.x) &&                    
            (game.ball.y - b_half_s <= game.player.y + p_half_h) &&         
            (game.ball.y + b_half_s >= game.player.y - p_half_h)) {         

            game.ball.x = game.player.x + game.player.width + b_half_s; 

            game.ball.dx *= -1.02f; 
            game.ball.dy *= 1.02f;

            float random_bounce = -0.005f + ((float)rand() / RAND_MAX) * 0.01f;
            game.ball.dy += random_bounce;

            float max_speed_x = 0.15f;
            float max_speed_y = 0.035f;
            
            if (game.ball.dx > max_speed_x) game.ball.dx = max_speed_x;
            if (game.ball.dx < -max_speed_x) game.ball.dx = -max_speed_x;
            
            if (game.ball.dy > max_speed_y) game.ball.dy = max_speed_y;
            if (game.ball.dy < -max_speed_y) game.ball.dy = -max_speed_y;

            step_dx = game.ball.dx / steps;
            step_dy = game.ball.dy / steps;

            reward += 0.1f;
        }

        if (game.ball.x > 1.0f) {
            game.ball.x = 1.0f - b_half_s;
            game.ball.dx *= -1.0f; 
            step_dx *= -1.0f;
            reward += 1.0f;
        }

        if (game.ball.x < 0.0f) {
            game.done = 1;
            reward -= 1.0f;
            break;
        }
    }

    return reward;
}

int is_done() {
    return game.done;
}

static float state[5];

float* get_state() {
    state[0] = game.ball.x;
    state[1] = game.ball.y;
    state[2] = game.ball.dx;
    state[3] = game.ball.dy;
    state[4] = game.player.y;
    
    return state; 
}