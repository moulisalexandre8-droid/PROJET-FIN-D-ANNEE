#include "renderer.h"
#include "../utils/constants.h"

void dessinerTexture(SDL_Renderer* rendu,SDL_Texture* texture,int x,int y,int largeur,int hauteur)
{
    SDL_Rect dest ={x,y,largeur,hauteur};

    SDL_RenderCopy(rendu, texture, NULL, &dest);
}

void dessinerJoueur(SDL_Renderer* rendu,SDL_Texture* texture,int x,int y,int tailleCaseX,int tailleCaseY)
{
    SDL_Rect dest ={x,y,tailleCaseX,tailleCaseY};

    SDL_RenderCopy(rendu, texture, NULL, &dest);
}