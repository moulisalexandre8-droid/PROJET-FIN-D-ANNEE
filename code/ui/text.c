#include "text.h"
#include <stdio.h>

SDL_Texture* creerTexte(SDL_Renderer* rendu, TTF_Font* font, const char* texte, SDL_Color couleur)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, texte, couleur);

    if (!surface)
    {
        printf("Erreur texte: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void dessinerTexteCentre(SDL_Renderer* rendu, SDL_Texture* texture, int x, int y, int w, int h)
{
    int tw, th;
    SDL_QueryTexture(texture, NULL, NULL, &tw, &th);

    SDL_Rect dest = {
        x + (w - tw) / 2,
        y + (h - th) / 2,
        tw,
        th
    };

    SDL_RenderCopy(rendu, texture, NULL, &dest);
}