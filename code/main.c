#include <SDL2/SDL.h>
#include <stdio.h>

// Là déja je vais m'occuper de la fenetre parce que j'ai vraiment envie qu'il en ai une,quitte a faire les fonctionnalittée plus tars, en plus j'en avais déja fait une il y a longtemps donc je me rappelle des commandes et tout ce qui va avec

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

    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}