#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Cluelau",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1550,
        850,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        printf("Erreur renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Taille d'une case (grille Cluedo)
    int tileSize = 50;

    // Position joueur (alignée sur grille)
    int playerX = 100;
    int playerY = 100;

    int running = 1;
    SDL_Event event;

    while (running)
    {
        // fermer la fenetre
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        // clavier
        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_UP])
            playerY -= tileSize;

        if (state[SDL_SCANCODE_DOWN])
            playerY += tileSize;

        if (state[SDL_SCANCODE_LEFT])
            playerX -= tileSize;

        if (state[SDL_SCANCODE_RIGHT])
            playerX += tileSize;

        // limite des ecrans
        if (playerX < 0) playerX = 0;
        if (playerY < 0) playerY = 0;

        if (playerX > 1550 - tileSize) playerX = 1550 - tileSize;
        if (playerY > 850 - tileSize) playerY = 850 - tileSize;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Joueur
        SDL_Rect player = {playerX, playerY, tileSize, tileSize};

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