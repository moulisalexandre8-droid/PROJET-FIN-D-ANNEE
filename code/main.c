#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Cluelau",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int playerX = 100;
    int playerY = 100;

    int running = 1;
    SDL_Event event;

    while (running)
    {
        // Events (fermeture fenêtre)
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        // CLAVIER EN TEMPS RÉEL
        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_UP])
            playerY -= 5;

        if (state[SDL_SCANCODE_DOWN])
            playerY += 5;

        if (state[SDL_SCANCODE_LEFT])
            playerX -= 5;

        if (state[SDL_SCANCODE_RIGHT])
            playerX += 5;

        // dessine
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect player = {playerX, playerY, 50, 50};

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}