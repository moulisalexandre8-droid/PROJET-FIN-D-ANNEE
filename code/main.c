#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }

    // Création fenêtre
    SDL_Window* window = SDL_CreateWindow(
        "Cluelau",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        0
    );

    if (!window)
    {
        printf("Erreur fenetre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        printf("Erreur renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Position joueur
    int playerX = 100;
    int playerY = 100;

    // Boucle de jeu
    int running = 1;
    SDL_Event event;

    while (running)
    {
        // Gestion événements
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        // Fond noir
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Joueur (carré rouge)
        SDL_Rect player = {playerX, playerY, 50, 50};

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player);

        // Affichage
        SDL_RenderPresent(renderer);

        // petite pause CPU
        SDL_Delay(16);
    }

    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}