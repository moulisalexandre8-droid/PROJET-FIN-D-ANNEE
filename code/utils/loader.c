#include "loader.h"
#include "../utils/constants.h"

#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture* chargerTexture(SDL_Renderer* rendu, const char* chemin)
{
    SDL_Texture* texture = IMG_LoadTexture(rendu, chemin);

    if (texture == NULL)
    {
        printf("Erreur chargement texture %s : %s\n",
               chemin,
               IMG_GetError());
    }

    return texture;
}