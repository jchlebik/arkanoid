# arkanoid
Arkanoid implementation using SDL2

## Introduction
This project is an implementation of the classic Arkanoid game using the SDL2 library. The game involves controlling a paddle to bounce a ball and break bricks.

## Installation
To install and run this project, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/arkanoid.git
    ```
2. Navigate to the project directory:
    ```sh
    cd arkanoid
    ```
3. Install the required dependencies [SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.10) and [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)

4. Build the project using the provided CMAKE:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

## Usage
To play the game, run the following command:
```sh
./Arkanoid
```

## Extending
The implementation is very barebones but modular. Extending it with extra logic (special types of bricks, ball logic or different layouts) should be easy given more time. Look into the headers for documentation and then into the src files.