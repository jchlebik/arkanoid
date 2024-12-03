#include <SDL2/SDL.h>
#include <iostream>

int main() {

    constexpr int width = 800;
    constexpr int height = 600;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Hello World", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, 
        SDL_WINDOW_SHOWN);

    if (!window) 
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    SDL_Event windowEvent;
    while(true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT || windowEvent.type == SDL_KEYDOWN)
            {
                break;
            }
        }

    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}