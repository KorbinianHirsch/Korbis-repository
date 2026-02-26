import ctypes
import os
import pygame

from agent import PongAgent

WINDOW_SIZE = 600
BLACK = (20, 20, 20)
WHITE = (200, 200, 200)
PADDLE_COLOR = (0, 255, 0)
BALL_COLOR = (255, 50, 50)
PADDLE_X = 0.05
PADDLE_W = 0.02
PADDLE_H = 0.2
BALL_SIZE = 0.02

class PongEnvironment:
    def __init__(self):
        lib_path = os.path.abspath("game.dll")
        self.lib = ctypes.CDLL(lib_path)

        self.lib.reset.argtypes = []
        self.lib.reset.restype = None
        self.lib.step.argtypes = [ctypes.c_int]
        self.lib.step.restype = ctypes.c_float
        self.lib.is_done.argtypes = []
        self.lib.is_done.restype = ctypes.c_int
        self.lib.get_state.argtypes = []
        self.lib.get_state.restype = ctypes.POINTER(ctypes.c_float)

        self.state_size = 5 

    def reset(self):
        self.lib.reset()
        return self.get_state()

    def step(self, action):
        reward = self.lib.step(action)
        done = bool(self.lib.is_done())
        return reward, done

    def get_state(self):
        state_ptr = self.lib.get_state()
        return [state_ptr[i] for i in range(self.state_size)]

    def render(self, screen, state):
        screen.fill(BLACK)

        ball_x = state[0]
        ball_y = state[1]
        paddle_y = state[4]

        px_ball_size = int(BALL_SIZE * WINDOW_SIZE)
        px_ball_x = int(ball_x * WINDOW_SIZE - px_ball_size / 2)
        px_ball_y = int(ball_y * WINDOW_SIZE - px_ball_size / 2)

        px_pad_w = int(PADDLE_W * WINDOW_SIZE)
        px_pad_h = int(PADDLE_H * WINDOW_SIZE)
        px_pad_x = int(PADDLE_X * WINDOW_SIZE)
        px_pad_y = int(paddle_y * WINDOW_SIZE - px_pad_h / 2)

        pygame.draw.rect(screen, PADDLE_COLOR, (px_pad_x, px_pad_y, px_pad_w, px_pad_h))
        pygame.draw.rect(screen, BALL_COLOR, (px_ball_x, px_ball_y, px_ball_size, px_ball_size))

        pygame.draw.line(screen, WHITE, (WINDOW_SIZE - 2, 0), (WINDOW_SIZE - 2, WINDOW_SIZE), 4)

        pygame.display.flip()

def main():
    pygame.init()
    screen = pygame.display.set_mode((WINDOW_SIZE, WINDOW_SIZE))
    pygame.display.set_caption("KI plays Pong")
    clock = pygame.time.Clock()

    env = PongEnvironment()

    agent = PongAgent(input_size=5)

    agent.load_model("pong_brain.pth") 
    agent.epsilon = 0.00
    
    state = env.reset()
    done = False
    hits = 0
    
    running = True
    game_active = True

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        if game_active:
            action = agent.get_action(state)
            reward, done = env.step(action)
            state = env.get_state()
            
            if reward > 0.05 and reward < 0.5:
                hits += 1
                pygame.display.set_caption(f"AI plays Pong | Hits: {hits}")
                
            env.render(screen, state)
            
            clock.tick(60) 

            if done:
                print(f"Game Over! Endscore (Hits): {hits}.")
                pygame.display.set_caption(f"GAME OVER | Final Hits: {hits}")
                game_active = False

    pygame.quit()

if __name__ == "__main__":
    main()