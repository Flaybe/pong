# Terminal Pong 🏓

A simple Pong game written in C++, played directly in the terminal — no graphics libraries, just text. The goal is to mess around with input and logic, and have some fun.

## What's in the code?

* ASCII-based game board drawn in the terminal
* Player paddle controlled by keyboard (`W`/`S` or arrow keys)
* Ball that moves and bounces
* Basic collision detection
* Non-blocking keyboard input (via `termios`)
* Simple game loop

## How to run it

1. Make sure you have `g++` installed.

2. Clone the repo or download the files.

3. In the project directory, run:

   ```bash
   make pong
   ```

4. Then run the game:

   ```bash
   ./pong
   ```

Works best in a Linux terminal.

## Why?

This project is mostly for learning and fun. No external libraries, just pure C++
