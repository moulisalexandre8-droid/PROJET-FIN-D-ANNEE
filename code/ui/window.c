#include "window.h"

void dessinerInterfaceDroite(SDL_Renderer* rendu)
{
    SDL_Rect panneau = {950, 0, 450, 860};

    // fond panneau
    SDL_SetRenderDrawColor(rendu, 35, 35, 35, 255);
    SDL_RenderFillRect(rendu, &panneau);

    // séparation
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
    SDL_RenderDrawLine(rendu, 950, 0, 950, 860);

    // zone dé
    SDL_Rect de = {1020, 50, 120, 120};

    SDL_SetRenderDrawColor(rendu, 220, 220, 220, 255);
    SDL_RenderFillRect(rendu, &de);

    SDL_SetRenderDrawColor(rendu, 90, 90, 90, 255);

    // carnet
    SDL_Rect carnet = {980, 470, 340, 320};

    SDL_SetRenderDrawColor(rendu, 60, 60, 60, 255);
    SDL_RenderFillRect(rendu, &carnet);
}