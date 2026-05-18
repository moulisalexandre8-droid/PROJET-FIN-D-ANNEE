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

void dessinerInterfaceDroite(SDL_Renderer* rendu)
{
    SDL_Rect panneau = {950, 0, 450, 860};

    // fond du panneau
    SDL_SetRenderDrawColor(rendu, 35, 35, 35, 255);
    SDL_RenderFillRect(rendu, &panneau);

    // séparation verticale
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
    SDL_RenderDrawLine(rendu, 950, 0, 950, 860);

    // zone dé
    SDL_Rect de = {1020, 50, 120, 120};

    SDL_SetRenderDrawColor(rendu, 220, 220, 220, 255);
    SDL_RenderFillRect(rendu, &de);

    // zone boutons
    SDL_Rect bouton1 = {1000, 250, 300, 60};
    SDL_Rect bouton2 = {1000, 340, 300, 60};

    SDL_SetRenderDrawColor(rendu, 90, 90, 90, 255);
    SDL_RenderFillRect(rendu, &bouton1);
    SDL_RenderFillRect(rendu, &bouton2);

    // zone carnet
    SDL_Rect carnet = {980, 470, 340, 320};

    SDL_SetRenderDrawColor(rendu, 60, 60, 60, 255);
    SDL_RenderFillRect(rendu, &carnet);
}