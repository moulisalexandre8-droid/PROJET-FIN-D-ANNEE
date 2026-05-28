#include "renderer.h"
#include "../utils/constants.h"

void dessinerTexture(SDL_Renderer* rendu, SDL_Texture* texture, int x, int y, int largeur, int hauteur)
{
    if (texture == NULL)
        return;

    SDL_Rect dest = {x, y, largeur, hauteur};
    SDL_RenderCopy(rendu, texture, NULL, &dest);
}

void dessinerJoueur(SDL_Renderer* rendu, SDL_Texture* texture, int x, int y, int tailleCaseX, int tailleCaseY)
{
    if (texture == NULL)
        return;

    SDL_Rect dest = {x - tailleCaseX / 2, y - tailleCaseY / 2, tailleCaseX, tailleCaseY};
    SDL_RenderCopy(rendu, texture, NULL, &dest);
}