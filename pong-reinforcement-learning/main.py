import ctypes
import os
import matplotlib.pyplot as plt
import numpy as np
from collections import deque

from agent import PongAgent

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
        state = [state_ptr[i] for i in range(self.state_size)]
        return state

def main():
    env = PongEnvironment()
    agent = PongAgent(input_size=5)
    
    episodes = 10000
    scores = []
    losses = []
    
    print("=== Start Deep Q-Learning Training (PONG) ===")
    
    try:
        FRAME_SKIP = 4
        
        for e in range(episodes):
            state = env.reset()
            done = False
            hits = 0
            steps = 0
            ep_losses = []
            
            while not done:
                action = agent.get_action(state)

                total_reward = 0.0
                for _ in range(FRAME_SKIP):
                    reward, done = env.step(action)
                    total_reward += reward
                    if done:
                        break
                
                next_state = env.get_state()

                agent.memory.push(state, action, total_reward, next_state, done)

                loss = agent.train_step()
                if loss is not None:
                    ep_losses.append(loss)
                
                state = next_state
                steps += 1

                if total_reward > 0.05 and total_reward < 0.5:
                    hits += 1

                if steps > 5000:
                    done = True
                    
            scores.append(hits)

            if len(ep_losses) > 0:
                losses.append(np.mean(ep_losses))
            else:
                losses.append(0.0)
            
            if (e + 1) % 10 == 0:
                avg_score = np.mean(scores[-10:])
                print(f"Episode: {e+1:4d} | Avg Hits: {avg_score:4.1f} | Epsilon: {agent.epsilon:.3f}")
                agent.update_target_network()

    except KeyboardInterrupt:
        print("\n[Warnung] Training stopped manually(Strg+C)!")

    finally:
        print("=== Training done ===")
        agent.save_model("pong_brain.pth")
        

        window = 20
        moving_avg_scores = np.convolve(scores, np.ones(window)/window, mode='valid')
        moving_avg_losses = np.convolve(losses, np.ones(window)/window, mode='valid')

        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

        ax1.plot(scores, label='Balltouches', alpha=0.3, color='blue')
        ax1.plot(range(window-1, len(scores)), moving_avg_scores, label=f'{window}-Games average', color='red')
        ax1.set_title('Learning of the agent')
        ax1.set_ylabel('Succesful hits')
        ax1.legend()
        ax1.grid(True)

        ax2.plot(losses, label='Trainingsloss', alpha=0.3, color='orange')
        ax2.plot(range(window-1, len(losses)), moving_avg_losses, label=f'{window}-Games average', color='darkred')
        ax2.set_title('Error of neural net')
        ax2.set_xlabel('Episodes')
        ax2.set_ylabel('Loss (MSE)')
        ax2.legend()
        ax2.grid(True)
        
        plt.tight_layout()
        plt.show()

if __name__ == "__main__":
    main()