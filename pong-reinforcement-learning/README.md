# Deep Reinforcement Learning: Pong Agent 🏓

## Overview
This repository contains a Deep Q-Network (DQN) agent trained to play the classic arcade game Pong. The project features a custom C-based physics engine for high-performance environment simulation, seamlessly integrated with Python and PyTorch for neural network training, and Pygame for real-time visualization.

## Features
* **Algorithm:** Deep Q-Learning (DQN) with Experience Replay
* **Neural Network:** PyTorch Feedforward Neural Network (FNN)
* **Environment:** Custom C-Engine via `ctypes` for lightning-fast training
* **Rendering:** Pygame integration for visual evaluation and debugging

## Requirements
* Python 3.8+
* PyTorch
* Pygame
* NumPy
* Matplotlib
* GCC (for compiling the C-engine)

## Installation

1. **Clone the repository**
    ```bash
   git clone [https://github.com/yourusername/pong-rl-agent.git](https://github.com/yourusername/pong-rl-agent.git)
   cd pong-rl-agent
   ```

2. **Install Python dependencies**
    ```bash
    pip install torch pygame numpy matplotlib
    ```

3. **Compile the Physics Engine**
    Compile the engine.c file into a shared library so Python can access it.
    
    Windows:

    ```bash
    gcc -shared -o engine.dll engine.c -O3
    ```

    Linux / macOS:

    ```bash
    gcc -shared -fPIC -o engine.so engine.c -O3
    ```

4. **Usage**

    Training the Agent:

    To train the neural network from scratch without rendering (for maximum speed), run:
    ```bash
    python train.py
    ```

    This script will execute the training loop, periodically save the best weights to pong_brain.pth, and display a plot showing the training progress (Score & Loss) upon completion.

    Evaluating / Watching the Agent:
    To watch the trained agent play using the visual Pygame environment, run:
    ```bash
    python main.py
    ```

    Note: Ensure that pong_brain.pth is present in the root directory before running this command.