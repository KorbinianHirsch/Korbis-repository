# Conway's Game of Life in C (SDL2)

A fast, visual implementation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) written in C, using the SDL2 library for rendering. 

The simulation features a wrapping (toroidal) grid, meaning gliders and patterns that travel off one edge of the screen will seamlessly reappear on the opposite side.

## Features
* **Randomized Initialization:** Every execution generates a unique starting grid.
* **Toroidal Grid:** The 100x100 grid wraps around the edges mathematically.
* **SDL2 Rendering:** Smooth and highly responsive visual output.

## Prerequisites

To compile and run this project, you need a C compiler (like `gcc` or `clang`) and the SDL2 development libraries installed on your system.

### Installing SDL2

**Debian/Ubuntu:**
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev